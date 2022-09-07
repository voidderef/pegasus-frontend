#pragma once

#include <cstdbool>
#include <cstdint>

#include <QObject>

namespace model {

class IODevice : public QObject {
    Q_OBJECT

public:
    enum Input {
        INPUT_ACCEPT = 0,
        INPUT_CANCEL = 1,
        INPUT_UP = 2,
        INPUT_DOWN = 3,
        INPUT_LEFT = 4,
        INPUT_RIGHT = 5,
        INPUT_TEST = 6,
        INPUT_SERVICE = 7,

        INPUT_TOTAL_COUNT = 8,
    };
    Q_ENUM(Input)

    virtual ~IODevice() = default;

    virtual const char* name() = 0;

    virtual bool open() = 0;
    virtual bool update() = 0;
    virtual bool close() = 0;

    virtual bool get_input(Input input) = 0;

    virtual void set_output(uint16_t index, bool active) = 0;
};

}