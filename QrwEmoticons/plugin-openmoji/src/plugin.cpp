#include "plugin.h"
#include <QrwEmoticonsHelper.h>

EmoticonOpenmojiPlugin::EmoticonOpenmojiPlugin(QObject* parent)
    : QrwEmoticonsPluginInterface(parent)
{
    //Q_INIT_RESOURCE(emoticons_openmoji_X);
}

EmoticonOpenmojiPlugin::~EmoticonOpenmojiPlugin()
{
    //Q_CLEANUP_RESOURCE(emoticons_openmoji_X);
}

QString EmoticonOpenmojiPlugin::getEmoticonPath(const QrwEmoticons::Emoticon &code) const
{
    return QStringLiteral(":/QrwEmoticons/openmoji/") % EMOTICON_FILENAME(code) % QStringLiteral(".svg");
}

bool EmoticonOpenmojiPlugin::hasEmoticon(const QrwEmoticons::Emoticon & code)
{
    const QString fileName = this->getEmoticonPath(code);
    return QFile::exists(fileName);
}

QPixmap EmoticonOpenmojiPlugin::getEmoticon(const QrwEmoticons::Emoticon & code, const QSize & size)
{
    const QString fileName = this->getEmoticonPath(code);
    QPixmap pix = QrwEmoticonsHelper::SvgToPixmap(fileName, size);
    return pix;
}
