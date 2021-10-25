#include "instance.h"

#include <QStringList>

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
    return 6;
}

static QStringList binaryAbbrs = {"B", "KiB", "MiB", "GiB", "TiB", "PiB", "EiB", "ZiB", "YiB"};

// BytesSize returns a human-readable size in bytes, kibibytes,
// mebibytes, gibibytes, or tebibytes (eg. "44kiB", "17MiB").
static QString bytesSize(qint64 size) {
    qint64 base = 1024;
    int i, unitsLimit = binaryAbbrs.size();
    for (i= 0; size >= base && i < unitsLimit; i++) {
        size /= base;
    }
    return QString("%1 %2").arg(size, 4).arg(binaryAbbrs[i]);
}

QVariant InstanceModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= instanceList.size())
        return QVariant();
    if (index.column() >= 6)
        return QVariant();

    if (role == Qt::TextAlignmentRole)
    {
        switch (index.column()) {
            case 0:
                return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
            case 1:
                return QVariant(Qt::AlignRight | Qt::AlignVCenter);
            case 2:
                // fall-through
            case 3:
                // fall-through
            case 4:
                // fall-through
            case 5:
                return QVariant(Qt::AlignHCenter | Qt::AlignVCenter);
        }
    }
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
            case 3:
                return QString::number(instance.cpus());
            case 4:
                return bytesSize(instance.memory());
            case 5:
                return bytesSize(instance.disk());
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
            case 3:
                return tr("CPUs");
            case 4:
                return tr("Mem.");
            case 5:
                return tr("Disk");
        }
    }
    return QVariant(); // QStringLiteral("Row %1").arg(section);
}
