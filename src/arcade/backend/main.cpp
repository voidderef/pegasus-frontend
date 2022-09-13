#include <QObject>
#include <QtPlugin>
#include <QQmlEngine>

#include "Arcade.h"
#include "Log.h"

#include <iostream>

void __attribute__ ((constructor)) lib_init()
{
    constexpr auto API_URI = "Arcade";

    qmlRegisterType<arcade::Arcade>(API_URI, 0, 1, "Arcade");
    qmlRegisterType<arcade::IOManager>(API_URI, 0, 1, "IOManager");

    LOG_INFO("Arcade backend lib main initialized");
}