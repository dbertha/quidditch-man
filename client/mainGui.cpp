#include "mainGui.hpp"
#include "clientMatchHandler.hpp"
#include "playerMgr.hpp"
#include "auctionMgr.hpp"

MainGui::MainGui(int sockfd,QMainWindow *parent) : 
parent_(parent), __client(new Client(sockfd, true)), __pushesNotifier(new QSocketNotifier(sockfd, QSocketNotifier::Read, this)) {
    __pushesNotifier->setEnabled(false); //genère le signal
    connect(__pushesNotifier,SIGNAL(activated(int)),this,SLOT(pushesHandler()));
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
void MainGui::pushesHandler(){
    ticker->hide();
    __pushesNotifier->setEnabled(false);
    //receive
    SerializedObject received = receiveOnSocket(__client->getSockfd());
    
    //handle
    switch(received.typeOfInfos){
		case MATCH_INVITATION : {
            int IDInvitor;
            char name[USERNAME_LENGTH];
            char * position = received.stringData;
            bool confirmation;
            QString texte;
            std::vector<int> playersInTeam;
            memcpy(&IDInvitor, position, sizeof(IDInvitor));
            position += sizeof(IDInvitor);
            memcpy(&name, position, sizeof(name));
            //texte << "Do you want to fight against " << name << "with ID : " << QString::number(IDInvitor) << " ?";
            texte = QString("%1, with ID %2 wants to play a match against you !").arg(name, QString::number(IDInvitor));
            QMessageBox msgBox;
            msgBox.setWindowTitle("You've got a match proposal !");
            msgBox.setText(texte);
            msgBox.setInformativeText("Do you accept the match ?");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::Yes);
            if(msgBox.exec() == QMessageBox::Yes){
                confirmation = true;
            }else {
                confirmation = false;
            }
            
            if(confirmation){
                playersInTeam = chooseTeamForMatch(__client, this);
            }
            __client->answerMatchProposal(confirmation, playersInTeam);
            //~ answerMatchProposal(confirmation, playersInTeam); //liste vide = refus de l'invitation
            if(__client->receiveMatchConfirmation() == MATCH_STARTING){
                //~ //startMatch( 2); //invité a l'équipe 2
            }
            break;
        }
        case SERVER_DOWN : {
            badConnection();
            break;
        }
        case MATCH_TOURNAMENT_START : {
            int IDOpponent, numTeam;
            char name[USERNAME_LENGTH];
            char * position = received.stringData;
            QString texte;
            //~ cout << "A tournament turn starts now !" << endl;
            memcpy(&IDOpponent, position, sizeof(IDOpponent));
            position += sizeof(IDOpponent);
            memcpy(&name, position, sizeof(name));
            position += sizeof(name);
            texte = QString("%1, with ID %2 is your opponent !").arg(name, QString::number(IDOpponent));
            QMessageBox msgBox;
            msgBox.setWindowTitle("A tournament match starts now !");
            msgBox.setText(texte);
            msgBox.setInformativeText("You have to accept.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();
            //forced to accept
            std::vector<int> playersInTeam = chooseTeamForMatch(__client, this);
            __client->sendTeamForMatchTournament(playersInTeam);
            //bloquant, l'adversaire doit avoir répondu aussi :
            numTeam = __client->receiveNumOfTeam();
            if(numTeam > 0){ //first to answer is the team 1
                //~ startMatch(numTeam);
            }
            break;
        }
	}
    __pushesNotifier->setEnabled(true);
    ticker->show();
}
int MainGui::badConnection() {
    ticker->hide();
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
    if (role==NORMAL_LOGIN) {
        ticker->hide();
    }
    //this slots acts for both manager and administrator as well :
    //if manager, the user may choose to participate to a tournament
    //if administrator, the user can create a new tournament
    chooseTournament(__client,role,this);
    if(role == NORMAL_LOGIN){
        ticker->show();
    }    
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
        //~ QProgressDialog *progress = new QProgressDialog("Waiting answer from opponent...", QString(), 0, 3, this);
        //~ progress->setWindowModality(Qt::WindowModal);
        //~ progress->show();
        //~ progress->setValue(0);
        //TODO : afficher un message d'attente (le programme se bloque)
        int confirmation = __client->receiveMatchConfirmation();
        //~ progress->setValue(1);
        if(confirmation == MATCH_STARTING){
            //startMatch(1); //inviteur a l'équipe 1
        }else{
            QMessageBox msgBox;
            msgBox.setText("Invitation denied !");
            msgBox.exec();
        }
    }
    ticker->show();
}

void MainGui::buyAPMenu() {
    buyAPDialog = new BuyAPDialog(__client,this);
    buyAPDialog->exec();
}

void MainGui::freeAPMenu() {
    freeAPDialog = new FreeAPDialog(__client,this);
    freeAPDialog->exec();
}

void MainGui::auctionsMenu(){
    if (chooseAuction(__client,this)==BAD_CONNECTION) badConnection();

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
        connect(newPromotionAction,SIGNAL(triggered()),this,SLOT(freeAPMenu()));
        actionPointsMenu->addAction(buyAPAction);
        connect(buyAPAction,SIGNAL(triggered()),this,SLOT(buyAPMenu()));
        ticker = new Ticker(__client, __pushesNotifier, this);
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
