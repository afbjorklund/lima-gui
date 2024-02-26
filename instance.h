#ifndef INSTANCE_H
#define INSTANCE_H

#include <QJsonObject>

#include <QAbstractListModel>
#include <QString>
#include <QList>
#include <QMap>

// NAME  STATUS  SSH  VMTYPE  ARCH  CPUS  MEMORY  DISK  DIR

//! [0]
class Instance
{
public:
    Instance() : Instance("") { }
    Instance(const QString &name)
        : m_name(name),
          m_status(""),
          m_dir(""),
          m_vm_type(""),
          m_mount_type(""),
          m_arch(""),
          m_cpu_type(""),
          m_cpus(0),
          m_memory(0),
          m_disk(0),
          m_ssh_address(""),
          m_ssh_local_port(0),
          m_ssh_config_file("")
    {
    }
    Instance(const QJsonObject &obj);

    QString name() const { return m_name; }
    QString status() const { return m_status; }
    void setStatus(QString status) { m_status = status; }
    QString dir() const { return m_dir; }
    void setDir(QString dir) { m_dir = dir; }
    QString vmType() const { return m_vm_type; }
    void setVmType(QString type) { m_vm_type = type; }
    QString mountType() const { return m_mount_type; }
    void setMountType(QString type) { m_mount_type = type; }
    QString arch() const { return m_arch; }
    void setArch(QString arch) { m_arch = arch; }
    QString cpuType() const { return m_cpu_type; }
    void setCpuType(QString type) { m_cpu_type = type; }
    int cpus() const { return m_cpus; }
    QString strCpus() const;
    void setCpus(int cpus) { m_cpus = cpus; }
    qint64 memory() const { return m_memory; }
    QString strMemory() const;
    void setMemory(qint64 memory) { m_memory = memory; }
    qint64 disk() const { return m_disk; }
    QString strDisk() const;
    void setDisk(qint64 disk) { m_disk = disk; }
    QString sshAddress() const { return m_ssh_address; }
    void setSshAddress(QString addr) { m_ssh_address = addr; }
    int sshLocalPort() const { return m_ssh_local_port; }
    void setSshLocalPort(int port) { m_ssh_local_port = port; }
    QString sshConfigFile() const { return m_ssh_config_file; }
    void setSshConfigFile(QString file) { m_ssh_config_file = file; }
    QVariantMap config() const { return m_config; }
    void setConfig(QVariantMap config) { m_config = config; }

    QString audioDevice();
    QString videoDisplay();

private:
    QString m_name;
    QString m_status;
    QString m_dir;
    QString m_vm_type;
    QString m_mount_type;
    QString m_arch;
    QString m_cpu_type;
    int m_cpus;
    qint64 m_memory;
    qint64 m_disk;
    QString m_ssh_address;
    int m_ssh_local_port;
    QString m_ssh_config_file;
    QVariantMap m_config;
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
