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

IOManager::IOManager(QObject* parent)
    : QObject(parent)
{
    init();
}

IOManager::~IOManager()
{
    shutdown();
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

            for (int i = 0; i < IODevice::INPUT_TOTAL_COUNT; i++) {
                auto arcade_input = static_cast<IODevice::Input>(i);

                if (io_device_state->inputIsPressed(arcade_input)) {
                    emit inputPressed(arcade_input);
                } else if (io_device_state->inputIsReleased(arcade_input)) {
                    emit inputReleased(arcade_input);
                } else if (io_device_state->inputIsHeld(arcade_input)) {
                    emit inputHeld(arcade_input);
                }
            }
        }

        emit inputPressed(IODevice::INPUT_SERVICE);

        // TODO make configurable/sane default to avoid banging the CPU?
        QThread::msleep(1000);
    }
}

} // namespace model