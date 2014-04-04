#ifndef FREEAPDIALOG_H
#define FREEAPDIALOG_H

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

class FreeAPDialog : public QDialog
{
    Q_OBJECT

public:
    FreeAPDialog(Client * client, QWidget *parent = 0);
    void init();

public slots:
    void stopCampaign();

private:
    Client * __client;
    QLabel *label;
    QLabel *label2;
    QPushButton *closeButton;
    QErrorMessage *errorMessageDialog;
};

#endif
