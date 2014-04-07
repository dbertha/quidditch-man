#ifndef MANAGEPLAYERWIDGET_H
#define MANAGEPLAYERWIDGET_H
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
#include "ListPlayersWidget.hpp"

class ListPlayersWidget;

class ManagePlayerWidget : public QWidget {
	Q_OBJECT
public:
	ManagePlayerWidget(Client* client, QWidget* parent);
	void paintEvent(QPaintEvent*);
	void pause();
	void resume();
	void maskLabel();
	void updateLabels();

public slots:
	void train();
	void heal();
	void sell();

public:
	Client* _client;
	QWidget* _parent;

	ListPlayersWidget* _listPlayersWidget;
	QListWidget* _capacity;
	QPushButton* _trainButton;
	QPushButton* _healButton;
	QPushButton* _sellButton;
	QLineEdit* _priceToSell;
	QHBoxLayout* _zone;
	QStackedWidget* _stack;
	QLabel* _errorLabel;
};
#endif