#include "container.h"

#include <QStringList>
#include <QJsonObject>

Container::Container(const QJsonObject &obj)
{
    QString id;
    if (obj.contains("ID")) {
        id = obj["ID"].toString();
    }
    if (id.isEmpty()) {
        return;
    }
    m_id = id;

    if (obj.contains("Image")) {
        QString image = obj["Image"].toString();
        setImage(image);
    }
    if (obj.contains("Command")) {
        QString command = obj["Command"].toString();
        setCommand(command);
    }
    if (obj.contains("CreatedAt")) {
        QString command = obj["CreatedAt"].toString();
        setCommand(command);
    }
    if (obj.contains("Status")) {
        QString status = obj["Status"].toString();
        setStatus(status);
    }
    if (obj.contains("Ports")) {
        QString ports = obj["Ports"].toString();
        setPorts(ports);
    }
    if (obj.contains("Names")) {
        QString names = obj["Names"].toString();
        setNames(names);
    }
}

void ContainerModel::setContainers(const ContainerList &containers)
{
    beginResetModel();
    containerList = containers;
    endResetModel();
}

int ContainerModel::rowCount(const QModelIndex &) const
{
    return containerList.count();
}

int ContainerModel::columnCount(const QModelIndex &) const
{
    return 1;
}

QVariant ContainerModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= rowCount(index))
        return QVariant();
    if (index.column() >= columnCount(index))
        return QVariant();

    if (role == Qt::DisplayRole) {
        Container container = containerList.at(index.row());
        switch (index.column()) {
        case 0:
            return container.id();
        }
    }
    return QVariant();
}

QVariant ContainerModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return tr("ID");
        }
    }
    return QVariant();
}
