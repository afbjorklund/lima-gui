#ifndef IMAGE_H
#define IMAGE_H

#include <QJsonObject>

#include <QAbstractListModel>
#include <QString>
#include <QList>
#include <QMap>

// REPOSITORY  TAG  IMAGE ID  CREATED  PLATFORM SIZE  BLOB SIZE

//! [0]
class Image
{
public:
    Image() : Image("") { }
    Image(const QString &id)
        : m_id(id),
          m_repository(""),
          m_tag(""),
          m_created(""),
          m_platform(""),
          m_size(0),
          m_blob_size(0),
          m_digest("")
    {
    }
    Image(const QJsonObject &obj);

    QString id() const { return m_id; }
    QString repository() const { return m_repository; }
    void setRepository(QString repository) { m_repository = repository; }
    QString tag() const { return m_tag; }
    void setTag(QString tag) { m_tag = tag; }
    QString created() const { return m_created; }
    void setCreated(QString created) { m_created = created; }
    QString platform() const { return m_platform; }
    void setPlatform(QString platform) { m_platform = platform; }
    QString digest() const { return m_digest; }
    void setDigest(QString digest) { m_digest = digest; }

private:
    QString m_id;
    QString m_repository;
    QString m_tag;
    QString m_created;
    QString m_platform;
    qint64 m_size;
    qint64 m_blob_size;
    QString m_digest;
};
//! [0]

typedef QList<Image> ImageList;
typedef QHash<QString, Image> ImageHash;

//! [1]
class ImageModel : public QAbstractListModel
{
    Q_OBJECT

public:
    ImageModel(const ImageList &images, QObject *parent = nullptr)
        : QAbstractListModel(parent), imageList(images)
    {
    }

    void setImages(const ImageList &images);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

private:
    ImageList imageList;
};
//! [1]

#endif // IMAGE_H
