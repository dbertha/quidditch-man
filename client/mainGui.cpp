#include "mainGui.hpp"

MainGui::MainGui(int sockfd,QMainWindow *parent) : sockfd_(sockfd), parent_(parent) {
    setFixedSize(800,480);
    createActions();
    firstMenu();
    setWindowTitle(tr("Quidditch Manager 2014"));
    loginDialog = new LoginDialog(sockfd_,this);
    nbPlayers=money=nbFans=nbActionPoints=0;
    login();
}

MainGui::~ MainGui() {}

void MainGui::run() {
   this->show();
}
void MainGui::quit() {
    std::cout<<"quit"<<std::endl;
}
int MainGui::badConnection() {
    QErrorMessage *errorMessageDialog = new QErrorMessage(this);
    errorMessageDialog->showMessage(tr("No connection with the server."));
    return (0);
}
void MainGui::buildings() {
    buildingsDialog = new BuildingsDialog(sockfd_,this);
    buildingsDialog->exec();
}
void MainGui::listPlayers() {
    ticker->hide();
    int res = choosePlayer(sockfd_,this);
    if (res==BAD_CONNECTION) badConnection();
    else {
// .............
    }
    ticker->show();
}

void MainGui::listMgrs() {
    ticker->hide();
    int res = choosePartner(sockfd_,this);
    if (res==BAD_CONNECTION) badConnection();
    else {
        std::vector<int> chosenPlayers;
        chosenPlayers = chooseTeamForMatch(sockfd_, this); //tous les rôles sont nécessairement remplis 
        //(on suppose suffisament de joueurs)
        std::cout << "index choisi : " << std::endl;
        for(unsigned int i = 0; i < chosenPlayers.size() ; ++i){
            std::cout << chosenPlayers[i] << std::endl;
        }
// .............
    }
    ticker->show();
}
void MainGui::login() {
    loginDialog->init();
    if (loginDialog->exec()==loginDialog->Accepted) {//connecte; on cree le menu
        role=loginDialog->getRole();
        createMenu();
    }
}
void MainGui::logout() {
    menuBar()->clear();
    firstMenu();
    ticker->hide();
    loginDialog->init(); //reset userName
}
void MainGui::createActions() {
    loginAction=new QAction(tr("Login"),this);
    exitAction=new QAction(tr("Exit"),this);
    aboutAction=new QAction(tr("About"),this);
    logoutAction=new QAction(tr("Logout"),this);
    listMgrsAction=new QAction(tr("List available managers"),this);
    listAuctionsAction=new QAction(tr("List auctions"),this);
    listPlayersAction=new QAction(tr("List my players"),this);
    buildingsAction=new QAction(tr("Open board"),this);
    listTournamentsAction=new QAction(tr("List tournaments"),this);
    newTournamentAction=new QAction(tr("New tournament"),this);
    newPromotionAction=new QAction(tr("Start a new promotion campaign"),this);
    buyAPAction=new QAction(tr("Buy action points"),this);
}

void MainGui::firstMenu() {
    // menu minimal - le mgr ne s'est pas encore identifie
        connect(aboutAction,SIGNAL(triggered()),this,SLOT(about()));
        connect(exitAction,SIGNAL(triggered()),this,SLOT(close()));
        connect(loginAction,SIGNAL(triggered()),this,SLOT(login()));

        fileMenu=menuBar()->addMenu(tr("File"));
        fileMenu->addAction(loginAction);
        fileMenu->addAction(exitAction);
        helpMenu=menuBar()->addMenu(tr("Help"));
    //    helpMenu->addAction(helpAction);
        helpMenu->addAction(aboutAction);
}

void MainGui::createMenu() {
    fileMenu->removeAction(loginAction);
//        fileMenu->addAction(logoutAction);
//        connect(logoutAction,SIGNAL(triggered()),this,SLOT(logout()));
    if (role==NORMAL_LOGIN) {
        matchMenu=menuBar()->addMenu(tr("Match"));
        matchMenu->addAction(listMgrsAction);
        connect(listMgrsAction,SIGNAL(triggered()),this,SLOT(listMgrs()));
        auctionMenu=menuBar()->addMenu(tr("Auctions"));
        auctionMenu->addAction(listAuctionsAction);
        playersMenu=menuBar()->addMenu(tr("Players"));
        playersMenu->addAction(listPlayersAction);
        connect(listPlayersAction,SIGNAL(triggered()),this,SLOT(listPlayers()));
        buildingsMenu=menuBar()->addMenu(tr("Buildings"));
        buildingsMenu->addAction(buildingsAction);
        connect(buildingsAction,SIGNAL(triggered()),this,SLOT(buildings()));
        tournamentsMenu=menuBar()->addMenu(tr("Tournaments"));
        tournamentsMenu->addAction(listTournamentsAction);
        actionPointsMenu=menuBar()->addMenu(tr("Action Points"));
        actionPointsMenu->addAction(newPromotionAction);
        actionPointsMenu->addAction(buyAPAction);
        ticker = new Ticker(sockfd_, this);
        ticker->show();
    }
    else if (role==ADMIN_LOGIN) {
        tournamentsMenu=menuBar()->addMenu(tr("Tournaments"));
        tournamentsMenu->addAction(newTournamentAction);
        tournamentsMenu->addAction(listTournamentsAction);
    }
}
void MainGui::about() {
    QMessageBox::about(this, tr("Quidditch Manager 2014"),
               "<p>MANON LEGRAND"
               "<p>HELENE PLISNIER"
               "<p>DAVID BERTHA"
               "<p>AUDRY DELESTREE");
}
int MainGui::getMoney() {return money;}
int MainGui::getNbPlayers() {return nbPlayers;}
int MainGui::getNbFans() {return nbFans;}
int MainGui::getActionPoints() {return nbActionPoints;}
void MainGui::setMoney(const int m) {money=m;}
void MainGui::setNbPlayers(const int m) {nbPlayers=m;}
void MainGui::setNbFans(const int m) {nbFans=m;}
void MainGui::setActionPoints(const int m) {nbActionPoints=m;}