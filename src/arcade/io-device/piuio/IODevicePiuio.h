#pragma once

#include "../../api/IODevice.h"

namespace arcade {

class IODevicePiuio : public QObject, public IODevice {
    Q_OBJECT
    Q_INTERFACES(arcade::IODevice)
    Q_PLUGIN_METADATA(IID "org.djhackers.pegasus.IODevicePiuio")

public:
    IODevicePiuio();
    ~IODevicePiuio();

    const char* name() override;

    bool open() override;
    bool update() override;
    bool close() override;

    bool get_input(Input::Event input) override;
    void set_output(uint16_t index, bool active) override;

private:
    void* m_handle;
    bool m_buffered_inputs[Input::Event::INPUT_EVENT_TOTAL_COUNT];
};

} // namespace arcade