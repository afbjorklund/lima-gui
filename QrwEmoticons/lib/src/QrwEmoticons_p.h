#ifndef QRWEMOTICONS_P_H
#define QRWEMOTICONS_P_H

#include <QrwEmoticons/QrwEmoticons>
#include <QPluginLoader>
#include <QJsonObject>
#include <QPointer>

typedef QSet<QrwEmoticons::Emoticon> EmoticonsData;

class QrwEmoticonsPrivate : public QObject
{
    Q_OBJECT

    QRWEMOTICONS_DECLARE_PUBLIC(QrwEmoticons)

    friend class QrwEmoticonsTextObjectInterface;

public:
    ~QrwEmoticonsPrivate();

    enum TextFormatProperty {
        EmoticonUnicode = QTextFormat::UserProperty,
        EmoticonImagePath,
        EmoticonQString
    };
    Q_ENUMS(TextFormatProperty)

    static QString GetEmoticonString( const QrwEmoticons::Emoticon & code );
    static QString GetEmoticonString( uint character );

protected Q_SLOTS:
    void onTextDocumentContentsChanged(int position, int charsRemoved, int charsAdded);

protected:
    explicit QrwEmoticonsPrivate(QrwEmoticons*, QTextDocument*);

    static const EmoticonsData EMOTICONS;
    static const int EmoticonTextFormatObjectType = QTextFormat::UserObject + 0xEEEE;

    bool isEmoticon( uint character );
    bool isEmoticon( const QrwEmoticons::Emoticon & code );

    QrwEmoticons::Emoticon getEmoticonCode(QTextCursor c, int* selectionLength);

    QString getDocumentText( bool html = false ) const;
    QString getCursorText( const QTextCursor & cursor, bool html = false ) const;

    void applyTextCharFormat(int pos = 0);

    bool loadPlugin(const QString & id = QString());

    int getLineHeight(int posInDocument, const QTextFormat & format) const;

protected:
    struct Plugin {
        QPointer<QrwEmoticonsPluginInterface> interf;
        QJsonObject metaData;
        void clear() {
            interf = Q_NULLPTR;
            metaData = QJsonObject();
        }
        operator bool() const {
            return interf != Q_NULLPTR;
        }
    } m_Plugin;

    QPluginLoader   m_PluginLoader;
    bool            m_CurrentlyApplying;
    QTextDocument*  m_TextDocument;
    quint8          m_MaxEmoticonCharCodeCount;
    QSize           m_MinimumEmoticonSize;
};

inline uint qHash(const QrwEmoticons::Emoticon &code, uint seed)
{
    return qHash(QrwEmoticonsPrivate::GetEmoticonString(code), seed);
}

#endif //QRWEMOTICONS_P_H
