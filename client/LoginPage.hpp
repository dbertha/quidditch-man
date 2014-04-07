#ifndef LOGINPAGE_H
#define LOGINPAGE_H

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

class LoginPage : public QWidget
{
    Q_OBJECT

public:
    LoginPage(Client * client, MainWindow *parent = 0,bool registration=false);
    void paintEvent(QPaintEvent*);
    void makeErrorLabel(std::string text);

public slots:
	void confirm();
	void cancel();

private:
    Client * _client;
    MainWindow* _parent;

    char _username[USERNAME_LENGTH];
    char _password[PASSWORD_LENGTH];
    bool _registration;
    QLabel* _errorLabel;
    QLineEdit* _loginZone;
    QLineEdit* _passwordZone;
    QLineEdit* _passwordZone2;
    QPushButton* _confirmButton;
    QPushButton* _cancelButton;
};

#endif
