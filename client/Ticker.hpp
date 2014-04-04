#ifndef TICKER_HPP
#define TICKER_HPP

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QHBoxLayout>
#include <QPainter>
#include <QPaintEvent>
#include <QTimerEvent>
#include <QShowEvent>
#include <QHideEvent>
#include <QSocketNotifier>
#include <iostream>
#include "Client.hpp"
#include "MainGui.hpp"
class MainGui;
class Ticker : public QWidget {
    Q_OBJECT

public:
    Ticker(Client * client, QSocketNotifier *, MainGui *parent = 0);

protected:
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *event);
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);

private:
    QString myText;
    QLabel *label;
    MainGui *parent_;
    int myTimerId,counter,nbPlayers,money,nbFans,actionPoints;
    void showInfo();
    Client * __client;
    QSocketNotifier * __pushesNotifier;
};

#endif
