TEMPLATE = lib
TARGET = io-piuio

QT += qml quick sql
CONFIG += plugin c++11 warn_on exceptions_off

LIBS += -lusb-1.0

SOURCES += \
    $$PWD/IODevicePiuio.cpp \
    $$PWD/main.cpp \
    $$PWD/piuio-usb.c \
    $$PWD/usb.c

HEADERS += \
    $$PWD/IODevicePiuio.h \
    $$PWD/piuio-usb.h \
    $$PWD/piuio.h \
    $$PWD/result.h

include(../../api/api.pri)

DEFINES *= $${COMMON_DEFINES}
