QT += gui widgets svg xml

INCLUDEPATH += $$PWD/include

# Removed all emojis but openmoji and circles

QMAKE_PRE_LINK += \
    cd $$PWD/lib && qmake && make; \
    cd $$PWD/plugin-openmoji && qmake && make

LIBS += \
    -L$$PWD/lib \
    -lQrwEmoticons \
    -L$$PWD/plugin-openmoji \
    -lemoticons_openmoji
