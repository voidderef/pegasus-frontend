#pragma once

#include <vector>

#include <QObject>
#include <QFuture>
#include <QAtomicInt>

#include "IODevice.h"
#include "IODeviceState.h"

namespace model {

class IOManager : public QObject {
    Q_OBJECT

public:
    explicit IOManager(QObject* parent = nullptr);
    ~IOManager();

signals:
    // TODO this can be further improved to also keep track of how long the button is pressed already
    void inputPressed(IODevice::Input input);
    void inputReleased(IODevice::Input input);
    void inputHeld(IODevice::Input input);

private:
    std::vector<IODeviceState*> m_io_device_states;

    QAtomicInt m_loop_thread;
    QFuture<void> m_io_thread;

    // TODO search for libraries in a given folder to dynamically load IO devices
    void init();
    void shutdown();

    void io_thread();
};

} // namespace model
