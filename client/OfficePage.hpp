#ifndef OFFICEPAGE_H
#define OFFICEPAGE_H
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
#include "BuyAPWidget.hpp"
#include "GainAPWidget.hpp"
#include "ManagePlayerWidget.hpp"
#include "AuctionWidget.hpp"
class InfosWidget;
class BuyAPWidget;
class GainAPWidget;
class ManagePlayerWidget;
class AuctionWidget;

class OfficePage : public QWidget {
	Q_OBJECT
public:
	OfficePage(Client* client,MainWindow* parent);
	void paintEvent(QPaintEvent*);
	void pause();
	void resume();
	void hideStack();
	void showStack();
	void update();
	void blockButtons();
	void deblockButtons();

public slots:
	void listPlayers();
	void listAuctions();
	void buyAP();
	void gainAP();
	void returnMenu();
	

private:
	Client* _client;
	MainWindow* _parent;

	QTimer* _infosUpdate;
	QPushButton* _players;
	QPushButton* _auctions;
	QPushButton* _buyAP;
	QPushButton* _gainAP;
	QPushButton* _returnButton;
	QStackedWidget* _stack;
	InfosWidget* _infos;
	BuyAPWidget* _buyAPWidget;
	GainAPWidget* _gainAPWidget;
	ManagePlayerWidget* _managePlayerWidget;
	AuctionWidget* _auctionWidget;

};

#endif