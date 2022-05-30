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
    Instance() : Instance("") { }
    Instance(const QString &name)
        : m_name(name), m_status(""), m_dir(""), m_arch(""), m_cpus(0), m_memory(0), m_disk(0)
    {
    }

    QString name() const { return m_name; }
    QString status() const { return m_status; }
    void setStatus(QString status) { m_status = status; }
    QString dir() const { return m_dir; }
    void setDir(QString dir) { m_dir = dir; }
    QString arch() const { return m_arch; }
    void setArch(QString arch) { m_arch = arch; }
    int cpus() const { return m_cpus; }
    void setCpus(int cpus) { m_cpus = cpus; }
    qint64 memory() const { return m_memory; }
    void setMemory(qint64 memory) { m_memory = memory; }
    qint64 disk() const { return m_disk; }
    void setDisk(qint64 disk) { m_disk = disk; }

private:
    QString m_name;
    QString m_status;
    QString m_dir;
    QString m_arch;
    int m_cpus;
    qint64 m_memory;
    qint64 m_disk;
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
        : QAbstractListModel(parent), instanceList(instances), m_quiet(false)
    {
    }

    void setInstances(const InstanceList &instances);
    void setQuiet(bool quiet) { m_quiet = quiet; }
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

private:
    InstanceList instanceList;
    bool m_quiet;
};
//! [1]

#endif // INSTANCE_H
