#include "QrwEmoticonsHelper.h"
#include <QtGui>
#include <QtSvg>
#include <QtXml>

QByteArray QrwEmoticonsHelper::SvgPrepare(const QString &fileName)
{
    QDomDocument doc;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file))
        return QByteArray();

    /*
     * QSvgRenderer fill certain elements with "black" if they are not
     * explicitly filled
     */

    const QString fillKey = QStringLiteral("fill");
    const QString fillValue = QStringLiteral("transparent");

    QDomNodeList pathElements = doc.elementsByTagName( QStringLiteral("path") );
    for( int i = 0; i < pathElements.size(); i++ )
    {
        QDomElement e = pathElements.item(i).toElement();
        if( !e.hasAttribute(fillKey) )
            e.setAttribute(fillKey, fillValue);
    }
    QDomNodeList rectElements = doc.elementsByTagName( QStringLiteral("rect") );
    for( int i = 0; i < rectElements.size(); i++ )
    {
        QDomElement e = rectElements.item(i).toElement();
        if( !e.hasAttribute(fillKey) )
            e.setAttribute(fillKey, fillValue);
    }
    return doc.toByteArray();
}

QPixmap QrwEmoticonsHelper::SvgToPixmap(const QString & fileName, const QSize & size)
{
    QPixmap pix;
    const QString cacheKey = fileName % QStringLiteral("@%1x%2").arg(size.width()).arg(size.height());
    if( !QPixmapCache::find(cacheKey, &pix) )
    {
        if( QFile::exists(fileName) )
        {
            const QByteArray svg = QrwEmoticonsHelper::SvgPrepare(fileName);
            QSvgRenderer renderer(svg);
            if( renderer.isValid() )
            {
                pix = QPixmap(size);
                pix.fill(Qt::transparent);

                QPainter pixPainter(&pix);
                renderer.render(&pixPainter, QRectF()); // bounding rectangle is not specified the SVG file is mapped to the whole paint device

                pixPainter.end();

                QPixmapCache::insert(cacheKey, pix);
            }
        }
    }
    return pix;
}
