#include "playersDialog.hpp"

PlayersDialog::PlayersDialog(const int sockfd, const int player, QWidget *parent)
    : sockfd_(sockfd), QDialog(parent), player_(player) {

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
    else if (actionPoints<AP_AUCTION) {
        blockedLabel = new QLabel(tr("Not enough action points to manage your player."));
        mainLayout->addWidget(blockedLabel);
        }
        else {
            trainButton = new QPushButton(tr("Train"));
            trainButton->setDefault(true);
            healButton = new QPushButton(tr("Heal"));
            sellButton = new QPushButton(tr("Sell"));
            broomstickButton = new QPushButton(tr("Broomstick"));
            cancelButton= new QPushButton(tr("Cancel"));
            bottomLayout = new QHBoxLayout;
            bottomLayout->addWidget(trainButton);
            bottomLayout->addWidget(healButton);
            bottomLayout->addWidget(sellButton);
            bottomLayout->addWidget(broomstickButton);
            bottomLayout->addWidget(cancelButton);
            connect(trainButton, SIGNAL(clicked()), this, SLOT(train()));
            connect(healButton, SIGNAL(clicked()), this, SLOT(heal()));
            connect(sellButton, SIGNAL(clicked()), this, SLOT(sell()));
            connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
            mainLayout->addLayout(bottomLayout);
            strActions<<"train "<<"heal "<<"sell ";
    }
    mainLayout->addStretch();
    setLayout(mainLayout);
    setMinimumWidth(415);
}
void PlayersDialog::getInfo() {
    //loads the data presented by the model
    if(askForManagerInfos(sockfd_)==0) return badConnection();
    receiveManagerInfos(sockfd_,&nbPlayers,&money,&nbFans,&actionPoints);
    if(askForPlayerInfos(sockfd_,player_)==0) return badConnection();
    playerInfos = receivePlayerInfo(sockfd_);
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
void PlayersDialog::train() {select(TRAIN);}
void PlayersDialog::heal() {select(HEAL);}
void PlayersDialog::sell() {select(SELL);}

void PlayersDialog::select(const int action) {
    //interacts when the button is clicked
    int row = selectionModel->currentIndex().row();
    if(row<0 || row >=vAxe.length()) return;
    QString str ="Are you sure to "+strActions.at(action);
    if (action==TRAIN) str = str+" the "+vAxe.at(row)+" of";
    str +=" your player ?";
    if(QMessageBox::question(this,tr("Upgrade"),str,
       QMessageBox::Ok | QMessageBox::Cancel)!=QMessageBox::Ok) return;

    switch (action) {
        case TRAIN :
            if (actionPoints<AP_TRAINING) break;
            if(trainPlayer(sockfd_,player_,row)==0) return badConnection();
            if (getConfirmation(sockfd_)) return confirmation();
            break;
        case HEAL :
            if (actionPoints<AP_HOSPITAL) break;
            if(healPlayer(sockfd_,player_)==0) return badConnection();
            if (getConfirmation(sockfd_)) return confirmation();
            break;
        case SELL :
            if (actionPoints<AP_AUCTION) break;
            bool ok;
            int price = QInputDialog::getInt(this,tr("New auction"),
                    tr("starting price"),0,0,999999999,1,&ok);
            if (ok) {
                if(sellPlayer(sockfd_,player_,price)==0) return badConnection();
                if (getConfirmation(sockfd_)) {
                    str="Auction started !";
                    QMessageBox::information(this,tr("New auction"),str,QMessageBox::Ok);
                    accept();
                }
            }
            else return;
    }
    str="Impossible : the player is blocked or you do'nt have enough action points.";
    QMessageBox::critical(this,tr("Upgrade"),str,QMessageBox::Ok);
}
void PlayersDialog::confirmation() {
    if(askForBuildingInfos(sockfd_,TRAININGCENTER)==0) return badConnection();
    std::vector<int> Infos = receiveBuildingInfos(sockfd_);
    QString str="Starting... It will be over in "+
        QString::number(Infos[2])+" minutes !";
    QMessageBox::information(this,tr("Upgrade"),str,QMessageBox::Ok);
    accept();
}
void PlayersDialog::badConnection() {
    QErrorMessage *errorMessageDialog = new QErrorMessage(this);
    errorMessageDialog->showMessage(tr("No connection with the server."));
    reject();
}
