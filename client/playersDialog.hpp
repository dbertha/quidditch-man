#ifndef PLAYERSDIALOG_HPP
#define PLAYERSDIALOG_HPP
#define TRAIN 0
#define HEAL 1
#define SELL 2
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
#include "Client.hpp"
#include "buildingsModel.hpp"

class PlayersDialog : public QDialog {
    Q_OBJECT
public:
    PlayersDialog(Client *, const int, const int, QWidget *parent = 0);

public slots:
    void train();
    void heal();
    void sell();

private:
    void getInfo();
    void select(const int);
    void confirmation();
    void badConnection();
    QPushButton *trainButton;
    QPushButton *healButton;
    QPushButton *sellButton;
    QPushButton *broomstickButton;
    QPushButton *cancelButton;
    QPushButton *OKButton;
    QPushButton *NOButton;
    QHBoxLayout *bottomLayout;
    QVBoxLayout *mainLayout;
    QTableView *tableView;
    BuildingsModel *buildingsModel;
    QItemSelectionModel *selectionModel;
    QLabel *blockedLabel, *lifeValueLabel;
    QVector<QStringList> loadedData;
    QStringList hAxe, vAxe, strActions;
    std::vector<int> playerInfos;
    int player_,nbPlayers,money,nbFans,actionPoints;
    Client * __client;
};

#endif
