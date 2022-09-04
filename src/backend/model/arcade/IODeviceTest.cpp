#include "IODevice.h"

#include <stdio.h>
#include <unistd.h>

namespace model {

IODevice::IODevice()
{

}

IODevice::~IODevice()
{

}

const char* IODevice::name()
{
    return "test";
}

bool IODevice::open()
{
    return true;
}

bool IODevice::update()
{
    printf(">>>>>\n");

    sleep(1);

    return true;
}

bool IODevice::close()
{
    return true;
}

bool IODevice::is_input_active(Input input)
{

}

} // namespace model