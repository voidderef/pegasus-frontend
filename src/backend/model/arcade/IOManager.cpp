#include "IOManager.h"

#include <cstring>

#include <QtConcurrent/QtConcurrent>
#include <QThread>

#include "piuio/IODevicePiuio.h"
#include "Log.h"

#include <QGuiApplication>
#include <QKeyEvent>
#include <QTimer>
#include <QWindow>

namespace model {

IOManager::IOManager()
{

}

IOManager::~IOManager()
{

}

void IOManager::init()
{
    Log::info(LOGMSG("Initializing arcade IO manager..."));

    // TODO static for now, make dynamic with library loading etc later
    IODevice* io_device = new IODevicePiuio();
    auto io_device_state = new IODeviceState(io_device);

    m_io_device_states.push_back(io_device_state);

    for (auto io_device_state : m_io_device_states) {
        // TODO if opening fails, do not add it to the list of devices to poll
        if (!io_device_state->m_device->open()) {
            Log::error(LOGMSG("Opening IO device %1 failed").arg(io_device_state->m_device->name()));
        }
    } 

    m_loop_thread.store(1);
    m_io_thread = QtConcurrent::run(this, &IOManager::io_thread);

    while (!m_io_thread.isRunning()) {
        QThread::msleep(100);
    }

    Log::info(LOGMSG("Initializing arcade IO manager finished"));
}

void IOManager::shutdown()
{
    Log::info(LOGMSG("Shutting down arcade IO manager..."));

    m_loop_thread.store(0);

    while (!m_io_thread.isRunning()) {
        QThread::msleep(100);
    }

    for (auto io_device_state : m_io_device_states) {
        if (!io_device_state->m_device->close()) {
            Log::error(LOGMSG("Closing IO device %1 failed").arg(io_device_state->m_device->name()));
        }

        delete io_device_state;
    }   

    Log::info(LOGMSG("Shutting down arcade IO manager finished"));
}

void IOManager::io_thread()
{
    while (m_loop_thread.load() > 0) {
        for (auto io_device_state : m_io_device_states) {
            io_device_state->input_buffer_shift();

            // TODO remove any devices that fail updating
            if (!io_device_state->m_device->update()) {
                Log::error(LOGMSG("Updating IO device %1 failed").arg(io_device_state->m_device->name()));
            }

            io_device_state->capture_current_input_states();

            // TODO emit events to frontend

            // TODO this is super hacky right now as i have no idea how and if i can even emit
            // abstract "events" that are not either gamepad or keyboard events to the QT framework
            
            
            for (int i = 0; i < static_cast<int>(IODevice::Input::TOTAL_COUNT); i++) {
                if (io_device_state->is_pressed(static_cast<IODevice::Input>(i))) {
                    QWindow* const focus_window = qApp ? qApp->focusWindow() : nullptr;

                    if (focus_window) {
                        int keycode = event_to_qt_keyboard_key(static_cast<IODevice::Input>(i));
                        QKeyEvent event(QEvent::KeyPress, keycode, Qt::NoModifier);
                        QGuiApplication::sendEvent(focus_window, &event);
                    }
                } else if (io_device_state->is_released(static_cast<IODevice::Input>(i))) {
                    QWindow* const focus_window = qApp ? qApp->focusWindow() : nullptr;
                    
                    if (focus_window) {
                        int keycode = event_to_qt_keyboard_key(static_cast<IODevice::Input>(i));
                        QKeyEvent event(QEvent::KeyRelease, keycode, Qt::NoModifier);
                        QGuiApplication::sendEvent(focus_window, &event);
                    }
                }
            }
        }

        QThread::msleep(100);
    }
}

int IOManager::event_to_qt_keyboard_key(IODevice::Input input)
{
    switch (input) {
        case IODevice::Input::ACCEPT:
            return Qt::Key_Enter;
        case IODevice::Input::CANCEL:
            return Qt::Key_Escape;
        case IODevice::Input::LEFT:
            return Qt::Key_Left;
        case IODevice::Input::RIGHT:
            return Qt::Key_Right;
        case IODevice::Input::UP:
            return Qt::Key_Up;
        case IODevice::Input::DOWN:
            return Qt::Key_Down;
        case IODevice::Input::DETAILS:
        case IODevice::Input::FILTERS:
        case IODevice::Input::NEXT_PAGE:
        case IODevice::Input::PREV_PAGE:
        case IODevice::Input::PAGE_UP:
        case IODevice::Input::PAGE_DOWN:
        case IODevice::Input::MAIN_MENU:
        default:
            // Unsupported
            return 0;
    }
}

IODeviceState::IODeviceState(IODevice* device) :
    m_device(device)
{
    memset(&m_buffer_prev_inputs, 0, sizeof(m_buffer_prev_inputs));
}

IODeviceState::~IODeviceState()
{
    delete m_device;
}

void IODeviceState::capture_current_input_states()
{
    for (int i = 0; i < static_cast<int>(IODevice::Input::TOTAL_COUNT); i++) {
        m_buffer_cur_inputs[i] = m_device->is_input_active(static_cast<IODevice::Input>(i));
    }
}

bool IODeviceState::is_pressed(IODevice::Input input) const
{
    int idx = static_cast<int>(input);

    Q_ASSERT(idx < static_cast<int>(IODevice::Input::TOTAL_COUNT));

    return !m_buffer_prev_inputs[idx] && m_buffer_cur_inputs[idx];
}

bool IODeviceState::is_released(IODevice::Input input) const
{
    int idx = static_cast<int>(input);

    Q_ASSERT(idx < static_cast<int>(IODevice::Input::TOTAL_COUNT));

    return m_buffer_prev_inputs[idx] && !m_buffer_cur_inputs[idx];
}

void IODeviceState::input_buffer_shift()
{
    memcpy(&m_buffer_prev_inputs, &m_buffer_cur_inputs, sizeof(m_buffer_prev_inputs));
    memset(&m_buffer_cur_inputs, 0, sizeof(m_buffer_cur_inputs));
}

} // namespace model