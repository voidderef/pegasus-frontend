#pragma once

#include <vector>

#include <QObject>
#include <QFuture>
#include <QAtomicInt>

#include "IODevice.h"

namespace model {

class IOManager : public QObject {
    Q_OBJECT

public:
    IOManager();
    ~IOManager();

    // TODO search for libraries in a given folder to dynamically load IO devices
    void init();
    void shutdown();

private:
    std::vector<IODevice*> m_io_devices;

    QAtomicInt m_loop_thread;
    QFuture<void> m_io_thread;

    void io_thread();
};

} // namespace model
