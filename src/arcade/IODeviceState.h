#pragma once

#include <QObject>
#include <QFuture>
#include <QAtomicInt>

#include "api/IODevice.h"

namespace model {

class IODeviceState : public QObject {
public:
    IODeviceState(IODevice* device);
    ~IODeviceState();

    void capture_current_input_states();

    bool inputIsPressed(IODevice::Input input) const;

    bool inputIsReleased(IODevice::Input input) const;

    bool inputIsHeld(IODevice::Input input) const;

    void input_buffer_shift();

    IODevice* m_device;
    bool m_buffer_cur_inputs[IODevice::INPUT_TOTAL_COUNT];
    bool m_buffer_prev_inputs[IODevice::INPUT_TOTAL_COUNT];
};

} // namespace model