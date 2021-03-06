TEMPLATE = lib
CONFIG += plugin
CONFIG += static
QT += core svg xml

TARGET = $$qtLibraryTarget(emoticons_openmoji)

RESOURCES += \
    $$files(*.qrc)

INCLUDEPATH += \
    ../include \
    ../shared/src

HEADERS = \
    src/plugin.h \
    ../shared/src/QrwEmoticonsHelper.h

SOURCES = \
    src/plugin.cpp \
    ../shared/src/QrwEmoticonsHelper.cpp

OTHER_FILES = \
    plugin.json
