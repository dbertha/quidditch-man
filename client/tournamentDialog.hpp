#ifndef TOURNAMENTDIALOG_HPP
#define TOURNAMENTDIALOG_HPP

#include <QDialog>
#include <QStringListModel>
#include <QListView>
#include <QStringList>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QString>
#include <QMessageBox>
#include <QInputDialog>
#include "common/Defines.hpp"
#include "Client.hpp"
class QStringListModel;
class QListView;

class TournamentDialog : public QDialog {
    Q_OBJECT
public:
    TournamentDialog(Client *, const QStringList &items,const int,QWidget *parent = 0);
    int getPosition();

private slots:
    void select();
private:
    QStringListModel *model;
    QListView *listView;
    QPushButton *selectButton;
    QPushButton *cancelButton;
    QVBoxLayout *mainLayout;
    QHBoxLayout *bottomLayout;
    int row, role_;
    Client * __client;
    void technicalProblem();
};

#endif
