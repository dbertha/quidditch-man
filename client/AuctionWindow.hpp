#ifndef AUCTIONWINDOWS_HPP
#define AUCTIONWINDOWS_HPP

#include "../common/Defines.hpp"
#include "Client.hpp"

#include <QDialog>
#include <QTableView>
#include <QItemSelectionModel>
#include <QLabel>
#include <QVector>
#include <QStringList>
#include <QString>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QInputDialog>
#include <QErrorMessage>
#include <iostream>
#include <QThread>
#include <QTimer>

class AuctionWindow : public QDialog {
	Q_OBJECT
public:
    AuctionWindow(Client *, int, QWidget *parent = 0);

public slots:
	void bid();
	void endOfTurn();
	void changeLabel();
	void init();


private:
    QHBoxLayout *bottomLayout;
    QVBoxLayout *mainLayout;
    QPushButton *bidButton;
    QPushButton *quitButton;
    QLabel *price, *bidders;
    Client * __client;
    int startingPrice_;
    int turnPrice_;
    int currentPrice_;
    int currentTurn_;
    int timeLeft_;
    int nbOfBidders_;
    bool hasBidden_;
    QThread thread;
    QTimer *priceUpdate;
};


#endif