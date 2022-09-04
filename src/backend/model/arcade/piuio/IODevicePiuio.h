#pragma once

#include "../IODevice.h"

namespace model {

class IODevicePiuio : public IODevice {
public:
    IODevicePiuio();
    ~IODevicePiuio();

    const char* name() override;

    bool open() override;
    bool update() override;
    bool close() override;

    bool is_input_active(IODevice::Input input) override;
    void set_output(uint16_t index, bool active) override;

private:
    void* m_handle;
    bool m_buffered_inputs[static_cast<int>(IODevice::Input::TOTAL_COUNT)];
};

}