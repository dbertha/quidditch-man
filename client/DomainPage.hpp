#ifndef DOMAINPAGE_H
#define DOMAINPAGE_H
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
#include "InfosWidget.hpp"
#include "BuildingWidget.hpp"
	
class InfosWidget;
class BuildingWidget;

class DomainPage : public QWidget {
	Q_OBJECT
public:
	DomainPage(Client* client,MainWindow* parent);
	void paintEvent(QPaintEvent*);
	void pause();
	void resume();
	void hideStack();
	void update();
	void blockButtons();
	void deblockButtons();

public slots:
	void stadium();
	void trainingCenter();
	void hospital();
	void fanShop();
	void promotionCenter();
	void returnMenu();
	

private:
	Client* _client;
	MainWindow* _parent;

	QTimer* _infosUpdate;
	QPushButton* _stadium;
	QPushButton* _trainingCenter;
	QPushButton* _hospital;
	QPushButton* _fanShop;
	QPushButton* _promotionCenter;
	QPushButton* _returnButton;
	QGridLayout* _grid;
	QStackedWidget* _stack;
	InfosWidget* _infos;
	std::vector<BuildingWidget*> _buildingWidgets;
};

#endif