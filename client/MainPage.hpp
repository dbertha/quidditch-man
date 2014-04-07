#ifndef MAINPAGE_H
#define MAINPAGE_H
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

class InfosWidget;

class MainPage : public QWidget {
	Q_OBJECT
public:
	MainPage(Client* client,MainWindow* parent);
	void paintEvent(QPaintEvent*);
	void pause();
	void resume();
	void blockButtons();
	void deblockButtons();
	
public slots:
	void enterStadium();
	void enterManagerOffice();
	void visitDomain();
	void quit();

private:
	Client* _client;
	MainWindow* _parent;
	QPushButton* _stadium;
	QPushButton* _managerOffice;
	QPushButton* _domain;
	QPushButton* _quit;
	InfosWidget* _infos;
};

#endif