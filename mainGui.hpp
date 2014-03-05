#ifndef MAINGUI_H
#define MAINGUI_H
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <QMainWindow>
#include <QAction>
#include <QMenuBar>
#include <QMenu>
#include <QMessageBox>
#include <QStringList>
#include "ticker.hpp"
#include "loginDialog.hpp"
#include "clientMatchHandler.hpp"

class MainGui : public QMainWindow{
    Q_OBJECT
public:
        MainGui(int,QMainWindow *parent=0);
        ~ MainGui();
        void run();
private slots:
        void about();
        void quit();
        void login();
        void logout();
        void listMgrs();
private:
        int badConnection();
        void createActions();
        void firstMenu();
        void createMenu();
        QAction *loginAction;
        QAction *logoutAction;
        QAction *exitAction;
        QAction *aboutAction;
        QAction *listMgrsAction;
        QAction *listAuctionsAction;
        QAction *newAuctionAction;
        QAction *listPlayersAction;
        QAction *enterStadiumAction;
        QAction *enterTrainingCenterAction;
        QAction *enterHospitalAction;
        QAction *enterFanShopAction;
        QMenu *fileMenu;
        QMenu *matchMenu;
        QMenu *auctionMenu;
        QMenu *playersMenu;
        QMenu *buildingsMenu;
        QMenu *helpMenu;
        Ticker *ticker;
        LoginDialog *loginDialog;
        int sockfd_;
        QMainWindow parent_;
};

#endif // MAINGUI_H
