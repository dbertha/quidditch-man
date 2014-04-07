#ifndef MAINWINDOW_H
#define MAINWINDOW_H
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
#include <QTimer>
#include <QLabel>
#include <QApplication>
#include <QtGui>
#include "Client.hpp"

#include "Tournaments.hpp"
#include "Ticker.hpp"
#include "LoginDialog.hpp"
#include "MainMenu.hpp"
#include "BuildingsDialog.hpp"
#include "BuyAPDialog.hpp"
#include "FreeAPDialog.hpp"
#include "MatchWindow.hpp"
#include "MainGui.hpp"

#include "ConnexionPage.hpp"
#include "LoginPage.hpp"
#include "MainPage.hpp"
#include "StadiumPage.hpp"
#include "OfficePage.hpp"
#include "DomainPage.hpp"
#include "AdminPage.hpp"
#include "InfosWidget.hpp"
#include "NotificationWidget.hpp"

class MainMenu;
class ConnexionPage;
class MainPage;
class Ticker;
class BuildingsDialog;
class BuyAPDialog;
class StadiumPage;
class OfficePage;
class DomainPage;
class AdminPage;
class InfosWidget;
class NotificationWidget;

class MainWindow : public QDialog {
    Q_OBJECT
public:
	MainWindow(int);

	void displayWindow(QWidget* window);
	void paintEvent(QPaintEvent*);
	void quit();
	void cancel();
	void enterLogin(bool registration=false);
	void connexion(int role);
	void mainPage();
	void stadiumPage();
	void officePage();
	void domainPage();
	void block();
	void deblock();

	void friendlyMatch();
	void trainingMatch();

	int badConnection();

public slots:
    void pushesHandler();
	void pause();
	void resume();

private:
	QStackedWidget *_stack;
	ConnexionPage* _connexionPage;
	MainPage* _mainPage;
	StadiumPage* _stadiumPage;
	OfficePage* _officePage;
	DomainPage* _domainPage;
	AdminPage* _adminPage;
	Client* _client;
	int _sockfd;
    QSocketNotifier * __pushesNotifier;
    QTimer* _timerPause;
    QTimer* _timerResume;
    InfosWidget* _infos;
    NotificationWidget* _notification;
    QGridLayout* grid;

};
#endif