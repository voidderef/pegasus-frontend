#pragma once

#include <QObject>

#include "IOManager.h"

#include "utils/QmlHelpers.h"

namespace model {

class Arcade : public QObject {
    Q_OBJECT
    Q_PROPERTY(IOManager* ioManager READ ioManager WRITE setIoManager)  

public:
    explicit Arcade(QObject* parent = nullptr);

    IOManager* ioManager() const {
        return m_ioManager;
    }

    void setIoManager(IOManager* ioManager) {
        m_ioManager = ioManager;
    }

private:
    IOManager* m_ioManager;
};

} // namespace model