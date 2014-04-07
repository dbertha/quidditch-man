#ifndef GAINAPWIDGET_H
#define GAINAPWIDGET_H
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

class GainAPWidget : public QWidget {
	Q_OBJECT
public:
	GainAPWidget(Client* client, QWidget* parent);
	void paintEvent(QPaintEvent*);
	void maskLabel();

public slots:
	void startCampaign();
	void stopCampaign();
	void updateLabel();
public:
	Client* _client;
	QWidget* _parent;

	QPushButton* _startButton;
    QPushButton* _stopButton;
    QLabel* _ratio;
    QLabel* _result;
    QLabel* _timeLabel;
    int _time;
    bool _campaigning;
    QTimer* _timer;
};

#endif