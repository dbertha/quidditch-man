#include "AuctionsDialog.hpp"
#include "AuctionWindow.hpp"

AuctionsDialog::AuctionsDialog(Client * client, int auctionID, int startingPrice, QWidget *parent)
    : QDialog(parent), __client(client),auctionID_(auctionID),startingPrice_(startingPrice) {

    buildingsModel=new BuildingsModel(this);
    hAxe<<"Current level"<<"Trainings left"<<"Bonus brtick";
    vAxe<<"Speed"<<"Strength"<<"Precision"<<"Reflex"<<"Resistance";
    buildingsModel->setAxes(hAxe,vAxe);
    loadedData.resize(vAxe.length());

    tableView = new QTableView(this);
    tableView->setModel(buildingsModel);
    tableView->setAlternatingRowColors(true);
    selectionModel = tableView->selectionModel();
    nbPlayers=money=nbFans=actionPoints=0;
    getInfo();
    QString str=QString("Life = %1 (max = %2) - Estimated value = %3 gold").arg(playerInfos[13]).arg(playerInfos[4]).arg(playerInfos[14]);
    lifeValueLabel = new QLabel(str);
    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tableView);
    mainLayout->addWidget(lifeValueLabel);
    if (playerInfos[10]==1) {
        blockedLabel = new QLabel(tr("This player is blocked."));
        mainLayout->addWidget(blockedLabel);
    }
    else if (actionPoints<AP_ENTER_AUCTION) {
        blockedLabel = new QLabel(tr("Not enough action points to enter an auction."));
        mainLayout->addWidget(blockedLabel);
        }
    else {
    joinButton = new QPushButton(tr("Join"));
    joinButton->setDefault(true);
           
    cancelButton= new QPushButton(tr("Cancel"));
    bottomLayout = new QHBoxLayout;
    bottomLayout->addWidget(joinButton);
    bottomLayout->addWidget(cancelButton);
    connect(joinButton, SIGNAL(clicked()), this, SLOT(join()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    mainLayout->addLayout(bottomLayout);
    strActions<<"join ";
    }
    mainLayout->addStretch();
    setLayout(mainLayout);
    setMinimumWidth(415);
}
void AuctionsDialog::getInfo() {
    //loads the data presented by the model
    __client->receiveManagerInfos(&nbPlayers,&money,&nbFans,&actionPoints);
    playerInfos = __client->receivePlayerInfo(auctionID_);
    int len=vAxe.length();
    for (int j=0;j<len;++j) {
        loadedData[j].clear();
        QString str = QString::number(playerInfos[j]);
        loadedData[j]<<str;
        str = QString::number(playerInfos[j+len]);
        if ((playerInfos[12]>0)&&(playerInfos[11]==j))
            loadedData[j]<<str<<"v";//bonus
        else loadedData[j]<<str<<" ";
    }
    buildingsModel->loadData(&loadedData);
}
void AuctionsDialog::join() {
    __client->joinAuction(auctionID_);
    int joinResult = __client->getConfirmation();
    if (joinResult==1){
        auctionWindow = new AuctionWindow(__client,startingPrice_,this);
        auctionWindow->exec();
    }
    else {
        QErrorMessage* errorMessageDialog = new QErrorMessage(this);
        errorMessageDialog->showMessage(tr("You don't have enough action points to join this auction"));
    }
    reject();
}

void AuctionsDialog::badConnection() {
    QErrorMessage *errorMessageDialog = new QErrorMessage(this);
    errorMessageDialog->showMessage(tr("No connection with the server."));
    reject();
}
