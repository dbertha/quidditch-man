#ifndef INFOSWIDGET_H
#define INFOSWIDGET_H
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

class InfosWidget : public QWidget {
	Q_OBJECT
public:
	InfosWidget(Client* client, QWidget* parent);
	void paintEvent(QPaintEvent*);
	void pause();
	void resume();

public slots:
	void updateLabels();

public:
	Client* _client;
	QWidget* _parent;

	int _money,_fans,_players,_actionPoints;
	QTimer* _timer;
	QLabel* _moneyLabel;
	QLabel* _fansLabel;
	QLabel* _actionPointsLabel;
	QLabel* _moneyIcon;
	QLabel* _fansIcon;
	QLabel* _actionPointsIcon;
};

#endif