#include "IODeviceP3io.h"

#include <string.h>

#include "Log.h"

#include "usb.h"

namespace model {

IODeviceP3io::IODeviceP3io() :
    m_handle(INVALID_HANDLE_VALUE)
{

}

IODeviceP3io::~IODeviceP3io()
{
    Q_ASSERT(m_handle == INVALID_HANDLE_VALUE);
}

const char* IODeviceP3io::name()
{
    return "p3io";
}

bool IODeviceP3io::open()
{
    Q_ASSERT(m_handle == INVALID_HANDLE_VALUE);

    memset(m_buffered_inputs, 0, sizeof(m_buffered_inputs));
    memset(m_prev_jamma, 0, sizeof(m_prev_jamma));

    m_handle = p3io_usb_open();

    return m_handle != INVALID_HANDLE_VALUE;
}

bool IODeviceP3io::update()
{
    Q_ASSERT(m_handle != INVALID_HANDLE_VALUE);

    uint32_t jamma[3];

    if (!p3io_usb_read_jamma(m_handle, jamma)) {
        return false;
    }

    if (m_prev_jamma[0] != jamma[0]) {
        printf(">>>> %X\n", jamma[0]);
    }

    m_prev_jamma[0] = jamma[0];

    return true;
}

bool IODeviceP3io::close()
{
    Q_ASSERT(m_handle != INVALID_HANDLE_VALUE);

    p3io_usb_close(m_handle);

    m_handle = INVALID_HANDLE_VALUE;

    return true;
}

bool IODeviceP3io::get_input(IODevice::Input input)
{
    return m_buffered_inputs[input];
}

void IODeviceP3io::set_output(uint16_t index, bool active)
{
    Log::info(LOGMSG("Setting output P3IO: %1 -> %2").arg(index).arg(active));
}

} // namespace model