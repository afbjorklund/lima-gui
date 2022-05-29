TEMPLATE = lib
QT += gui widgets svg

TARGET = QrwEmoticons

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DEFINES += QRWEMOTICONS_LIB

INCLUDEPATH += \
    ../include \
    ../include/QrwEmoticons

SOURCES += \
    src/QrwEmoticons.cpp \
    src/QrwEmoticons_p.cpp \
    src/QrwEmoticons_data.cpp \
    src/TextEdit.cpp \
    src/QrwEmoticonsTextObjectInterface.cpp

HEADERS += \
    ../include/QrwEmoticons/Global.h \
    ../include/QrwEmoticons/QrwEmoticonsPlugin.h \
    ../include/QrwEmoticons/QrwEmoticons.h \
    ../include/QrwEmoticons/TextEdit.h \
    src/QrwEmoticons_p.h \
    src/QrwEmoticonsTextObjectInterface_p.h

# install to build directory
target.path = ../build
INSTALLS += target
