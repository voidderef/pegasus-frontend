#pragma once

#include <cstdbool>
#include <cstdint>

#include <QtPlugin>

#include "Input.h"

namespace arcade {

// TODO needs a proper logger interface to pass logger instance to this
class IODevice {
public:
    virtual const char* name() = 0;

    virtual bool open() = 0;
    virtual bool update() = 0;
    virtual bool close() = 0;

    virtual bool get_input(Input::Event input) = 0;

    virtual void set_output(uint16_t index, bool active) = 0;
};

}

Q_DECLARE_INTERFACE(arcade::IODevice, "org.djhackers.pegasus.IODevice")
