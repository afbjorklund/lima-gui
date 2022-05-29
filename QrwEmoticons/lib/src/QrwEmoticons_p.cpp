#include "QrwEmoticons_p.h"
#include "QrwEmoticonsTextObjectInterface_p.h"
#include <QLibraryInfo>
#include <QCoreApplication>
#include <QAbstractTextDocumentLayout>
#include <QTextDocumentFragment>
#include <QTextBlock>
#include <QTextLayout>

QrwEmoticonsPrivate::QrwEmoticonsPrivate(QrwEmoticons* q, QTextDocument* document)
    : QObject(q), q_ptr(q), m_CurrentlyApplying(false), m_TextDocument(document)
    , m_MaxEmoticonCharCodeCount(8), m_MinimumEmoticonSize(20,20)
{
    bool loaded = this->loadPlugin();
    Q_ASSERT( loaded );

    Q_ASSERT( m_TextDocument );
    if( m_TextDocument )
    {
        connect(document, &QTextDocument::contentsChange, this, &QrwEmoticonsPrivate::onTextDocumentContentsChanged);

        QrwEmoticonsTextObjectInterface* interf = new QrwEmoticonsTextObjectInterface(this);
        m_TextDocument->documentLayout()->registerHandler(QrwEmoticonsPrivate::EmoticonTextFormatObjectType, interf);
        this->applyTextCharFormat();
    }
}

QrwEmoticonsPrivate::~QrwEmoticonsPrivate()
{
    if( m_PluginLoader.isLoaded() )
        m_PluginLoader.unload();
}

QString QrwEmoticonsPrivate::GetEmoticonString( const QrwEmoticons::Emoticon & code )
{
    QString emoticonStr;
    for( int i = 0; i < code.size(); ++i )
        emoticonStr += QrwEmoticonsPrivate::GetEmoticonString( code.at(i) );
    return emoticonStr;
}

QString QrwEmoticonsPrivate::GetEmoticonString( uint character )
{
    QString emoticonStr;
    if( QChar::requiresSurrogates(character) )
    {
        QChar chars[] = {
            QChar(QChar::highSurrogate(character)),
            QChar(QChar::lowSurrogate(character))
        };
        emoticonStr = QString(chars,2);
    }
    else
        emoticonStr = QChar(character);

    return emoticonStr;
}

void QrwEmoticonsPrivate::onTextDocumentContentsChanged(int position, int /*charsRemoved*/, int charsAdded)
{
    if( charsAdded > 0 )
        this->applyTextCharFormat( position );
}

bool QrwEmoticonsPrivate::isEmoticon( uint character )
{
    return this->isEmoticon( QrwEmoticons::Emoticon() << character );
}

bool QrwEmoticonsPrivate::isEmoticon( const QrwEmoticons::Emoticon & code )
{
    if( code.isEmpty() )
        return false;
    return EMOTICONS.contains( code );
}

QrwEmoticons::Emoticon QrwEmoticonsPrivate::getEmoticonCode( QTextCursor c, int* selectionLength )
{
    if( selectionLength )
        *selectionLength = 0;

    if( c.isNull() || c.atEnd() )
        return QrwEmoticons::Emoticon();

    int start = c.position();
    c.setPosition( start, QTextCursor::MoveAnchor );
    int end = start;
    // NOTE:
    //  QTextCursor::movePosition(QTextCursor::NextCharacter) can increase the actual
    //  selected characters by more than one, so we are checking the selection here
    //  instead of simply iterating to the max count
    while( c.selectedText().length() < (m_MaxEmoticonCharCodeCount*2) )
    {
        if( NOT c.movePosition( QTextCursor::NextCharacter, QTextCursor::KeepAnchor, 1) )
            break;
        end = c.position();
    }

    while( end > start )
    {
        c.setPosition( start, QTextCursor::MoveAnchor );
        c.setPosition( end, QTextCursor::KeepAnchor );

        const QString selection = c.selectedText();

        QrwEmoticons::Emoticon code;

        for( int i = 0; i < selection.length(); ++i )
        {
            const ushort prevCharacter = i > 0 ? selection[i-1].unicode() : 0x0;
            const ushort unicodeCharacter = selection[i].unicode();

            uint emoticonCharCode = 0x0;

            if( QChar::isHighSurrogate(unicodeCharacter) )
                continue;

            if( QChar::isLowSurrogate(unicodeCharacter) && QChar::isHighSurrogate(prevCharacter) )
            {
                // convert unicode surrogate pair back to unicode value
                emoticonCharCode = QChar::surrogateToUcs4(prevCharacter,unicodeCharacter);
                // QChar::surrogateToUcs4() is a little bit faster than the following:
                //const uint emoticonCharCode = ((hi - 0xd800) * 0x400) + (lo - 0xdc00) + 0x10000;
            }
            else
                emoticonCharCode = unicodeCharacter;

            if( emoticonCharCode > 0 )
                code << emoticonCharCode;
            else
                break;
        }

        if( this->isEmoticon(code) )
        {
            if( selectionLength )
                *selectionLength = selection.length();
            return code;
        }

        if( !c.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor, 1) )
            break;
        end = c.position();
    }

    return QrwEmoticons::Emoticon();
}

