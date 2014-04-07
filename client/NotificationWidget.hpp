#ifndef NOTIFICATIONWIDGET_H
#define NOTIFICATIONWIDGET_H
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


class MainWindow;

class NotificationWidget : public QWidget {
	Q_OBJECT
public:
	NotificationWidget(Client* client, MainWindow* parent=0);
	void paintEvent(QPaintEvent*);
	void tournamentNotification(char name[], int opponentID);
	void friendlyMatchNotification(char name[], int opponentID);
	void answerFriendlyMatch(bool choice);
	void trainingMatchImpossibleNotification();
	void friendlyMatchDeniedNotification();
	void noConnectionNotification();

public slots:
	void acceptFriendlyMatch();
	void refuseFriendlyMatch();
	void startMatch();
	void quit();
	void back();

public:
	Client* _client;
	MainWindow* _parent;

	int _money,_fans,_players,_actionPoints;
	QTimer* _timer;
	QLabel* _label;
	QPushButton* _okButton;
	QPushButton* _noButton;
	QPushButton* _yesButton;
	QPushButton* _quitButton;
	QPushButton* _backButton;
	QGridLayout* grid;

};

#endif