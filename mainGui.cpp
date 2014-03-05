#include "mainGui.hpp"

MainGui::MainGui(int sockfd,QMainWindow *parent) : sockfd_(sockfd), parent_(parent) {
    setFixedSize(1200,600);
    createActions();
    firstMenu();
    setWindowTitle(tr("Quidditch Manager 2014"));
    loginDialog = new LoginDialog(sockfd_,this);
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

void MainGui::listMgrs() {
    int res = choosePartner(sockfd_,this);
    if (res==BAD_CONNECTION) badConnection();
    else {

    }
}
void MainGui::login() {
    loginDialog->init();
    if (loginDialog->exec()==loginDialog->Accepted) {//le mgr est connecte; on cree le menu
        std::cout<<"Manager identifie"<<std::endl;
        createMenu();
    }
}
void MainGui::logout() {
    menuBar()->clear();
    firstMenu();
    loginDialog->init(); //reset userName
}
void MainGui::createActions() {
    loginAction=new QAction(tr("Login"),this);
    exitAction=new QAction(tr("Exit"),this);
    aboutAction=new QAction(tr("About"),this);
    logoutAction=new QAction(tr("Logout"),this);
    listMgrsAction=new QAction(tr("List available managers"),this);
    listAuctionsAction=new QAction(tr("List auctions"),this);
    newAuctionAction=new QAction(tr("New"),this);
    listPlayersAction=new QAction(tr("List my players"),this);
    enterStadiumAction=new QAction(tr("Enter Stadium"),this);
    enterTrainingCenterAction=new QAction(tr("Enter Training Center"),this);
    enterHospitalAction=new QAction(tr("Enter Hospital"),this);
    enterFanShopAction=new QAction(tr("Enter FanShop"),this);
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
        fileMenu->addAction(logoutAction);
        connect(logoutAction,SIGNAL(triggered()),this,SLOT(logout()));
        matchMenu=menuBar()->addMenu(tr("Match"));
        matchMenu->addAction(listMgrsAction);
        connect(listMgrsAction,SIGNAL(triggered()),this,SLOT(listMgrs()));
        auctionMenu=menuBar()->addMenu(tr("Auctions"));
        auctionMenu->addAction(newAuctionAction);
        auctionMenu->addAction(listAuctionsAction);
        playersMenu=menuBar()->addMenu(tr("Players"));
        playersMenu->addAction(listPlayersAction);
        buildingsMenu=menuBar()->addMenu(tr("Buildings"));
        buildingsMenu->addAction(enterStadiumAction);
        buildingsMenu->addAction(enterTrainingCenterAction);
        buildingsMenu->addAction(enterHospitalAction);
        buildingsMenu->addAction(enterFanShopAction);
}
void MainGui::about() {
    QMessageBox::about(this, tr("Quidditch Manager 2014"),
               "<p>MANON LEGRAND"
               "<p>HELENE PLISNIER"
               "<p>DAVID BERTHA"
               "<p>AUDRY DELESTREE");
}
