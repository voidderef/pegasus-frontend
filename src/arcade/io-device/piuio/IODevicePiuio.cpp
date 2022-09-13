#include "IODevicePiuio.h"

#include <string.h>

extern "C" {

#include "piuio.h"
#include "piuio-usb.h"
#include "result.h"

}

#include "../../backend/Log.h"

namespace arcade {

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
        LOG_ERROR("No connected PIUIO over USB found");
        return false;
    }

    result_t result = piuio_usb_open(&m_handle);

    if (RESULT_IS_ERROR(result)) {
        LOG_ERROR("Opening PIUIO over USB failed: %s", strerror(result));
        return false;
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
         LOG_ERROR("Polling PIUIO over USB failed: %s", strerror(result));
         return false;
    }

    memset(m_buffered_inputs, 0, sizeof(m_buffered_inputs));

    for (int i = 0; i < PIUIO_SENSOR_MASK_TOTAL_COUNT; i++) {
        // Player 1
        m_buffered_inputs[Input::Event::INPUT_EVENT_LEFT] |= input.pakets[i].piu.sensor_p1_ld;
        m_buffered_inputs[Input::Event::INPUT_EVENT_RIGHT] |= input.pakets[i].piu.sensor_p1_rd;
        m_buffered_inputs[Input::Event::INPUT_EVENT_ACCEPT] |= input.pakets[i].piu.sensor_p1_cn;
        m_buffered_inputs[Input::Event::INPUT_EVENT_CANCEL] |= input.pakets[i].piu.sensor_p1_lu;
        m_buffered_inputs[Input::Event::INPUT_EVENT_CANCEL] |= input.pakets[i].piu.sensor_p1_ru;

        // Player 2
        m_buffered_inputs[Input::Event::INPUT_EVENT_LEFT] |= input.pakets[i].piu.sensor_p2_ld;
        m_buffered_inputs[Input::Event::INPUT_EVENT_RIGHT] |= input.pakets[i].piu.sensor_p2_rd;
        m_buffered_inputs[Input::Event::INPUT_EVENT_ACCEPT] |= input.pakets[i].piu.sensor_p2_cn;
        m_buffered_inputs[Input::Event::INPUT_EVENT_CANCEL] |= input.pakets[i].piu.sensor_p2_lu;
        m_buffered_inputs[Input::Event::INPUT_EVENT_CANCEL] |= input.pakets[i].piu.sensor_p2_ru;

        // Operator
        m_buffered_inputs[Input::Event::INPUT_EVENT_TEST] |= input.pakets[i].piu.test;
        m_buffered_inputs[Input::Event::INPUT_EVENT_SERVICE] |= input.pakets[i].piu.test;
    }

    m_buffered_inputs[0] = !m_buffered_inputs[0];

    return true;
}

bool IODevicePiuio::close()
{
    Q_ASSERT(m_handle != nullptr);

    piuio_usb_close(m_handle);

    m_handle = nullptr;

    return true;
}

bool IODevicePiuio::get_input(Input::Event input)
{
    return m_buffered_inputs[input];
}

void IODevicePiuio::set_output(uint16_t index, bool active)
{
    LOG_DEBUG("Setting output PIUIO: %d -> %d", index, active);
}

} // namespace model