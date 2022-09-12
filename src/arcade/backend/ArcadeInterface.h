#pragma once

#include <QtPlugin>

#include "IOManager.h"

namespace arcade {

class ArcadeInterface {
public:
    virtual void init() = 0;

    virtual IOManager* ioManager() const = 0;
    virtual void setIoManager(IOManager* ioManager) = 0;
};

}

Q_DECLARE_INTERFACE(arcade::ArcadeInterface, "org.djhackers.pegasus.ArcadeInterface")
