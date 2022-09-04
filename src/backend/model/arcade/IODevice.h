#pragma once

#include <cstdbool>
#include <cstdint>

namespace model {

class IODevice {
public:
    virtual ~IODevice() = default;

    enum class Input : unsigned char {
        ACCEPT = 0,
        CANCEL = 1,
        DETAILS = 2,
        FILTERS = 3,
        NEXT_PAGE = 4,
        PREV_PAGE = 5,
        PAGE_UP = 6,
        PAGE_DOWN = 7,
        MAIN_MENU = 8,
        LEFT = 9,
        RIGHT = 10,
        UP = 11,
        DOWN = 12,

        TOTAL_COUNT = 13,
    };

    virtual const char* name() = 0;

    virtual bool open() = 0;
    virtual bool update() = 0;
    virtual bool close() = 0;

    virtual bool is_input_active(Input input) = 0;

    virtual void set_output(uint16_t index, bool active) = 0;
};

}