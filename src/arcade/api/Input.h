#pragma once

#include <QObject>

namespace arcade {

class Input : public QObject {
    Q_OBJECT

public:
    Input() :
        QObject() {}

    enum Event {
        INPUT_EVENT_ACCEPT = 0,
        INPUT_EVENT_CANCEL = 1,
        INPUT_EVENT_UP = 2,
        INPUT_EVENT_DOWN = 3,
        INPUT_EVENT_LEFT = 4,
        INPUT_EVENT_RIGHT = 5,
        INPUT_EVENT_TEST = 6,
        INPUT_EVENT_SERVICE = 7,

        INPUT_EVENT_TOTAL_COUNT = 8,
    };
    Q_ENUMS(Input)
};

}
