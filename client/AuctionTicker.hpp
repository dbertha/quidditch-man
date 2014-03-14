#ifndef AUCTIONTICKER_HPP
#define AUCTIONTICKER_HPP

#include "../common/Defines.hpp"
#include "Client.hpp"
#include "auctionWindow.hpp"

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

class AuctionTicker : public QWidget {
    Q_OBJECT

public:
    AuctionTicker(Client * client, int, QSocketNotifier *, MainGui *parent = 0);

protected:
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *event);
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);

private:
    QString myText;
    QLabel *label;
    MainGui *parent_;
    int currentPrice;
    void showInfo();
    Client * __client;
    QSocketNotifier * __pushesNotifier;
};

#endif