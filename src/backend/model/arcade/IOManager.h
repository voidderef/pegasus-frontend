#pragma once

#include <vector>

#include <QObject>
#include <QFuture>
#include <QAtomicInt>

#include "IODevice.h"

namespace model {

class IODeviceState {
public:
    IODeviceState(IODevice* device);
    ~IODeviceState();

    void capture_current_input_states();

    bool is_pressed(IODevice::Input input) const;

    bool is_released(IODevice::Input input) const;

    void input_buffer_shift();

    IODevice* m_device;
    bool m_buffer_cur_inputs[static_cast<int>(IODevice::Input::TOTAL_COUNT)];
    bool m_buffer_prev_inputs[static_cast<int>(IODevice::Input::TOTAL_COUNT)];
};

class IOManager : public QObject {
    Q_OBJECT

public:
    IOManager();
    ~IOManager();

    // TODO search for libraries in a given folder to dynamically load IO devices
    void init();
    void shutdown();

private:
    std::vector<IODeviceState*> m_io_device_states;

    QAtomicInt m_loop_thread;
    QFuture<void> m_io_thread;

    void io_thread();
    
    static int event_to_qt_keyboard_key(IODevice::Input input);
};

} // namespace model
