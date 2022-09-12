TEMPLATE = lib
TARGET = arcadebackend

QT += qml quick sql
CONFIG += plugin c++11 warn_on exceptions_off

SOURCES += \
    $$PWD/ArcadePlugin.cpp \
    $$PWD/IODeviceState.cpp \
    $$PWD/IOManager.cpp

HEADERS += \
    $$PWD/ArcadeInterface.h \
    $$PWD/ArcadePlugin.h \
    $$PWD/IODeviceState.h \
    $$PWD/IOManager.h

include(../api/api.pri)

DEFINES *= $${COMMON_DEFINES}
