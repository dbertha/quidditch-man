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


class MainMenu;
class ConnexionPage;
class Ticker;
class BuildingsDialog;
class BuyAPDialog;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
	MainWindow(int);

	void displayWindow(QWidget* window);
	void paintEvent(QPaintEvent*);
	void quit();
	void cancel();
	void enterLogin(bool registration=false);
	void connexion(int role);

private:
	QStackedWidget *_stack;
	ConnexionPage* _connexionPage;
	Client* _client;
	int _sockfd;

};
#endif