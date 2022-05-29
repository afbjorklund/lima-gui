#ifndef QRWEMOTICONS_H
#define QRWEMOTICONS_H

#include <QrwEmoticons/Global.h>
#include <QtGui/QTextDocument>
#include <QtGui/QTextObjectInterface>

class QrwEmoticonsPrivate;

class QRWEMOTICONS_EXPORT QrwEmoticons : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QString provider READ provider WRITE setProvider NOTIFY providerChanged )
    Q_PROPERTY( int maximumEmoticonCharCount READ maximumEmoticonCharCount WRITE setMaximumEmoticonCharCount NOTIFY maximumEmoticonCharCountChanged )
    Q_PROPERTY( QSize minimumEmoticonSize READ minimumEmoticonSize WRITE setMinimumEmoticonSize NOTIFY minimumEmoticonSizeChanged )

    QRWEMOTICONS_DECLARE_PRIVATE(QrwEmoticons)

public:
    explicit QrwEmoticons(QTextDocument* document, QObject* parent = Q_NULLPTR);
    virtual ~QrwEmoticons() Q_DECL_OVERRIDE;

    typedef QVector<uint> Emoticon;

    QString getPlainText() const;
    QString getPlainText(const QTextCursor & cursor) const;
    QString getHtml() const;
    QString getHtml(const QTextCursor & cursor) const;

    QString provider() const;
    void setProvider(const QString & provider, bool dynamic = true);
    quint8 maximumEmoticonCharCount() const;
    void setMaximumEmoticonCharCount(quint8 count);
    QSize minimumEmoticonSize() const;
    void setMinimumEmoticonSize(const QSize & size);

Q_SIGNALS:
    void providerChanged(const QString & provider);
    void maximumEmoticonCharCountChanged(quint8 maximumEmoticonCharCount);
    void minimumEmoticonSizeChanged(const QSize & minimumEmoticonSize);
};

Q_DECLARE_METATYPE(QrwEmoticons::Emoticon)

#endif // QRWEMOTICONS_H
