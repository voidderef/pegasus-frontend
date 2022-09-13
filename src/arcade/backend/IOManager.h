#pragma once

#include <vector>

#include <QObject>
#include <QFuture>
#include <QAtomicInt>
#include <QQmlParserStatus>

#include "../api/IODevice.h"

namespace arcade {

class IOManager : public QObject, public QQmlParserStatus {
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY(QVector<IODevice*> ioDevices READ ioDevices WRITE setIoDevices CONSTANT)

public:
    void classBegin() override;
    void componentComplete() override;

public:
    QVector<IODevice*> ioDevices() const {
        return m_ioDevices;
    }

    void setIoDevices(const QVector<IODevice*>& ioDevices) {
        m_ioDevices = ioDevices;
    }

private:
    QVector<IODevice*> m_ioDevices;

private:
    QAtomicInt m_loop_thread;
    QFuture<void> m_io_thread;

    void init();
    void shutdown();

    void io_thread();
};

} // namespace arcade
