TEMPLATE = lib
TARGET = arcadebackend

QT += qml quick sql
CONFIG += plugin c++11 warn_on exceptions_off

SOURCES += \
    $$PWD/Arcade.cpp \
    $$PWD/IODeviceState.cpp \
    $$PWD/IOManager.cpp \
    $$PWD/main.cpp

HEADERS += \
    $$PWD/Arcade.h \
    $$PWD/ArcadePlugin.h \
    $$PWD/IODeviceState.h \
    $$PWD/IOManager.h \
    $$PWD/Log.h

include(../api/api.pri)

DEFINES *= $${COMMON_DEFINES}
