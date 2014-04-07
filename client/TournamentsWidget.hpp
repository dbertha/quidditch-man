#ifndef TOURNAMENTSWIDGET_H
#define TOURNAMENTSWIDGET_H
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
#include <vector>
#include "Client.hpp"
#include "MainWindow.hpp"

class TournamentsWidget : public QWidget {
	Q_OBJECT
public:
	TournamentsWidget(Client* client, QWidget* parent, int role);
	void paintEvent(QPaintEvent*);
	void pause();
	void resume();
	void maskLabel();

public slots:
	void displayTournament();
	void create();
	void join();
	void updateLabels();

public:
	Client* _client;
	QWidget* _parent;

	int _role;
	QListWidget* _listTournaments;

	QLabel* _currentParticipants;
	QLabel* _startingPrice;
	QLineEdit* _price, *_participants;
	QTimer* _timer;
	QLabel* _label;
	QPushButton* _create;
	QPushButton* _join;

};
#endif