#include "IOManager.h"

#include <iostream>
#include <cstring>
#include <dlfcn.h>

#include <QtConcurrent/QtConcurrent>
#include <QThread>

#include <QGuiApplication>
#include <QKeyEvent>
#include <QTimer>
#include <QWindow>
#include <QtPlugin>

#include "Log.h"

namespace arcade {

void IOManager::classBegin()
{
    // noop
}

void IOManager::componentComplete()
{
    init();
}

void IOManager::init()
{
    LOG_INFO("Arcade IO manager initializing...");

    for (auto ioDeviceLibrary : m_ioDeviceLibraries) {
        LOG_DEBUG("Loading IO device library: %s", ioDeviceLibrary.toStdString().c_str());

        QPluginLoader loader(ioDeviceLibrary);

        if (loader.load()) {
            arcade::IODevice *io_device = qobject_cast<arcade::IODevice*>(loader.instance());

            auto io_device_state = new IODeviceState(io_device);
            m_io_device_states.push_back(io_device_state);

            LOG_INFO("Loaded IO device library: %s", ioDeviceLibrary.toStdString().c_str());
        } else {
            LOG_ERROR("Failed loading IO device library: %s", ioDeviceLibrary.toStdString().c_str());
        }
    }

    for (auto io_device_state : m_io_device_states) {
        LOG_DEBUG("Opening IO device: %s", io_device_state->m_device->name());

        // TODO if opening fails, do not add it to the list of devices to poll
        if (!io_device_state->m_device->open()) {
            LOG_ERROR("Failed opening IO device: %s", io_device_state->m_device->name());
        }
    }

    LOG_DEBUG("Starting IO thread...");

    m_loop_thread.store(1);
    m_io_thread = QtConcurrent::run(this, &IOManager::io_thread);

    while (!m_io_thread.isRunning()) {
        QThread::msleep(100);
    }

    LOG_DEBUG("Arcade IO manager initialized");
}

void IOManager::shutdown()
{
    LOG_INFO("Arcade IO manager shutting down...");

    LOG_DEBUG("Waiting for IO thread to stop...");

    m_loop_thread.store(0);

    while (!m_io_thread.isRunning()) {
        QThread::msleep(100);
    }

    LOG_DEBUG("Closing IO devices...");

    for (auto io_device_state : m_io_device_states) {
        LOG_DEBUG("Closing IO device: %s", io_device_state->m_device->name());

        if (!io_device_state->m_device->close()) {
            LOG_WARN("Failed closing IO device: %s", io_device_state->m_device->name());
        }

        delete io_device_state;
    }   

    LOG_INFO("Arcade IO manager shutting down completed");
}

void IOManager::io_thread()
{
    LOG_DEBUG("IO thread running");

    while (m_loop_thread.load() > 0) {
        for (auto io_device_state : m_io_device_states) {
            io_device_state->input_buffer_shift();

            // TODO remove any devices that fail updating
            if (!io_device_state->m_device->update()) {
                LOG_ERROR("Failed updating IO device: %s", io_device_state->m_device->name());
            }

            io_device_state->capture_current_input_states();

            for (int i = 0; i < Input::Event::INPUT_EVENT_TOTAL_COUNT; i++) {
                auto arcade_input = static_cast<Input::Event>(i);

                if (io_device_state->inputIsPressed(arcade_input)) {
                    emit inputPressed(arcade_input);
                } else if (io_device_state->inputIsReleased(arcade_input)) {
                    emit inputReleased(arcade_input);
                } else if (io_device_state->inputIsHeld(arcade_input)) {
                    emit inputHeld(arcade_input);
                }
            }
        }

        // TODO make configurable/sane default to avoid banging the CPU?
        QThread::msleep(1000);
    }

    LOG_DEBUG("IO thread stopped");
}

} // namespace model
