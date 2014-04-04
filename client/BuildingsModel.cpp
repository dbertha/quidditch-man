#include "BuildingsModel.hpp"

BuildingsModel::BuildingsModel(QWidget *parent) : QAbstractTableModel(parent) {}

void BuildingsModel::setAxes(const QStringList &hAxe, const QStringList &vAxe) {
    horizontalAxe = hAxe; verticalAxe = vAxe;
    nbRows = verticalAxe.length(); nbColumns = horizontalAxe.length();
}
void BuildingsModel::loadData(QVector<QStringList> *data) {
    beginResetModel();
    loadedData = data;
//    std::cout<<"**buildingsModel"<<std::endl;
    endResetModel();
}
int BuildingsModel::rowCount(const QModelIndex & /* parent */) const {
    return nbRows;
}
int BuildingsModel::columnCount(const QModelIndex & /* parent */) const {
    return nbColumns;
}
QVariant BuildingsModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) return QVariant();
    if (role == Qt::TextAlignmentRole)
        return int(Qt::AlignHCenter | Qt::AlignVCenter);
    else if (role == Qt::DisplayRole) {
//            std::cout<<"**buildingsModel "<<index.row()<<" "<<index.column()<<std::endl;
        return loadedData->at(index.row()).at(index.column());
        }
    return QVariant();
}
QVariant BuildingsModel::headerData(int section, Qt::Orientation direction, int role) const {
    if (role != Qt::DisplayRole) return QVariant();
    if (direction==Qt::Horizontal) return horizontalAxe.at(section);
    else return verticalAxe.at(section);
}
