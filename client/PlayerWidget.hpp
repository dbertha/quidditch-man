#ifndef PLAYERWIDGET_H
#define PLAYERWIDGET_H
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

class PlayerWidget : public QWidget {
	Q_OBJECT
public:
	PlayerWidget(Client* client, QWidget* parent,QString name,int playerID,bool auction=false);
	void paintEvent(QPaintEvent*);
	void pause();
	void resume();
	void updateLabels();
	void setNewID(int playerID);

public:
	Client* _client;
	QWidget* _parent;
	std::vector<int> _infos;
	int _playerID;
	QString _name;
	QLabel* _nameLabel;
	QLabel* _speed;
	QLabel* _strength;
	QLabel* _precision;
	QLabel* _reflex;
	QLabel* _resistance;
	QLabel* _life;
	QLabel* _blocked;
	QLabel* _estimatedValue;
	bool _auction;
	QTimer* _timer;
};
#endif