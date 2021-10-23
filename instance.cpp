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

int InstanceModel::columnCount(const QModelIndex &) const
{
    return 2;
}

QVariant InstanceModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= instanceList.size())
        return QVariant();
    if (index.column() >= 2)
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        Instance instance = instanceList.at(index.row());
        if (index.column() == 0)
            return instance.name();
        else if (index.column() == 1)
            return instance.status();
    }
    return QVariant();
}

QVariant InstanceModel::headerData(int section, Qt::Orientation orientation,
                                     int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        if (section == 0)
            return tr("Name");
        else if (section == 1)
            return tr("Status");
    }
    return QVariant(); // QStringLiteral("Row %1").arg(section);
}
