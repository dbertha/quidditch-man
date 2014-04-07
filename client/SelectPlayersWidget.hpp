#ifndef SELECTPLAYERSWIDGET_H
#define SELECTPLAYERSWIDGET_H
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

class MainWindow;
class ListPlayersWidget;

class SelectPlayersWidget : public QWidget {
	Q_OBJECT
public:
	SelectPlayersWidget(Client* client, MainWindow* parent);
	void paintEvent(QPaintEvent*);
	void pause();
	void resume();
	void hideButtons();
	void maskLabel();
	void init(bool value,int opponentID);

    //orderedRoles << "KEEPER" << "SEEKER" << "CHASER1" << "CHASER2" << "CHASER3" << "BEATER1" << "BEATER2";

public slots:
	void updateListManagers();
	void friendlyMatch();
	void choosePlayer();
	void activateButton();
	void confirm();
	void cancel();

public:
	Client* _client;
	MainWindow* _parent;
	bool _ask;
	std::vector<int> _team;
	int _opponentID;

	ListPlayersWidget* _listPlayersWidget;
	QLabel* _noManagerLabel;

	std::vector<int> _IDList;
	std::vector<std::string> _namesList;

	QLabel* _errorLabel;
	QListWidget* _listManagers;

	QTimer* _timer;

	QPushButton* _friendlyMatch;
	QPushButton* _confirmButton;
	QPushButton* _cancelButton;
	std::vector<QString> _rolesName;
	std::vector<QPushButton *> _roles;
};
#endif