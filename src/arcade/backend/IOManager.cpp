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

    LOG_DEBUG("Total IO devices: %ld", m_ioDevices.size());

    QVector<IODevice*> opened_io_devices;

    for (auto io_device : m_ioDevices) {
        LOG_DEBUG("Opening IO device: %s", io_device->name());

        if (!io_device->open()) {
            LOG_ERROR("Failed opening IO device: %s", io_device->name());
            continue;
        }

        opened_io_devices.push_back(io_device);
    }

    m_ioDevices = opened_io_devices;

    LOG_INFO("Opened %ld io devices successfully", m_ioDevices.size());

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

    for (auto io_device : m_ioDevices) {
        LOG_DEBUG("Closing IO device: %s", io_device->name());

        if (!io_device->close()) {
            LOG_WARN("Failed closing IO device: %s", io_device->name());
        }

        delete io_device;
    }

    m_ioDevices.clear();

    LOG_INFO("Arcade IO manager shutting down completed");
}

void IOManager::io_thread()
{
    LOG_DEBUG("IO thread running");

    while (m_loop_thread.load() > 0) {
        for (auto io_device : m_ioDevices) {
            // TODO remove any devices that fail updating
            if (!io_device->update()) {
                LOG_ERROR("Failed updating IO device: %s", io_device->name());
            }
        }

        // TODO make configurable/sane default to avoid banging the CPU?
        QThread::msleep(1000);
    }

    LOG_DEBUG("IO thread stopped");
}

} // namespace arcade
