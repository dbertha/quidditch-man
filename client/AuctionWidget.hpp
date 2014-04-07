#ifndef AUCTIONWIDGET_H
#define AUCTIONWIDGET_H
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
#include "ListAuctionsWidget.hpp"

class ListAuctionsWidget;

class AuctionWidget : public QWidget {
	Q_OBJECT
public:
	AuctionWidget(Client* client, QWidget* parent);
	void paintEvent(QPaintEvent*);
	void pause();
	void resume();
	void maskLabel();
	void startTurn();
	void endOfTurn();
	void makeJoinable();

public slots:
	void init();
	void join();
	void bid();
	void quit();
	void prepareForNextTurn();
	void prepareForEndOfTurn();
	void changeLabel();

public:
	Client* _client;
	QWidget* _parent;

	ListAuctionsWidget* _listAuctionsWidget;
	
	QTimer* _endOfTurnTimer,*_nextTurnTimer,*_updater;
	int _startingPrice,_turnPrice,_timeBeforeEndOfTurn,_timeBeforeTurn,_timeLeft,_currentTurn;
	bool _hasBidden,_hasLeft;
	int _currentPrice,_nbOfBidders,_auctionID;
	QPushButton* _bidButton;
	QPushButton* _quitButton;
	QPushButton* _joinButton;
	QLabel* _priceLabel;
	QLabel* _biddersLabel;
	QHBoxLayout* _zone;
	QStackedWidget* _stack;
	QLabel* _errorLabel;
};
#endif