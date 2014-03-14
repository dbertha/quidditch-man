#ifndef SELECTIONSDIALOG_H
#define SELECTIONSDIALOG_H

#include <QDialog>
#include <QStringListModel>
#include <QListView>
#include <QStringList>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

class QStringListModel;
class QListView;

class SelectionDialog : public QDialog {
    Q_OBJECT

public:
    SelectionDialog(const QStringList &items, QWidget *parent = 0);
    int getPosition();

private slots:
    void select();

private:
    QStringListModel *model;
    QListView *listView;

    QPushButton *selectButton;
    QPushButton *cancelButton;
    QHBoxLayout *bottomLayout;
    QVBoxLayout *mainLayout;
    int row;
};

#endif
