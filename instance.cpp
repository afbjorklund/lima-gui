#include "instance.h"

#include <QStringList>
#include <QJsonObject>

Instance::Instance(const QJsonObject &obj) : Instance()
{
    QString name;
    if (obj.contains("name")) {
        name = obj["name"].toString();
    }
    if (name.isEmpty()) {
        return;
    }
    m_name = name;

    if (obj.contains("status")) {
        QString status = obj["status"].toString();
        setStatus(status);
    }
    if (obj.contains("dir")) {
        QString dir = obj["dir"].toString();
        setDir(dir);
    }
    if (obj.contains("vmType")) {
        QString type = obj["vmType"].toString();
        setVmType(type);
    } else {
        setVmType(QString());
    }
    if (obj.contains("arch")) {
        QString arch = obj["arch"].toString();
        setArch(arch);
    }
    if (obj.contains("cpuType")) {
        QString type = obj["cpuType"].toString();
        setCpuType(type);
    } else {
        setCpuType(QString());
    }
    if (obj.contains("cpus")) {
        int cpus = obj["cpus"].toInt();
        setCpus(cpus);
    }
    if (obj.contains("memory")) {
        double memory = obj["memory"].toDouble();
        setMemory((qint64)memory);
    }
    if (obj.contains("disk")) {
        double disk = obj["disk"].toDouble();
        setDisk((qint64)disk);
    }
    if (obj.contains("sshLocalPort")) {
        int port = obj["sshLocalPort"].toInt();
        setSshLocalPort(port);
    } else {
        setSshLocalPort(0);
    }
    if (obj.contains("sshConfigFile")) {
        QString file = obj["sshConfigFile"].toString();
        setSshConfigFile(file);
    } else {
        setSshConfigFile("");
    }
}

void InstanceModel::setInstances(const InstanceList &instances)
{
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
    return 7;
}

static QStringList binaryAbbrs = { "B", "KiB", "MiB", "GiB", "TiB", "PiB", "EiB", "ZiB", "YiB" };

// BytesSize returns a human-readable size in bytes, kibibytes,
// mebibytes, gibibytes, or tebibytes (eg. "44kiB", "17MiB").
static QString bytesSize(qint64 size)
{
    qint64 base = 1024;
    int i, unitsLimit = binaryAbbrs.size();
    for (i = 0; size >= base && i < unitsLimit; i++) {
        size /= base;
    }
    return QString("%1 %2").arg(size, 4).arg(binaryAbbrs[i]);
}

QString Instance::strCpus() const
{
    return QString::number(cpus());
}

QString Instance::strMemory() const
{
    return bytesSize(memory());
}

QString Instance::strDisk() const
{
    return bytesSize(disk());
}

QVariant InstanceModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= rowCount(index))
        return QVariant();
    if (index.column() >= columnCount(index))
        return QVariant();

    Qt::Alignment nameColumnHAlign;
    if (m_quiet)
        nameColumnHAlign = Qt::AlignHCenter;
    else
        nameColumnHAlign = Qt::AlignLeft;

    if (role == Qt::TextAlignmentRole) {
        switch (index.column()) {
        case 0:
            return QVariant(nameColumnHAlign | Qt::AlignVCenter);
        case 1:
            return QVariant(Qt::AlignRight | Qt::AlignVCenter);
        case 2:
            // fall-through
        case 3:
            // fall-through
        case 4:
            // fall-through
        case 5:
            // fall-through
        case 6:
            return QVariant(Qt::AlignHCenter | Qt::AlignVCenter);
        }
    }
    if (role == Qt::DisplayRole) {
        Instance instance = instanceList.at(index.row());
        switch (index.column()) {
        case 0:
            return instance.name();
        case 1:
            return instance.status();
        case 2:
            return instance.vmType();
        case 3:
            return instance.arch();
        case 4:
            return instance.strCpus();
        case 5:
            return instance.strMemory();
        case 6:
            return instance.strDisk();
        }
    }
    return QVariant();
}

QVariant InstanceModel::headerData(int section, Qt::Orientation orientation, int role) const
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
            return tr("VMtype");
        case 3:
            return tr("Arch");
        case 4:
            return tr("CPUs");
        case 5:
            return tr("Mem.");
        case 6:
            return tr("Disk");
        }
    }
    return QVariant();
}
