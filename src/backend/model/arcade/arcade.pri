HEADERS += \
    $$PWD/Arcade.h \
    $$PWD/IODevice.h \
    $$PWD/IODeviceState.h \
    $$PWD/IOManager.h

SOURCES += \
    $$PWD/Arcade.cpp \
    $$PWD/IODeviceState.cpp \
    $$PWD/IOManager.cpp

include(piuio/piuio.pri)
