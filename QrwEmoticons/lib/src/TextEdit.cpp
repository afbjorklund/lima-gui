#include <QrwEmoticons/TextEdit.h>
#include <QrwEmoticons/QrwEmoticons.h>
#include <QMimeData>

QrwEmoticonsTextEdit::QrwEmoticonsTextEdit(QWidget* parent)
    : QTextEdit(parent)
{
    m_Emoticons = new QrwEmoticons(this->document(),this);
}

QString QrwEmoticonsTextEdit::toHtml() const
{
    return m_Emoticons->getHtml();
}

QString QrwEmoticonsTextEdit::toPlainText() const
{
    return m_Emoticons->getPlainText();
}

QrwEmoticons* QrwEmoticonsTextEdit::emoticons() const
{
    return m_Emoticons;
}

void QrwEmoticonsTextEdit::relayout()
{
    QTextDocument* doc = this->document();
    doc->markContentsDirty(0, doc->toPlainText().length());
}

QMimeData* QrwEmoticonsTextEdit::createMimeDataFromSelection() const
{
    QTextCursor cursor = this->textCursor();
    if( NOT cursor.hasSelection() )
        return Q_NULLPTR;

    const QString htmlText = m_Emoticons->getHtml( cursor );
    const QString plainText = m_Emoticons->getPlainText( cursor );

    QMimeData* data = new QMimeData;
        data->setHtml( htmlText );
        data->setText( plainText );
    return data;
}

/*
QString stripQtTags( const QString & html )
{
    // strip html - only get contents between <body>...</body> tags
    int start = html.indexOf( QLatin1String("<p"), 0, Qt::CaseInsensitive );
    int end = html.lastIndexOf( QLatin1String("</p>"), -1, Qt::CaseInsensitive );
    return html.mid( start, end-start+4 );
}
*/
