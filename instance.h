#ifndef INSTANCE_H
#define INSTANCE_H

#include <QAbstractListModel>

class QStringList;

//! [0]
class InstanceModel : public QAbstractListModel
{
    Q_OBJECT

public:
    InstanceModel(const QStringList &strings, QObject *parent = nullptr)
        : QAbstractListModel(parent), stringList(strings) {}

    void setInstances(const QStringList &strings);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

private:
    QStringList stringList;
};
//! [0]

#endif // INSTANCE_H
