#include "IOManager.h"

#include <QtConcurrent/QtConcurrent>
#include <QThread>

#include "piuio/IODevicePiuio.h"
#include "Log.h"

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

    IODevice* io_device = new IODevicePiuio();

    m_io_devices.push_back(io_device);

    for (auto io_device : m_io_devices) {
        // TODO if opening fails, do not add it to the list of devices to poll
        if (!io_device->open()) {
            Log::error(LOGMSG("Opening IO device %1 failed").arg(io_device->name()));
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

    for (auto io_device : m_io_devices) {
        if (!io_device->close()) {
            Log::error(LOGMSG("Closing IO device %1 failed").arg(io_device->name()));
        }

        delete io_device;
    }   

    Log::info(LOGMSG("Shutting down arcade IO manager finished"));
}

void IOManager::io_thread()
{
    while (m_loop_thread.load() > 0) {
        for (auto io_device : m_io_devices) {
            // TODO remove any devices that fail updating
            if (!io_device->update()) {
                Log::error(LOGMSG("Updating IO device %1 failed").arg(io_device->name()));
            }
        }

        QThread::msleep(100);
    }
}

} // namespace model