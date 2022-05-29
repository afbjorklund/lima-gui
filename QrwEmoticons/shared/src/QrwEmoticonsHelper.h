#ifndef QRWEMOTICONS_HELPER_H
#define QRWEMOTICONS_HELPER_H

#include <QtCore>

namespace QrwEmoticonsHelper
{
    QByteArray SvgPrepare(const QString & fileName);
    QPixmap SvgToPixmap(const QString & fileName, const QSize & size);
};

#endif //QRWEMOTICONS_HELPER_H
