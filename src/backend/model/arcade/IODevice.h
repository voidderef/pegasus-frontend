#pragma once

#include <cstdbool>

namespace model {

class IODevice {
public:
    IODevice();
    ~IODevice();

    enum class Input : unsigned char {
        ACCEPT,
        CANCEL,
        DETAILS,
        FILTERS,
        NEXT_PAGE,
        PREV_PAGE,
        PAGE_UP,
        PAGE_DOWN,
        MAIN_MENU,
    };

    const char* name();

    bool open();
    bool update();
    bool close();

    bool is_input_active(Input input);
};

}