QString QrwEmoticonsPrivate::getDocumentText( bool html ) const
{
    if( NOT m_TextDocument )
        return QString();

    QScopedPointer<QTextDocument> doc(m_TextDocument->clone());

    QTextCursor c( doc.data() );

    c.movePosition( QTextCursor::Start, QTextCursor::MoveAnchor );
    while( NOT c.atEnd() )
    {
        if( NOT c.movePosition( QTextCursor::NextCharacter, QTextCursor::KeepAnchor, 1 ) )
            break;
        const QString selection = c.selectedText().trimmed();
        if( selection == QString(QChar::ObjectReplacementCharacter) )
        {
            const QTextCharFormat f = c.charFormat();
            if( f.objectType() == QrwEmoticonsPrivate::EmoticonTextFormatObjectType )
            {
                const QrwEmoticons::Emoticon code = f.property( QrwEmoticonsPrivate::EmoticonUnicode ).value<QrwEmoticons::Emoticon>();
                Q_ASSERT( NOT code.isEmpty() );

                const QString replaceText = f.stringProperty( QrwEmoticonsPrivate::EmoticonQString );
                Q_ASSERT( NOT replaceText.isEmpty() );

                c.insertText( replaceText );
            }
        }

        c.setPosition( c.position(), QTextCursor::MoveAnchor ); // "collapse" selection
    }

    return html ? doc->toHtml() : doc->toPlainText();
}

QString QrwEmoticonsPrivate::getCursorText( const QTextCursor & cursor, bool html ) const
{
    if( NOT cursor.document() || NOT cursor.hasSelection() )
        return QString();
    else if( cursor.hasComplexSelection() )
    {
        qWarning("QrwEmoticons: QTextCursor with complex selections may not work as expected");
    }

    QScopedPointer<QTextDocument> doc( m_TextDocument->clone() );
    QTextCursor c( doc.data() );
        c.setPosition( qMin(cursor.position(), cursor.anchor()), QTextCursor::MoveAnchor );
        c.setPosition( qMax(cursor.position(), cursor.anchor()), QTextCursor::KeepAnchor );

    int start = c.anchor();
    int end = c.position();

    c.setPosition( start, QTextCursor::MoveAnchor );
    while( c.position() <= end )
    {
        if( NOT c.movePosition( QTextCursor::NextCharacter, QTextCursor::KeepAnchor, 1 ) )
            break;
        const QString selection = c.selectedText().trimmed();
        if( selection == QString(QChar::ObjectReplacementCharacter) )
        {
            const QTextCharFormat f = c.charFormat();
            if( f.objectType() == QrwEmoticonsPrivate::EmoticonTextFormatObjectType )
            {
                const QrwEmoticons::Emoticon code = f.property( QrwEmoticonsPrivate::EmoticonUnicode ).value<QrwEmoticons::Emoticon>();
                Q_ASSERT( NOT code.isEmpty() );

                const QString replaceText = f.stringProperty( QrwEmoticonsPrivate::EmoticonQString );
                Q_ASSERT( NOT replaceText.isEmpty() );

                end += (replaceText.length()-1); // i am not quite sure why this is actually needed (only in conjunction with QTextDocumentFragment)
                c.insertText( replaceText, QTextCharFormat() );
            }
        }

        c.setPosition( c.position(), QTextCursor::MoveAnchor ); // "collapse" selection
    }

    c.setPosition( start, QTextCursor::MoveAnchor );
    c.setPosition( end, QTextCursor::KeepAnchor );
    QTextDocumentFragment fragment = c.selection();
    QString text = html ? fragment.toHtml() : fragment.toPlainText();
    return text;
}

