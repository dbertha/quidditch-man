#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QErrorMessage;

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    LoginDialog(QWidget *parent = 0);

signals:
    void login(const QString &,const QString &);
    void registrate(const QString &,const QString &);

private slots:
    void loginClicked();
    void enableloginButton(const QString &text);

private:
    QLabel *label;
    QLineEdit *lineEdit;
    QCheckBox *newUserCheckBox;
    QPushButton *loginButton;
    QPushButton *closeButton;
    QString userName, password1, password2;
    QErrorMessage *errorMessageDialog;
};

#endif
