#ifndef LISTAUCTIONSWIDGET_H
#define LISTAUCTIONSWIDGET_H
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

class ListAuctionsWidget : public QWidget {
	Q_OBJECT
public:
	ListAuctionsWidget(Client* client, QWidget* parent);
	void paintEvent(QPaintEvent*);
	void pause();
	void resume();
	int getAuctionID();
	int getAuctionStartingPrice();
	void hide();
	void show();

public slots:
	void displayAuction();
	void update();

public:
	Client* _client;
	QWidget* _parent;

	QListWidget* _listAuctions;
	QStackedWidget* _stack;
	QLabel* _noAuctionLabel;
	std::vector<std::string> _auctionsTitle;
	std::vector<PlayerWidget*> _auctions;
	std::vector<int> _auctionsID;
	std::vector<int> _auctionsStartingPrice;
	QTimer* _timer;
};
#endif