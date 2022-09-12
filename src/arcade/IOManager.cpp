#include "IOManager.h"

#include <cstring>
#include <dlfcn.h>

#include <QtConcurrent/QtConcurrent>
#include <QThread>

#include "../backend/Log.h"

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
    //IODevice* io_device = new IODevicePiuio();
    //auto io_device_state = new IODeviceState(io_device);

    Log::info(LOGMSG("Loading %1 IODevice libraries...").arg(m_ioDeviceLibraries.size()));

    for (auto ioDeviceLibraryPath : m_ioDeviceLibraries) {
        auto io_device = load_from_library(ioDeviceLibraryPath);

        if (io_device != nullptr) {
            auto io_device_state = new IODeviceState(io_device);

            m_io_device_states.push_back(io_device_state);
        } else {
            Log::warning(LOGMSG("Loading IODevice from library %1 failed, skipping").arg(ioDeviceLibraryPath));
        }
    }

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

IODevice* IOManager::load_from_library(const QString& path)
{
    typedef IODevice* (*create_io_device_t)(void);
    void* so = dlopen(path.toStdString().c_str(), RTLD_NOW);

	if (so == NULL) {
        Log::warning(LOGMSG("Ignoring library %1 that failed to load: %2").arg(path).arg(dlerror()));
        return nullptr;
    }

    create_io_device_t create_io_device = (create_io_device_t) dlsym(so, "create_io_device");

   if (create_io_device == nullptr) {
        Log::warning(LOGMSG("Could not find function 'create_io_device' in library %2").arg(path));
        dlclose(so);
        return nullptr;
    }

    return create_io_device();
}

} // namespace model