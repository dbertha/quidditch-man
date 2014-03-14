#ifndef BUYAPDIALOG_H
#define BUYAPDIALOG_H

#include <QDialog>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QRegExp>
#include <QRegExpValidator>
#include <QPushButton>
#include <QErrorMessage>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QString>
#include <QIntValidator>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Client.hpp"
#include "../common/Defines.hpp"

class QCheckBox;
class QLabel;
class QLineEdit;
class QRegExp;
class QPushButton;
class QErrorMessage;
class QMessageBox;
class QIntValidator;

class BuyAPDialog : public QDialog
{
    Q_OBJECT

public:
    BuyAPDialog(Client * client, QWidget *parent = 0);
    void init();
    int getNumberOfAP();


public slots:
    void confirmationClicked();
    void enableConfirmationButton(const QString &text);

private:
    int role;
    Client * __client;
    QString number;
    int numberOfAP;
    QLabel *label;
    QLabel *label2;
    QLineEdit *lineEdit;
    QRegExp *regExp;
    QPushButton *confirmationButton;
    QPushButton *closeButton;
    QErrorMessage *errorMessageDialog;
    QValidator * validator;
};

#endif
