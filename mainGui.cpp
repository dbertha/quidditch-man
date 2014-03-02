#include "mainGui.hpp"
#include "loginDialog.hpp"
#include <QtGui>

MainGui::MainGui(int sockfd,QMainWindow *parent) : sockfd_(sockfd), parent_(parent) {
    setFixedSize(800,480);
// menu minimal - le mgr ne s'est pas encore identifie
    loginAction=new QAction(tr("Login"),this);
    exitAction=new QAction(tr("Exit"),this);
    aboutAction=new QAction(tr("About"),this);
    connect(aboutAction,SIGNAL(triggered()),this,SLOT(about()));
    connect(exitAction,SIGNAL(triggered()),this,SLOT(close()));
    connect(loginAction,SIGNAL(triggered()),this,SLOT(login()));
    fileMenu=menuBar()->addMenu(tr("File"));
    fileMenu->addAction(loginAction);
    fileMenu->addAction(exitAction);
    helpMenu=menuBar()->addMenu(tr("Help"));
//    helpMenu->addAction(helpAction);
    helpMenu->addAction(aboutAction);
    setWindowTitle(tr("Quidditch Manager 2014"));
    dialog = new LoginDialog(this);
    login();
}

MainGui::~ MainGui() {}

void MainGui::run() {
   this->show();
}
void MainGui::quit() {
    std::cout<<"quit"<<std::endl;
}
int MainGui::login() {//le mgr est connecte; on cree le menu
    if (dialog->exec()==dialog->Accepted) {
        std::cout<<"Manager identifie"<<std::endl;
        createMenu();
    }
}
void MainGui::createMenu() {

}
void MainGui::about() {

}
