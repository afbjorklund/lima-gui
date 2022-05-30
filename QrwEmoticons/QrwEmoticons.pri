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

pre-clean.commands = \
    cd $$PWD/lib && $(QMAKE) && $(MAKE) clean; \
    cd $$PWD/plugin-openmoji && $(QMAKE) && $(MAKE) clean
pre-distclean.commands = \
    cd $$PWD/lib && $(QMAKE) && $(MAKE) distclean; \
    cd $$PWD/plugin-openmoji && $(QMAKE) && $(MAKE) distclean
QMAKE_EXTRA_TARGETS += pre-clean pre-distclean
CLEAN_DEPS += pre-clean
DISTCLEAN_DEPS += pre-distclean
