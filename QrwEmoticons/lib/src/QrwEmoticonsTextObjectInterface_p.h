#ifndef QRWEMOTICONSTEXTOBJECTINTERFACE_H
#define QRWEMOTICONSTEXTOBJECTINTERFACE_H

#include <QrwEmoticons/QrwEmoticons.h>
#include <QtGui/QTextObjectInterface>

class QrwEmoticonsPrivate;

class QrwEmoticonsTextObjectInterface : public QObject, public QTextObjectInterface
{
    Q_OBJECT
    Q_INTERFACES(QTextObjectInterface)

public:
    explicit QrwEmoticonsTextObjectInterface(QrwEmoticonsPrivate* pvt);

    // QTextObjectInterface interface
    virtual void drawObject(QPainter* painter, const QRectF & rect, QTextDocument* doc, int posInDocument, const QTextFormat & format) Q_DECL_OVERRIDE;
    virtual QSizeF intrinsicSize(QTextDocument* doc, int posInDocument, const QTextFormat & format) Q_DECL_OVERRIDE;

private:
    QrwEmoticonsPrivate* m_Prvt;
};

#endif // QRWEMOTICONSTEXTOBJECTINTERFACE_H
