#ifndef CONTAINER_H
#define CONTAINER_H

#include <QJsonObject>

#include <QAbstractListModel>
#include <QString>
#include <QList>
#include <QMap>

// CONTAINER ID  IMAGE  COMMAND  CREATED STATUS  PORTS  NAMES

//! [0]
class Container
{
public:
    Container() : Container("") { }
    Container(const QString &id)
        : m_id(id),
          m_image(""),
          m_command(""),
          m_created(""),
          m_status(""),
          m_ports(""),
          m_names(""),
          m_platform("")
    {
    }
    Container(const QJsonObject &obj);

    QString id() const { return m_id; }
    QString image() const { return m_image; }
    void setImage(QString image) { m_image = image; }
    QString command() const { return m_command; }
    void setCommand(QString command) { m_command = command; }
    QString created() const { return m_created; }
    void setCreated(QString created) { m_created = created; }
    QString status() const { return m_status; }
    void setStatus(QString status) { m_status = status; }
    QString ports() const { return m_ports; }
    void setPorts(QString ports) { m_ports = ports; }
    QString names() const { return m_names; }
    void setNames(QString names) { m_names = names; }
    QString platform() const { return m_platform; }
    void setPlatform(QString platform) { m_platform = platform; }

private:
    QString m_id;
    QString m_image;
    QString m_command;
    QString m_created;
    QString m_status;
    QString m_ports;
    QString m_names;
    QString m_platform;
};
//! [0]

typedef QList<Container> ContainerList;
typedef QHash<QString, Container> ContainerHash;

//! [1]
class ContainerModel : public QAbstractListModel
{
    Q_OBJECT

public:
    ContainerModel(const ContainerList &containers, QObject *parent = nullptr)
        : QAbstractListModel(parent), containerList(containers)
    {
    }

    void setContainers(const ContainerList &containers);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

private:
    ContainerList containerList;
};
//! [1]

#endif // INSTANCE_H
