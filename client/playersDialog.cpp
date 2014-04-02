#include "playersDialog.hpp"

PlayersDialog::PlayersDialog(Client * client, const int player, const int function, QWidget *parent)
    : QDialog(parent), player_(player), __client(client) {

    buildingsModel=new BuildingsModel(this);
    hAxe<<"Current level"<<"Trainings left"<<"Bonus brtick";
    vAxe<<"Speed"<<"Strength"<<"Precision"<<"Reflex"<<"Resistance";
    buildingsModel->setAxes(hAxe,vAxe);
    loadedData.resize(vAxe.length());
    //uses a model-view pattern with a custom BuildingsModel model
    tableView = new QTableView(this);
    tableView->setModel(buildingsModel);
    tableView->setAlternatingRowColors(true);
    selectionModel = tableView->selectionModel();
    nbPlayers=money=nbFans=actionPoints=0;
    getInfo();// gets all infos about the player
    QString str=QString("Life = %1 (max = %2) - Estimated value = %3 gold").arg(playerInfos[13]).arg(playerInfos[4]).arg(playerInfos[14]);
    lifeValueLabel = new QLabel(str);
    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tableView);
    mainLayout->addWidget(lifeValueLabel);
    if (playerInfos[10]==1) {
        blockedLabel = new QLabel(tr("This player is blocked."));
        mainLayout->addWidget(blockedLabel);
    }
    else if (function==AP_PLAYERMGR) {
            if (actionPoints<AP_AUCTION) {
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
                if (playerInfos[13]<playerInfos[4]) bottomLayout->addWidget(healButton);
                //this button is only needed if life is lesser than maximum
                bottomLayout->addWidget(sellButton);
//            bottomLayout->addWidget(broomstickButton);
                bottomLayout->addWidget(cancelButton);
                connect(trainButton, SIGNAL(clicked()), this, SLOT(train()));
                connect(healButton, SIGNAL(clicked()), this, SLOT(heal()));
                connect(sellButton, SIGNAL(clicked()), this, SLOT(sell()));
                connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
                mainLayout->addLayout(bottomLayout);
                strActions<<"train "<<"heal "<<"sell ";
            }
    }
    else {
        OKButton = new QPushButton(tr("Yes"));
        OKButton->setDefault(true);
        NOButton = new QPushButton(tr("No"));
        bottomLayout = new QHBoxLayout;
        bottomLayout->addWidget(OKButton);
        bottomLayout->addWidget(NOButton);
        connect(OKButton,SIGNAL(clicked()),this,SLOT(accept()));
        connect(NOButton,SIGNAL(clicked()),this,SLOT(reject()));
        mainLayout->addLayout(bottomLayout);
    }
    mainLayout->addStretch();
    setLayout(mainLayout);
    setMinimumWidth(415);
}
void PlayersDialog::getInfo() {
    //loads the data presented by the model
    if(__client->askForManagerInfos()==0) return badConnection();
    __client->receiveManagerInfos(&nbPlayers,&money,&nbFans,&actionPoints);
    if(__client->askForPlayerInfos(player_)==0) return badConnection();
    playerInfos = __client->receivePlayerInfo();
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
    if(QMessageBox::question(this,tr("Player"),str,
       QMessageBox::Ok | QMessageBox::Cancel)!=QMessageBox::Ok) return;

    switch (action) {
        case TRAIN :
            if (actionPoints<AP_TRAINING) break;
            if(__client->trainPlayer(player_,row)==0) return badConnection();
            if (__client->getConfirmation()) return confirmation();
            break;
        case HEAL :
            if (actionPoints<AP_HOSPITAL) break;
            if(__client->healPlayer(player_)==0) return badConnection();
            if (__client->getConfirmation()) return confirmation();
            break;
        case SELL :
            if (actionPoints<AP_AUCTION) break;
            bool ok;
            int price = QInputDialog::getInt(this,tr("New auction"),
                    tr("starting price"),0,0,playerInfos[14],1,&ok);
                    //starting price is limited by estimated value
            if (ok) {
                if(__client->sellPlayer(player_,price)==0) return badConnection();
                if (__client->getConfirmation()) {
                    str="Auction started !";
                    QMessageBox::information(this,tr("New auction"),str,QMessageBox::Ok);
                    accept();
                    return;
                }
            }
            else return;
    }
    str="Impossible : the player is blocked, this action is not needed or you do'nt have enough action points.";
    QMessageBox::critical(this,tr("Upgrade"),str,QMessageBox::Ok);
}
void PlayersDialog::confirmation() {
    if(__client->askForBuildingInfos(TRAININGCENTER)==0) return badConnection();
    std::vector<int> Infos = __client->receiveBuildingInfos();
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
