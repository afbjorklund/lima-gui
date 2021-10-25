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
    return 3;
}

QVariant InstanceModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= instanceList.size())
        return QVariant();
    if (index.column() >= 3)
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        Instance instance = instanceList.at(index.row());
        switch (index.column()) {
            case 0:
                return instance.name();
            case 1:
                return instance.status();
            case 2:
                return instance.arch();
        }
    }
    return QVariant();
}

QVariant InstanceModel::headerData(int section, Qt::Orientation orientation,
                                     int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
            case 0:
                return tr("Name");
            case 1:
                return tr("Status");
            case 2:
                return tr("Arch");
        }
    }
    return QVariant(); // QStringLiteral("Row %1").arg(section);
}
