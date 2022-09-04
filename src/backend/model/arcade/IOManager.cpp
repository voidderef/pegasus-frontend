#include "IOManager.h"

#include <QtConcurrent/QtConcurrent>
#include <QThread>

#include "IODevice.h"
#include "Log.h"

namespace model {

IOManager::IOManager()
{

}

void IOManager::init()
{
    Log::info("Initializing arcade IO manager...");

    auto io_device = new IODevice();

    m_io_devices.push_back(io_device);

    m_loop_thread.store(1);
    m_io_thread = QtConcurrent::run(this, &IOManager::io_thread);

    while (!m_io_thread.isRunning()) {
        QThread::msleep(100);
    }

    Log::info("Initializing arcade IO manager finished");
}

void IOManager::shutdown()
{
    Log::info("Shutting down arcade IO manager...");

    m_loop_thread.store(0);

    while (!m_io_thread.isRunning()) {
        QThread::msleep(100);
    }

    Log::info("Shutting down arcade IO manager finished");
}

void IOManager::io_thread()
{
    while (m_loop_thread.load() > 0) {
        for (auto io_device : m_io_devices) {
            io_device->update();
        }

        QThread::msleep(100);
    }
}

} // namespace model