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
#include "Client.hpp"
#include "mainGui.hpp"
class MainGui;
class Ticker : public QWidget {
    Q_OBJECT

public:
    Ticker(Client * client, MainGui *parent = 0);

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
};

#endif
