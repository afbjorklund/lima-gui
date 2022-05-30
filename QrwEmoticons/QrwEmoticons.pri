QT += gui widgets svg xml

INCLUDEPATH += $$PWD/include

# Removed all emojis but openmoji and circles

QMAKE_PRE_LINK += \
    cd $$PWD/lib && $(QMAKE) && $(MAKE); \
    cd $$PWD/plugin-openmoji && $(QMAKE) && $(MAKE)

LIBS += \
    -L$$PWD/lib \
    -lQrwEmoticons \
    -L$$PWD/plugin-openmoji \
    -lemoticons_openmoji
