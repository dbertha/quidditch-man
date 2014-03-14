#ifndef AUCTIONSDIALOG_HPP
#define AUCTIONSDIALOG_HPP
#define JOIN 0
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
#include <QSocketNotifier>
#include "Client.hpp"
#include "buildingsModel.hpp"

#include "auctionWindow.hpp"

class AuctionsDialog : public QDialog {
    Q_OBJECT
public:
    AuctionsDialog(Client *, int,int, QWidget *parent = 0);

public slots:
    void join();

private:
    void getInfo();
    void badConnection();
    QPushButton *joinButton;
    QPushButton *cancelButton;
    QHBoxLayout *bottomLayout;
    QVBoxLayout *mainLayout;
    QTableView *tableView;
    BuildingsModel *buildingsModel;
    QItemSelectionModel *selectionModel;
    QLabel *blockedLabel, *lifeValueLabel;
    QVector<QStringList> loadedData;
    QStringList hAxe, vAxe, strActions;
    std::vector<int> playerInfos;
    int player_,nbPlayers,money,nbFans,actionPoints,auctionID_,startingPrice_;
    Client * __client;
    QSocketNotifier * __notifier;
    AuctionWindow* auctionWindow;
};

#endif
