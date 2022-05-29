#include "QrwEmoticonsTextObjectInterface_p.h"
#include "QrwEmoticons_p.h"
#include <QPainter>

QrwEmoticonsTextObjectInterface::QrwEmoticonsTextObjectInterface(QrwEmoticonsPrivate* pvt)
    : QObject(pvt), m_Prvt(pvt)
{
}

void QrwEmoticonsTextObjectInterface::drawObject(QPainter* painter, const QRectF & rect, QTextDocument* doc, int posInDocument, const QTextFormat & format)
{
    Q_UNUSED(doc);
    Q_UNUSED(posInDocument);

    const QrwEmoticons::Emoticon code = format.property( QrwEmoticonsPrivate::EmoticonUnicode ).value<QrwEmoticons::Emoticon>();
    //const QString emoticonImagePath = format.stringProperty( QrwEmoticonsPrivate::EmoticonImagePath );
    //const QPixmap pix( emoticonImagePath );

    const QRectF drawRect = rect.adjusted(1,0,-1,0);
    painter->save();
        if( !m_Prvt->m_Plugin || !m_Prvt->m_Plugin.interf->hasEmoticon(code) )
        {
            painter->setPen( QPen(Qt::darkGray, 1, Qt::SolidLine) );
            painter->setBrush( QColor(240,240,240) );
            painter->drawRect( drawRect.adjusted(1,0,-1,0) ); // adjust again
            painter->setFont( QFont("typewriter", qMax(6,int(drawRect.height()*0.6))) );
            painter->drawText( drawRect, Qt::AlignCenter, QStringLiteral("?") );
        }
        else
        {
            const QPixmap pix = m_Prvt->m_Plugin.interf->getEmoticon(code, drawRect.size().toSize());

            painter->setPen( Qt::NoPen );
            painter->setBrush( Qt::NoBrush );
            painter->drawPixmap( drawRect, pix, QRectF(QPointF(0,0),pix.size()) );
        }
    painter->restore();
}

QSizeF QrwEmoticonsTextObjectInterface::intrinsicSize(QTextDocument* doc, int posInDocument, const QTextFormat & format)
{
    int height = m_Prvt->getLineHeight(posInDocument, format);
    QSize size(height+2, height);
    if( m_Prvt->m_MinimumEmoticonSize.isValid() )
        size = size.expandedTo( m_Prvt->m_MinimumEmoticonSize );
    return size;
}
