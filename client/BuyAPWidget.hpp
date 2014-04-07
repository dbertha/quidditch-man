#ifndef BUYAPWIDGET_H
#define BUYAPWIDGET_H
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

class BuyAPWidget : public QWidget {
	Q_OBJECT
public:
	BuyAPWidget(Client* client, QWidget* parent);
	void paintEvent(QPaintEvent*);
	void maskLabel();

public slots:
	void buy();

public:
	Client* _client;
	QWidget* _parent;

	QLineEdit* _lineEdit;
	QLabel* _label;
	QPushButton* _buyButton;
	QLabel* _errorLabel;
};

#endif