#pragma once

#include <QObject>
#include <QFuture>
#include <QAtomicInt>

#include "../api/IODevice.h"

namespace arcade {

class IODeviceState : public QObject {
public:
    IODeviceState(IODevice* device);
    ~IODeviceState();

    void capture_current_input_states();

    bool inputIsPressed(Input::Event input) const;

    bool inputIsReleased(Input::Event input) const;

    bool inputIsHeld(Input::Event input) const;

    void input_buffer_shift();

    IODevice* m_device;
    bool m_buffer_cur_inputs[Input::INPUT_EVENT_TOTAL_COUNT];
    bool m_buffer_prev_inputs[Input::INPUT_EVENT_TOTAL_COUNT];
};

} // namespace arcade
