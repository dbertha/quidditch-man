#ifndef BUILDINGSMODEL_HPP
#define BUILDINGSMODEL_HPP

#include <QAbstractTableModel>
#include <QWidget>
#include <QStringList>
#include <QVector>
#include <QVariant>
#include <QModelIndex>

class BuildingsModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    BuildingsModel(QWidget *parent = 0);
    void setAxes(const QStringList &, const QStringList &);
    void loadData(QVector<QStringList> *);
    int rowCount(const QModelIndex &) const;
    int columnCount(const QModelIndex &) const;
    QVariant data(const QModelIndex &index, int) const;
    QVariant headerData(int, Qt::Orientation, int) const;
private:
    QStringList verticalAxe, horizontalAxe;
    QVector<QStringList> *loadedData;
    int nbRows, nbColumns;
};
#endif // BUILDINGSMODEL_HPP
