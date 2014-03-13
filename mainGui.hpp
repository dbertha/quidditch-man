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
#include "tournaments.hpp"
#include "ticker.hpp"
#include "loginDialog.hpp"
#include "mainMenu.hpp"
#include "buildingsDialog.hpp"
class MainMenu;
class Ticker;
class BuildingsDialog;

class MainGui : public QMainWindow {
    Q_OBJECT
public:
        MainGui(int,QMainWindow *parent=0);
        ~MainGui();
        int getMoney();
        int getNbPlayers();
        int getNbFans();
        int getActionPoints();
        void setMoney(const int);
        void setNbPlayers(const int);
        void setNbFans(const int);
        void setActionPoints(const int);
public slots:
        void listMgrs();
        void listPlayers();
        void buildings();
        void tournaments();
private slots:
        void about();
        void login();
        void logout();
private:
        int badConnection();
        void createActions();
        void firstMenu();
        void createMenu();
        void createButtons();
        QAction *loginAction;
        QAction *logoutAction;
        QAction *exitAction;
        QAction *aboutAction;
        QAction *listMgrsAction;
        QAction *listAuctionsAction;
        QAction *listPlayersAction;
        QAction *buildingsAction;
        QAction *listTournamentsAction;
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
        MainMenu *mainMenu;
        BuildingsDialog *buildingsDialog;
        LoginDialog *loginDialog;
        int sockfd_, role, nbPlayers, money, nbFans, nbActionPoints;
        QMainWindow *parent_;
};

#endif
