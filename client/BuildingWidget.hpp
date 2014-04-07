#ifndef BUILDINGWIDGET_H
#define BUILDINGWIDGET_H
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

class BuildingWidget : public QWidget {
	Q_OBJECT
public:
	BuildingWidget(Client* client, QWidget* parent,int buildingID);
	void paintEvent(QPaintEvent*);
	void pause();
	void resume();
	void maskLabel();

public slots:
	void updateLabels();
	void upgrade();

public:
	Client* _client;
	QWidget* _parent;
	int _buildingID;
	std::vector<int> _infos;
	QTimer* _timer;
	QLabel* _name;
	QLabel* _levelLabel;
	QLabel* _priceLabel;
	QLabel* _attributeLabel;
	QLabel* _isUpgradingLabel;
	QLabel* _errorLabel;
	std::vector<std::string> _names;
	QPushButton* _upgradeButton;
	int _upgrading;
};

#endif