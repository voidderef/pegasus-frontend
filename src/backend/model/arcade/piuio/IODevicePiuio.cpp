#include "IODevicePiuio.h"

#include <string.h>

#include "Log.h"

#include "piuio.h"
#include "piuio-usb.h"
#include "result.h"

namespace model {

IODevicePiuio::IODevicePiuio() :
    m_handle(nullptr)
{

}

IODevicePiuio::~IODevicePiuio()
{
    Q_ASSERT(m_handle == nullptr);
}

const char* IODevicePiuio::name()
{
    return "piuio";
}

bool IODevicePiuio::open()
{
    Q_ASSERT(m_handle == nullptr);

    memset(m_buffered_inputs, 0, sizeof(m_buffered_inputs));

    if (!piuio_usb_available()) {
        Log::warning("No connected PIUIO over USB found");
        return false;
    }

    result_t result = piuio_usb_open(&m_handle);

    if (RESULT_IS_ERROR(result)) {
        Log::error(LOGMSG("Opening PIUIO over USB failed: %1").arg(strerror(result)));
    }

    return true;
}

bool IODevicePiuio::update()
{
    Q_ASSERT(m_handle != nullptr);

    union piuio_output_paket output;
    struct piuio_usb_input_batch_paket input;

    memset(&output, 0, sizeof(output));

    result_t result = piuio_usb_poll_full_cycle(m_handle, &output, &input);

    if (RESULT_IS_ERROR(result)) {
         Log::error(LOGMSG("Polling PIUIO over USB failed: %1").arg(strerror(result)));
         return false;
    }

    memset(m_buffered_inputs, 0, sizeof(m_buffered_inputs));

    // TODO include piubtn and allow switching on/off piubtn or pad controlled inputs
    for (int i = 0; i < PIUIO_SENSOR_MASK_TOTAL_COUNT; i++) {
        // Player 1
        m_buffered_inputs[static_cast<int>(IODevice::Input::LEFT)] |= input.pakets[i].piu.sensor_p1_ld;
        m_buffered_inputs[static_cast<int>(IODevice::Input::RIGHT)] |= input.pakets[i].piu.sensor_p1_rd;
        m_buffered_inputs[static_cast<int>(IODevice::Input::ACCEPT)] |= input.pakets[i].piu.sensor_p1_cn;

        // Player 2
        m_buffered_inputs[static_cast<int>(IODevice::Input::LEFT)] |= input.pakets[i].piu.sensor_p2_ld;
        m_buffered_inputs[static_cast<int>(IODevice::Input::RIGHT)] |= input.pakets[i].piu.sensor_p2_rd;
        m_buffered_inputs[static_cast<int>(IODevice::Input::ACCEPT)] |= input.pakets[i].piu.sensor_p2_cn;

        m_buffered_inputs[static_cast<int>(IODevice::Input::MAIN_MENU)] |= input.pakets[i].piu.test;
    }

    return true;
}

bool IODevicePiuio::close()
{
    Q_ASSERT(m_handle != nullptr);

    piuio_usb_close(m_handle);

    m_handle = nullptr;

    return true;
}

bool IODevicePiuio::is_input_active(IODevice::Input input)
{
    if (static_cast<int>(input) >= static_cast<int>(IODevice::Input::TOTAL_COUNT)) {
        Log::warning(LOGMSG("Invalid input index requested, defaulting to 0: %1").arg(static_cast<int>(input)));
        return false;
    }

    return m_buffered_inputs[static_cast<int>(input)];
}

void IODevicePiuio::set_output(uint16_t index, bool active)
{
    Log::info(LOGMSG("Setting output PIUIO: %1 -> %2").arg(index).arg(active));
}

} // namespace model