#ifndef QRWEMOTICONSPLUGININTERFACE_H
#define QRWEMOTICONSPLUGININTERFACE_H

#include <QtCore>
#include <QObject>
#include <QrwEmoticons/QrwEmoticons.h>

class QrwEmoticonsPluginInterface : public QObject
{
protected:
    explicit QrwEmoticonsPluginInterface(QObject* parent = Q_NULLPTR) : QObject(parent) {}

public:
    virtual ~QrwEmoticonsPluginInterface() {}

    virtual bool hasEmoticon(const QrwEmoticons::Emoticon & code) = 0;
    virtual QPixmap getEmoticon(const QrwEmoticons::Emoticon & code, const QSize & size) = 0;
};

#define QrwEmoticonsPluginInterface_iid "net.raven-worx.QrwEmoticons.PluginInterface"
Q_DECLARE_INTERFACE(QrwEmoticonsPluginInterface, QrwEmoticonsPluginInterface_iid)

#endif //QRWEMOTICONSPLUGININTERFACE_H
