#pragma once

#include <QObject>
#include <QtPlugin>
#include <QQmlEngine>

#include "IOManager.h"

#include <iostream>

namespace arcade {

class Arcade : public QObject {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.djhackers.pegasus.Arcade")

    Q_PROPERTY(IOManager* ioManager READ ioManager WRITE setIoManager CONSTANT)

public:
    IOManager* ioManager() const {
        return m_ioManager;
    }

    void setIoManager(IOManager* ioManager) {
        std::cout << "asdfasdasd" << std::endl;
        m_ioManager = ioManager;
    }

private:
    IOManager* m_ioManager;
};

} // namespace arcade
