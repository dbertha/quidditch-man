#ifndef BUILDINGSDIALOG_HPP
#define BUILDINGSDIALOG_HPP

#include <QDialog>
#include <QTableView>
#include <QItemSelectionModel>
#include <QVector>
#include <QStringList>
#include <QString>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPainter>
#include <QPaintEvent>
#include <QTimerEvent>
#include <QShowEvent>
#include <QHideEvent>
#include "common/NetworkInterface.hpp"
#include "buildingsModel.hpp"
#include "mainGui.hpp"
class MainGui;
class BuildingsDialog : public QDialog {
    Q_OBJECT
public:
    BuildingsDialog(const int, MainGui *parent = 0);
    
private slots:
    void select();

protected:
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *event);
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);

private:
    void showInfo();

    QPushButton *selectButton;
    QPushButton *cancelButton;
    QHBoxLayout *bottomLayout;
    QVBoxLayout *mainLayout;
    QTableView *tableView;
    QString str;
    BuildingsModel *buildingsModel;
    QItemSelectionModel *selectionModel;
    QVector<QStringList> loadedData;
    QStringList hAxe, vAxe;
    std::vector<int> buildingInfos;
    int sockfd_, row, myTimerId, i;
    MainGui *parent_;
};

#endif
