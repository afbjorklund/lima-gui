#ifndef INSTANCE_H
#define INSTANCE_H

#include <QAbstractListModel>
#include <QString>
#include <QList>
#include <QMap>

//! [0]
class Instance
{
public:
    Instance() {}
    Instance(const QString &name)
        : m_name(name), m_status("") {}

    QString name() const {
        return m_name;
    }
    QString status() const {
        return m_status;
    }
    void setStatus(QString status) {
        m_status = status;
    }
private:
    QString m_name;
    QString m_status;
};
//! [0]

typedef QList<Instance> InstanceList;
typedef QHash<QString, Instance> InstanceHash;

//! [1]
class InstanceModel : public QAbstractListModel
{
    Q_OBJECT

public:
    InstanceModel(const InstanceList &instances, QObject *parent = nullptr)
        : QAbstractListModel(parent), instanceList(instances) {}

    void setInstances(const InstanceList &instances);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

private:
    InstanceList instanceList;
};
//! [1]

#endif // INSTANCE_H
