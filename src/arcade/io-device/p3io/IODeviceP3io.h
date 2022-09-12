#pragma once

#include "../IODevice.h"

#include <windows.h>

namespace model {

class IODeviceP3io : public IODevice {
public:
    IODeviceP3io();
    ~IODeviceP3io();

    const char* name() override;

    bool open() override;
    bool update() override;
    bool close() override;

    bool get_input(IODevice::Input input) override;
    void set_output(uint16_t index, bool active) override;

private:
    HANDLE m_handle;
    bool m_buffered_inputs[INPUT_TOTAL_COUNT];
    uint32_t m_prev_jamma[3];
};

}