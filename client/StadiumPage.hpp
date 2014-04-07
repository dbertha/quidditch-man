#ifndef STADIUMPAGE_H
#define STADIUMPAGE_H
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
#include "InfosWidget.hpp"
#include "TournamentsWidget.hpp"
	
class InfosWidget;
class TournamentsWidget;

class StadiumPage : public QWidget {
	Q_OBJECT
public:
	StadiumPage(Client* client,MainWindow* parent);
	void paintEvent(QPaintEvent*);
	void pause();
	void hide();
	void resume();
	void blockButtons();
	void deblockButtons();

public slots:
	void friendlyMatch();
	void tournaments();
	void trainingMatch();
	void returnMenu();
	

private:
	Client* _client;
	MainWindow* _parent;

	QTimer* _infosUpdate;
	QPushButton* _friendlyMatch;
	QPushButton* _tournaments;
	QPushButton* _trainingMatch;
	QPushButton* _returnButton;
	QStackedWidget* _stack;
	
	InfosWidget* _infos;
	TournamentsWidget* _tournamentsWidget;
};

#endif