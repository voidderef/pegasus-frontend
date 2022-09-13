#include <QObject>
#include <QtPlugin>
#include <QQmlEngine>

#include "IODevicePiuio.h"

void __attribute__ ((constructor)) lib_init()
{
    constexpr auto API_URI = "Arcade.Io.Piuio";

    qmlRegisterType<arcade::IODevicePiuio>(API_URI, 0, 1, "IODevicePiuio");
    qRegisterMetaType<arcade::IODevicePiuio::Input>("IODevicePiuio::Input");
    qRegisterMetaType<arcade::IODevicePiuio::Output>("IODevicePiuio::Output");
}