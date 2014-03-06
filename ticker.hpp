#ifndef TICKER_HPP
#define TICKER_HPP

#include <QWidget>
#include <QString>
#include <QSize>
#include <QPainter>
#include <QPaintEvent>
#include <QTimerEvent>
#include <QShowEvent>
#include <QHideEvent>
#include "common/NetworkInterface.hpp"

class Ticker : public QWidget
{
    Q_OBJECT

public:
    Ticker(const int, QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *event);
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);

private:
    QString myText;
    int sockfd_, offset, myTimerId, money, nbFans, nbPlayers;
};

#endif
