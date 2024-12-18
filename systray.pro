HEADERS       = window.h \
                lima.h \
                templates.h \
                instance.h
SOURCES       = main.cpp \
                instance.cpp \
                templates.cpp \
                lima.cpp \
                window.cpp
RESOURCES     = systray.qrc

QT += widgets
requires(qtConfig(combobox))

TARGET        = lima-gui

macx: ICON    = icons/tux.icns

DISTFILES += \
    LICENSE

//DEFINES += QT_NO_SOURCEHIGHLITER
include(QSourceHighlite/QSourceHighlite.pri)

//DEFINES += QT_NO_EMOTICONS
include(QrwEmoticons/QrwEmoticons.pri)

//DEFINES += QT_NO_VNCCLIENT
include(QVNCClient/QVNCClient.pri)

# Enabling qtermwidget requires GPL-v2 license
#CONFIG += gpl_licensed

gpl_licensed {
  win32: DEFINES += QT_NO_TERMWIDGET

  unix: CONFIG += link_pkgconfig
  unix: PKGCONFIG += qtermwidget5
} else {
  DEFINES += QT_NO_TERMWIDGET

  win32: DEFINES += QT_NO_EMOTICONS
}
