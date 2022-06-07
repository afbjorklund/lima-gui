#include "image.h"

#include <QStringList>

Image::Image(const QJsonObject &obj)
{
    QString id;
    if (obj.contains("ID")) {
        id = obj["ID"].toString();
    }
    if (id.isEmpty()) {
        return;
    }
    m_id = id;

    if (obj.contains("Repository")) {
        QString repository = obj["Repository"].toString();
        setRepository(repository);
    }
    if (obj.contains("Tag")) {
        QString tag = obj[""].toString();
        setTag(tag);
    }
    if (obj.contains("CreatedAt")) {
        QString created = obj["CreatedAt"].toString();
        setCreated(created);
    }
    if (obj.contains("Platform")) {
        QString platform = obj["Platform"].toString();
        setPlatform(platform);
    }
    if (obj.contains("Digest")) {
        QString digest = obj["Digest"].toString();
        setDigest(digest);
    }
}

void ImageModel::setImages(const ImageList &images)
{
    beginResetModel();
    imageList = images;
    endResetModel();
}

int ImageModel::rowCount(const QModelIndex &) const
{
    return imageList.count();
}

int ImageModel::columnCount(const QModelIndex &) const
{
    return 1;
}

QVariant ImageModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= rowCount(index))
        return QVariant();
    if (index.column() >= columnCount(index))
        return QVariant();

    if (role == Qt::DisplayRole) {
        Image image = imageList.at(index.row());
        switch (index.column()) {
        case 0:
            return image.id();
        }
    }
    return QVariant();
}

QVariant ImageModel::headerData(int section, Qt::Orientation orientation, int role) const
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
