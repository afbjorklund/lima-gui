HEADERS       = window.h \
                lima.h \
                instance.h \
                examples.h \
                container.h \
                image.h
SOURCES       = main.cpp \
                instance.cpp \
                examples.cpp \
                container.cpp \
                image.cpp \
                lima.cpp \
                window.cpp
RESOURCES     = systray.qrc

QT += widgets
requires(qtConfig(combobox))

TARGET        = lima-gui

macx: ICON    = icons/tux.icns

DISTFILES += \
    LICENSE

include(QSourceHighlite/QSourceHighlite.pri)
include(QrwEmoticons/QrwEmoticons.pri)

# Enabling qtermwidget requires GPL-v2 license
#CONFIG += gpl_licensed

gpl_licensed {
  win32: DEFINES += QT_NO_TERMWIDGET

  unix: CONFIG += link_pkgconfig
  unix: PKGCONFIG += qtermwidget5
} else {
  DEFINES += QT_NO_TERMWIDGET
}
