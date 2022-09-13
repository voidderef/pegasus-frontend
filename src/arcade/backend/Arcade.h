#pragma once

#include <QObject>
#include <QtPlugin>
#include <QQmlEngine>
#include <QQmlParserStatus>

#include "IOManager.h"

namespace arcade {

class Arcade : public QObject, public QQmlParserStatus {
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PLUGIN_METADATA(IID "org.djhackers.pegasus.Arcade")

    Q_PROPERTY(IOManager* ioManager READ ioManager WRITE setIoManager CONSTANT)

public:
    ~Arcade();

    void classBegin() override;
    void componentComplete() override;

    IOManager* ioManager() const {
        return m_ioManager;
    }

    void setIoManager(IOManager* ioManager) {
        m_ioManager = ioManager;
    }

private:
    IOManager* m_ioManager;

    void init();
    void shutdown();
};

} // namespace arcade
