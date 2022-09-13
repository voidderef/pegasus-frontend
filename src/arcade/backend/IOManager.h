#pragma once

#include <vector>

#include <QObject>
#include <QFuture>
#include <QAtomicInt>
#include <QQmlParserStatus>

#include "../api/IODevice.h"
#include "IODeviceState.h"

namespace arcade {

class IOManager : public QObject, public QQmlParserStatus {
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY(QVector<QString> ioDevices READ ioDevices WRITE setIoDevices CONSTANT)

public:
    void classBegin() override;
    void componentComplete() override;

public:
    QVector<QString> ioDevices() const {
        return m_ioDeviceLibraries;
    }

    void setIoDevices(const QVector<QString>& ioDevices) {
        m_ioDeviceLibraries = ioDevices;
    }

signals:
    // TODO this can be further improved to also keep track of how long the button is pressed already
    void inputPressed(Input::Event input);
    void inputReleased(Input::Event input);
    void inputHeld(Input::Event input);

private:
    std::vector<IODeviceState*> m_io_device_states;

    QAtomicInt m_loop_thread;
    QFuture<void> m_io_thread;
    QVector<QString> m_ioDeviceLibraries;

    void init();
    void shutdown();

    void io_thread();
};

} // namespace arcade
