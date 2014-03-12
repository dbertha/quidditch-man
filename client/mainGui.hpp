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
#include "playerMgr.hpp"
#include "buildingsDialog.hpp"

class Ticker;
class BuildingsDialog;
class MainGui : public QMainWindow{
    Q_OBJECT
public:
        MainGui(int,QMainWindow *parent=0);
        ~ MainGui();
        void run();
        int getMoney();
        int getNbPlayers();
        int getNbFans();
        int getActionPoints();
        void setMoney(const int);
        void setNbPlayers(const int);
        void setNbFans(const int);
        void setActionPoints(const int);
private slots:
        void about();
        void quit();
        void login();
        void logout();
        void listMgrs();
        void listPlayers();
        void buildings();
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
        QAction *listPlayersAction;
        QAction *buildingsAction;
        QAction *listTournamentsAction;
        QAction *newTournamentAction;
        QAction *newPromotionAction;
        QAction *buyAPAction;
        QMenu *fileMenu;
        QMenu *matchMenu;
        QMenu *auctionMenu;
        QMenu *playersMenu;
        QMenu *buildingsMenu;
        QMenu *tournamentsMenu;
        QMenu *actionPointsMenu;
        QMenu *helpMenu;
        Ticker *ticker;
        BuildingsDialog *buildingsDialog;
        LoginDialog *loginDialog;
        int sockfd_, role, nbPlayers, money, nbFans, nbActionPoints;
        QMainWindow parent_;
};

#endif // MAINGUI_H
