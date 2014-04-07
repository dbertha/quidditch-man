#ifndef CONNEXIONPAGE_H
#define CONNEXIONPAGE_H

#include <QDialog>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QRegExp>
#include <QRegExpValidator>
#include <QPushButton>
#include <QErrorMessage>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QString>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Client.hpp"
#include "MainWindow.hpp"

class MainWindow;
class QCheckBox;
class QLabel;
class QLineEdit;
class QRegExp;
class QPushButton;
class QErrorMessage;

class ConnexionPage : public QWidget
{
    Q_OBJECT

public:
    ConnexionPage(Client * client, MainWindow *parent = 0);
    void paintEvent(QPaintEvent*);

public slots :
    void quit();
    void login();
    void newGame();

private:
    Client * _client;
    MainWindow* _parent;

    QPushButton *_loginButton;
    QPushButton *_registerButton;
    QPushButton *_closeButton;
};

#endif
