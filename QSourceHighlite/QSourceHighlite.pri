QT += gui

INCLUDEPATH += $$PWD

# Removed all languages but bash and yaml

HEADERS += $$PWD/qsourcehighliter.h \
           $$PWD/qsourcehighliterthemes.h \
           $$PWD/languagedata.h

SOURCES += $$PWD/qsourcehighliter.cpp \
    $$PWD/languagedata.cpp \
    $$PWD/qsourcehighliterthemes.cpp
