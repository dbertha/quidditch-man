#ifndef ADMINPAGE_H
#define ADMINPAGE_H
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
#include "MainWindow.hpp"
#include "TournamentsWidget.hpp"

class TournamentsWidget;

class AdminPage : public QWidget {
	Q_OBJECT
public:
	AdminPage(Client* client,MainWindow* parent);
	void paintEvent(QPaintEvent*);

public slots:
	void listTournaments();
	void createTournament();
	void quit();

private:
	Client* _client;
	MainWindow* _parent;
	QPushButton* _listTournaments;
	QPushButton* _quitButton;
	QLabel* _adminLabel;
	QGridLayout* _grid;
	QStackedWidget* _stack;
	TournamentsWidget* _tournamentsWidget;
};

#endif