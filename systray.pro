HEADERS       = window.h
SOURCES       = main.cpp \
                window.cpp
RESOURCES     = systray.qrc

QT += widgets
requires(qtConfig(combobox))

DISTFILES += \
    LICENSE

include(QSourceHighlite/QSourceHighlite.pri)

win32: DEFINES += QT_NO_TERMWIDGET

unix:!macx: CONFIG += link_pkgconfig
unix:!macx: PKGCONFIG += qtermwidget5
