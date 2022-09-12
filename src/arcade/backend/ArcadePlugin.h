#pragma once

#include <iostream>

#include <QObject>
#include <QtPlugin>
#include <QQmlEngine>

#include "ArcadeInterface.h"

namespace arcade {

class ArcadePlugin : public QObject, public ArcadeInterface {
    Q_OBJECT
    Q_INTERFACES(arcade::ArcadeInterface)
    Q_PLUGIN_METADATA(IID "org.djhackers.pegasus.Arcade")

    Q_PROPERTY(IOManager* ioManager READ ioManager WRITE setIoManager CONSTANT)

public:
    void init() override {
        constexpr auto API_URI = "Arcade";

        std::cout << "Arcade init" << std::endl;

        //qmlRegisterType<arcade::Arcade>(API_URI, 0, 14, "Arcade");
        qmlRegisterType<arcade::IOManager>(API_URI, 0, 1, "IOManager");
        qRegisterMetaType<arcade::Input::Event>("Input::Event");
    }

    IOManager* ioManager() const override {
        return m_ioManager;
    }

    void setIoManager(IOManager* ioManager) override {
        m_ioManager = ioManager;
    }

private:
    IOManager* m_ioManager;
};

} // namespace arcade