void QrwEmoticonsPrivate::applyTextCharFormat(int pos)
{
    if( m_CurrentlyApplying )
        return;

    m_CurrentlyApplying = true; // recursion protection
    const bool isModified = m_TextDocument->isModified();

    QTextCursor c( m_TextDocument );
    if( pos == 0 )
        c.movePosition( QTextCursor::Start, QTextCursor::MoveAnchor );
    else
        c.setPosition( pos, QTextCursor::MoveAnchor );

    c.beginEditBlock();
    while( NOT c.atEnd() )
    {
        int increase = 1;

        QrwEmoticons::Emoticon code = this->getEmoticonCode( c, &increase );
        if( NOT code.isEmpty() )
        {
            QTextCharFormat emoticonFormat;
                emoticonFormat.setObjectType( QrwEmoticonsPrivate::EmoticonTextFormatObjectType );
                emoticonFormat.setProperty( QrwEmoticonsPrivate::EmoticonUnicode, QVariant::fromValue<QrwEmoticons::Emoticon>(code) );
                //emoticonFormat.setProperty( QrwEmoticonsPrivate::EmoticonImagePath, QVariant::fromValue<QString>(this->GetEmoticonPath(code)) );
                emoticonFormat.setProperty( QrwEmoticonsPrivate::EmoticonQString, QVariant::fromValue<QString>(QrwEmoticonsPrivate::GetEmoticonString(code)) );
                emoticonFormat.setVerticalAlignment( QTextCharFormat::AlignBottom );

            c.setPosition( c.position()+increase, QTextCursor::KeepAnchor );
            c.insertText( QString(QChar::ObjectReplacementCharacter), emoticonFormat );
            //c.setPosition( c.position(), QTextCursor::MoveAnchor ); // collapse selection
        }
        else if( NOT c.movePosition( QTextCursor::NextCharacter, QTextCursor::MoveAnchor, 1 ) )
            break;
    }
    c.endEditBlock();

    m_TextDocument->setModified( isModified );
    m_CurrentlyApplying = false;
}

bool QrwEmoticonsPrivate::loadPlugin(const QString & id)
{
    if( id.length() > 0 && m_Plugin.metaData.value(QStringLiteral("id")).toString() == id )
        return true;

    if( m_PluginLoader.isLoaded() )
    {
        m_PluginLoader.unload();
        m_Plugin.clear();
    }

    QStringList pluginPaths = QStringList()
            << QCoreApplication::applicationDirPath()
            << QLibraryInfo::location(QLibraryInfo::PluginsPath)
            << QCoreApplication::libraryPaths();

    for( int p = 0; p < pluginPaths.count(); ++p )
    {
        QString path = pluginPaths.at(p) % QStringLiteral("/emoticons");
        QDir dir(path);
        if( !dir.exists(path) )
            continue;

        QStringList files = dir.entryList(QDir::Files);
        for( int f = 0; f < files.count(); ++f )
        {
            QString fileName = files.at(f);

            m_PluginLoader.setFileName( dir.absoluteFilePath(fileName) );

            if( m_PluginLoader.metaData().value("IID").toString() != QrwEmoticonsPluginInterface_iid )
                continue;

            QJsonObject metaData = m_PluginLoader.metaData().value("MetaData").toObject();
            if( !id.isEmpty() && metaData.value(QStringLiteral("id")).toString() != id )
                continue;

            if( m_PluginLoader.load() )
            {
                if( QrwEmoticonsPluginInterface* pluginInterface = qobject_cast<QrwEmoticonsPluginInterface*>(m_PluginLoader.instance()) )
                {
                    m_Plugin.interf = pluginInterface;
                    m_Plugin.metaData = metaData;
                    return true;
                }
            }
        }
    }

    return false;
}

int QrwEmoticonsPrivate::getLineHeight(int posInDocument, const QTextFormat & format) const
{
    switch( format.type() )
    {
        case QTextFormat::BlockFormat:
        {
            QTextBlockFormat blockFormat = format.toBlockFormat();
            //return int(blockFormat.lineHeight());

            QTextBlock block = m_TextDocument->findBlock(posInDocument);
            int h = qRound(block.layout()->lineAt(0).height());

            qDebug() << blockFormat.lineHeight() << h;
            return h;
        }
        case QTextFormat::CharFormat:
        {
            QTextCharFormat charFormat = format.toCharFormat();
            QFont font = charFormat.font();

            int h = font.pixelSize();
            if( h == -1 )
                h = qRound(font.pointSizeF() * 1.33333); // pt -> px

            return h;
        }
        default:
            Q_ASSERT( false );
            return 20;
    }
}
