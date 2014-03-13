#include "mainGui.hpp"
#include "clientMatchHandler.hpp"
#include "playerMgr.hpp"

MainGui::MainGui(int sockfd,QMainWindow *parent) : parent_(parent), __client(new Client(sockfd, true)) {
    setFixedSize(800,640);
    createActions();
    firstMenu();
    setWindowTitle(tr("Quidditch Manager 2014"));
    loginDialog = new LoginDialog(__client,this);
    nbPlayers=money=nbFans=nbActionPoints=0;
    login();
}

MainGui::~ MainGui() {
    delete __client;
    __client = NULL;
}

int MainGui::badConnection() {
    QErrorMessage *errorMessageDialog = new QErrorMessage(this);
    errorMessageDialog->showMessage(tr("No connection with the server."));
    return (0);
}
void MainGui::buildings() {
    buildingsDialog = new BuildingsDialog(__client,this);
    buildingsDialog->exec();
}
void MainGui::listPlayers() {
    if (choosePlayer(__client,this)==BAD_CONNECTION) badConnection();
}
void MainGui::tournaments() {
    //this slots acts for both manager and administrator as well :
    //if manager, the user may choose to participate to a tournament
    //if administrator, the user can create a new tournament
    chooseTournament(__client,role,this);
}

void MainGui::listMgrs() {
    ticker->hide();
    int res = choosePartner(__client,this);
    if (res==BAD_CONNECTION) badConnection();
    else if(res != NO_CHOICE){
        std::vector<int> chosenPlayers;
        chosenPlayers = chooseTeamForMatch(__client, this); //tous les rôles sont nécessairement remplis 
        //(on suppose suffisament de joueurs)
#ifdef __DEBUG
        std::cout << "index choisi : " << std::endl;
        for(unsigned int i = 0; i < chosenPlayers.size() ; ++i){
            std::cout << chosenPlayers[i] << std::endl;
        }      
#endif
        //send invitation
        __client->proposeMatchTo(res,  chosenPlayers);
        if(__client->receiveMatchConfirmation() == MATCH_STARTING){
            //startMatch(1); //inviteur a l'équipe 1
        }      
    }
    ticker->show();
}
    

//~ void MainGui::listAndChooseTournaments(){
    //~ ticker->hide();
    //~ int res = chooseTournament(__client,this);
    //~ if (res==BAD_CONNECTION) badConnection();
    //~ else if(res != NO_CHOICE){
        //~ __client->askToJoinTournament(); //pour le moment, un seul tournoi à la fois
        //~ int confirmation = __client->getConfirmation();
        //~ QMessageBox msgBox;
 //~ 
 //~ 
        //~ if(confirmation == 0){
            //~ msgBox.setText("Impossible to join this tournament !");
        //~ }else{
            //~ msgBox.setText("You are recorded as a participant of this tournament. Be ready for when it will start !");
//~ 
        //~ }
        //~ msgBox.exec();
    //~ }

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
    buildingsAction=new QAction(tr("List my buildings"),this);
    listTournamentsAction=new QAction(tr("List tournaments"),this);
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
        connect(listTournamentsAction,SIGNAL(triggered()),this,SLOT(tournaments()));
        actionPointsMenu=menuBar()->addMenu(tr("Action Points"));
        actionPointsMenu->addAction(newPromotionAction);
        actionPointsMenu->addAction(buyAPAction);
        ticker = new Ticker(__client, this);
        ticker->show();
        mainMenu = new MainMenu(this);
        setCentralWidget(mainMenu);
        mainMenu->show();
    }
    else if (role==ADMIN_LOGIN) {
        tournamentsMenu=menuBar()->addMenu(tr("Tournaments"));
        tournamentsMenu->addAction(listTournamentsAction);
        connect(listTournamentsAction,SIGNAL(triggered()),this,SLOT(tournaments()));
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
