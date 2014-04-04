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
#include <QProgressDialog>
#include <QSocketNotifier>
#include "Client.hpp"
#include "Tournaments.hpp"
#include "Ticker.hpp"
#include "LoginDialog.hpp"
#include "MainMenu.hpp"
#include "BuildingsDialog.hpp"
#include "BuyAPDialog.hpp"
#include "FreeAPDialog.hpp"
#include "MatchWindow.hpp"



class MainMenu;
class Ticker;
class BuildingsDialog;
class BuyAPDialog;

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
        void pushesHandler();
        void buyAPMenu();
        void freeAPMenu();
        void auctionsMenu();
        void trainingMatchHandler();
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
        BuyAPDialog *buyAPDialog;
        FreeAPDialog *freeAPDialog;
        LoginDialog *loginDialog;
        int role, nbPlayers, money, nbFans, nbActionPoints;
        QMainWindow parent_;
        Client * __client;
        QSocketNotifier * __pushesNotifier;

};

#endif
