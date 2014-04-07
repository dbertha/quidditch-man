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
#include "ListTournamentsWidget.hpp"

class ListTournamentsWidget;

class TournamentsWidget : public QWidget {
	Q_OBJECT
public:
	TournamentsWidget(Client* client, QWidget* parent);
	void paintEvent(QPaintEvent*);
	void pause();
	void resume();
	void maskLabel();

public slots:
	void create();

public:
	Client* _client;
	QWidget* _parent;

	ListTournamentsWidget* _listTournamentsWidget;
	
	int _price,_nbOfParticipants;
	QLabel* _priceLabel, * _participantsLabel;



};
#endif