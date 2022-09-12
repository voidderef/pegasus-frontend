TEMPLATE = lib

CONFIG += c++11 staticlib warn_on exceptions_off

SOURCES += \
    $$PWD/Arcade.cpp \
    $$PWD/IODeviceState.cpp \
    $$PWD/IOManager.cpp

HEADERS += \
    $$PWD/Arcade.h \
    $$PWD/IODeviceState.h \
    $$PWD/IOManager.h \
    $$PWD/../api/IODevice.h

include($$PWD/../api/api.pri)