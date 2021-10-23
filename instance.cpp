#include "instance.h"

void InstanceModel::setInstances(const InstanceList &instances) {
    beginResetModel();
    instanceList = instances;
    endResetModel();
}

int InstanceModel::rowCount(const QModelIndex &) const
{
    return instanceList.count();
}

QVariant InstanceModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= instanceList.size())
        return QVariant();

    if (role == Qt::DisplayRole)
        return QString("%1 (%2)").arg(instanceList.at(index.row()).name()).arg(instanceList.at(index.row()).status());
    else
        return QVariant();
}

QVariant InstanceModel::headerData(int section, Qt::Orientation orientation,
                                     int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
        return QStringLiteral("Column %1").arg(section);
    else
        return QStringLiteral("Row %1").arg(section);
}
