#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

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

#include "common/NetworkInterface.hpp"

class QCheckBox;
class QLabel;
class QLineEdit;
class QRegExp;
class QPushButton;
class QErrorMessage;

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    LoginDialog(int, QWidget *parent = 0);
    void init();
    QString getName();

signals:
    void login(const QString &,const QString &);
    void registrate(const QString &,const QString &);

private slots:
    void loginClicked();
    void enableloginButton(const QString &text);

private:
    int sockfd_;
    char username[USERNAME_LENGTH];
    char password[PASSWORD_LENGTH];
    QLabel *label;
    QLineEdit *lineEdit;
    QRegExp *regExp;
    QCheckBox *newUserCheckBox;
    QPushButton *loginButton;
    QPushButton *closeButton;
    QString userName, password1;
    QErrorMessage *errorMessageDialog;
};

#endif
