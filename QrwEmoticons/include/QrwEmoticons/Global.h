#ifndef QRWEMOTICONS_GLOBAL_H
#define QRWEMOTICONS_GLOBAL_H

#include <QtCore/qglobal.h>

#define NOT !

/*
 * LIB MACROS
 */

#if defined(QRWEMOTICONS_LIB)
#  define QRWEMOTICONS_EXPORT Q_DECL_EXPORT
#else
#  define QRWEMOTICONS_EXPORT Q_DECL_IMPORT
#endif

#define QRWEMOTICONS_DECLARE_PUBLIC(CLASS) \
    CLASS * q_ptr; \
    Q_DECLARE_PUBLIC(CLASS)

#define QRWEMOTICONS_DECLARE_PRIVATE(CLASS) \
    CLASS##Private* p_ptr; \
    Q_DECLARE_PRIVATE_D(p_ptr, CLASS)

/*
 * HELPER MACROS
 */

#define EMOTICON_FILENAME(E) \
    ( \
        E.size() == 1 ? EMOTICON_FILENAME_1(E) : \
        E.size() == 2 ? EMOTICON_FILENAME_2(E) : \
        E.size() == 3 ? EMOTICON_FILENAME_3(E) : \
        E.size() == 4 ? EMOTICON_FILENAME_4(E) : \
        E.size() == 5 ? EMOTICON_FILENAME_5(E) : \
        E.size() == 6 ? EMOTICON_FILENAME_6(E) : \
        E.size() == 7 ? EMOTICON_FILENAME_7(E) : \
        E.size() == 8 ? EMOTICON_FILENAME_8(E) : \
        QString::null \
    )

#define EMOTICON_FILENAME_1(E) \
    QStringLiteral("%1") \
    .arg(E[0], 5, 16, QLatin1Char('0'))

#define EMOTICON_FILENAME_2(E) \
    QStringLiteral("%1-%2") \
    .arg(E[0], 5, 16, QLatin1Char('0')) \
    .arg(E[1], 5, 16, QLatin1Char('0'))

#define EMOTICON_FILENAME_3(E) \
    QStringLiteral("%1-%2-%3") \
    .arg(E[0], 5, 16, QLatin1Char('0')) \
    .arg(E[1], 5, 16, QLatin1Char('0')) \
    .arg(E[2], 5, 16, QLatin1Char('0'))

#define EMOTICON_FILENAME_4(E) \
    QStringLiteral("%1-%2-%3-%4") \
    .arg(E[0], 5, 16, QLatin1Char('0')) \
    .arg(E[1], 5, 16, QLatin1Char('0')) \
    .arg(E[2], 5, 16, QLatin1Char('0')) \
    .arg(E[3], 5, 16, QLatin1Char('0'))

#define EMOTICON_FILENAME_5(E) \
    QStringLiteral("%1-%2-%3-%4-%5") \
    .arg(E[0], 5, 16, QLatin1Char('0')) \
    .arg(E[1], 5, 16, QLatin1Char('0')) \
    .arg(E[2], 5, 16, QLatin1Char('0')) \
    .arg(E[3], 5, 16, QLatin1Char('0')) \
    .arg(E[4], 5, 16, QLatin1Char('0'))

#define EMOTICON_FILENAME_6(E) \
    QStringLiteral("%1-%2-%3-%4-%5-%6") \
    .arg(E[0], 5, 16, QLatin1Char('0')) \
    .arg(E[1], 5, 16, QLatin1Char('0')) \
    .arg(E[2], 5, 16, QLatin1Char('0')) \
    .arg(E[3], 5, 16, QLatin1Char('0')) \
    .arg(E[4], 5, 16, QLatin1Char('0')) \
    .arg(E[5], 5, 16, QLatin1Char('0'))

#define EMOTICON_FILENAME_7(E) \
    QStringLiteral("%1-%2-%3-%4-%5-%6-%7") \
    .arg(E[0], 5, 16, QLatin1Char('0')) \
    .arg(E[1], 5, 16, QLatin1Char('0')) \
    .arg(E[2], 5, 16, QLatin1Char('0')) \
    .arg(E[3], 5, 16, QLatin1Char('0')) \
    .arg(E[4], 5, 16, QLatin1Char('0')) \
    .arg(E[5], 5, 16, QLatin1Char('0')) \
    .arg(E[6], 5, 16, QLatin1Char('0'))

#define EMOTICON_FILENAME_8(E) \
    QStringLiteral("%1-%2-%3-%4-%5-%6-%7-%8") \
    .arg(E[0], 5, 16, QLatin1Char('0')) \
    .arg(E[1], 5, 16, QLatin1Char('0')) \
    .arg(E[2], 5, 16, QLatin1Char('0')) \
    .arg(E[3], 5, 16, QLatin1Char('0')) \
    .arg(E[4], 5, 16, QLatin1Char('0')) \
    .arg(E[5], 5, 16, QLatin1Char('0')) \
    .arg(E[6], 5, 16, QLatin1Char('0')) \
    .arg(E[7], 5, 16, QLatin1Char('0'))

#endif // QRWEMOTICONS_GLOBAL_H
