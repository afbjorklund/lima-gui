#include <QrwEmoticons/QrwEmoticons.h>
#include "QrwEmoticons_p.h"

QrwEmoticons::QrwEmoticons(QTextDocument* document, QObject* parent)
    : QObject(parent)
{
    p_ptr = new QrwEmoticonsPrivate(this, document);
}

QrwEmoticons::~QrwEmoticons()
{
}

QString QrwEmoticons::getPlainText() const
{
    Q_D(const QrwEmoticons);
    return d->getDocumentText( false );
}

QString QrwEmoticons::getPlainText(const QTextCursor & cursor) const
{
    Q_D(const QrwEmoticons);
    return d->getCursorText( cursor, false );
}

QString QrwEmoticons::getHtml() const
{
    Q_D(const QrwEmoticons);
    return d->getDocumentText( true );
}

QString QrwEmoticons::getHtml(const QTextCursor & cursor) const
{
    Q_D(const QrwEmoticons);
    return d->getCursorText( cursor, true );
}

QString QrwEmoticons::provider() const
{
    Q_D(const QrwEmoticons);
    if( d->m_Plugin )
        return d->m_Plugin.metaData.value(QStringLiteral("id")).toString();
    else
        return QString();
}

void QrwEmoticons::setProvider(const QString & plugin)
{
    Q_D(QrwEmoticons);
    d->loadPlugin(plugin);
}

quint8 QrwEmoticons::maximumEmoticonCharCount() const
{
    Q_D(const QrwEmoticons);
    return d->m_MaxEmoticonCharCodeCount;
}

void QrwEmoticons::setMaximumEmoticonCharCount(quint8 count)
{
    Q_D(QrwEmoticons);
    if( d->m_MaxEmoticonCharCodeCount != count )
    {
        d->m_MaxEmoticonCharCodeCount = count;
        Q_EMIT maximumEmoticonCharCountChanged(count);
    }
}

QSize QrwEmoticons::minimumEmoticonSize() const
{
    Q_D(const QrwEmoticons);
    return d->m_MinimumEmoticonSize;
}

void QrwEmoticons::setMinimumEmoticonSize(const QSize & size)
{
    Q_D(QrwEmoticons);
    if( size.isValid() && d->m_MinimumEmoticonSize != size )
    {
        d->m_MinimumEmoticonSize = size;
        Q_EMIT minimumEmoticonSizeChanged(size);
    }
}
