#ifndef LISTPLAYERSWIDGET_H
#define LISTPLAYERSWIDGET_H
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
#include "PlayerWidget.hpp"

class PlayerWidget;

class ListPlayersWidget : public QWidget {
	Q_OBJECT
public:
	ListPlayersWidget(Client* client, QWidget* parent);
	void paintEvent(QPaintEvent*);
	void pause();
	void resume();
	int getCurrentPlayer();

public slots:
	void displayPlayer();
	void update();

public:
	Client* _client;
	QWidget* _parent;

	QListWidget* _listPlayers;
	QStackedWidget* _stack;
	std::vector<std::string> _playersName;
	std::vector<PlayerWidget*> _players;
	QTimer* _timer;
	QLabel* _noPlayerLabel;
};
#endif