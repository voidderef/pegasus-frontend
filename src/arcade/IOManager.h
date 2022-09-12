#pragma once

#include <vector>

#include <QObject>
#include <QFuture>
#include <QAtomicInt>

#include "api/IODevice.h"
#include "IODeviceState.h"

namespace model {

class IOManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVector<QString> ioDeviceLibraries READ ioDeviceLibraries WRITE setIoDeviceLibraries)

public:
    explicit IOManager(QObject* parent = nullptr);
    ~IOManager();


    QVector<QString> ioDeviceLibraries() const {
        return m_ioDeviceLibraries;
    }

    void setIoDeviceLibraries(const QVector<QString>& ioDeviceLibraries) {
        m_ioDeviceLibraries = ioDeviceLibraries;
    }

signals:
    // TODO this can be further improved to also keep track of how long the button is pressed already
    void inputPressed(IODevice::Input input);
    void inputReleased(IODevice::Input input);
    void inputHeld(IODevice::Input input);

private:
    std::vector<IODeviceState*> m_io_device_states;

    QAtomicInt m_loop_thread;
    QFuture<void> m_io_thread;
    QVector<QString> m_ioDeviceLibraries;

    // TODO search for libraries in a given folder to dynamically load IO devices
    void init();
    void shutdown();

    void io_thread();

    IODevice* load_from_library(const QString& path);
};

} // namespace model
