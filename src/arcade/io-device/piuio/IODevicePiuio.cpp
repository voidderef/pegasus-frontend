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

    memset(m_prev_buffered_inputs, 0, sizeof(m_prev_buffered_inputs));

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
    bool buffered_inputs[INPUT_TOTAL_COUNT];

    memset(&output, 0, sizeof(output));

    // Process outputs
    output.piu.pad_light_p1_lu = m_buffered_outputs[OUTPUT_PAD_P1_LU];
    output.piu.pad_light_p1_ru = m_buffered_outputs[OUTPUT_PAD_P1_RU];
    output.piu.pad_light_p1_cn = m_buffered_outputs[OUTPUT_PAD_P1_CN];
    output.piu.pad_light_p1_ld = m_buffered_outputs[OUTPUT_PAD_P1_LD];
    output.piu.pad_light_p1_rd = m_buffered_outputs[OUTPUT_PAD_P1_RD];

    output.piu.pad_light_p2_lu = m_buffered_outputs[OUTPUT_PAD_P2_LU];
    output.piu.pad_light_p2_ru = m_buffered_outputs[OUTPUT_PAD_P2_RU];
    output.piu.pad_light_p2_cn = m_buffered_outputs[OUTPUT_PAD_P2_CN];
    output.piu.pad_light_p2_ld = m_buffered_outputs[OUTPUT_PAD_P2_LD];
    output.piu.pad_light_p2_rd = m_buffered_outputs[OUTPUT_PAD_P2_RD];
    
    output.piu.top_lamp_r2 = m_buffered_outputs[OUTPUT_CABINET_TOP_LAMP_R2];
    output.piu.top_lamp_r1 = m_buffered_outputs[OUTPUT_CABINET_TOP_LAMP_R1];
    output.piu.top_lamp_l2 = m_buffered_outputs[OUTPUT_CABINET_TOP_LAMP_L2];
    output.piu.top_lamp_l1 = m_buffered_outputs[OUTPUT_CABINET_TOP_LAMP_L1];
    output.piu.light_bass = m_buffered_outputs[OUTPUT_CABINET_BASS_NEON];

    result_t result = piuio_usb_poll_full_cycle(m_handle, &output, &input);

    if (RESULT_IS_ERROR(result)) {
         LOG_ERROR("Polling PIUIO over USB failed: %s", strerror(result));
         return false;
    }

    memset(buffered_inputs, 0, sizeof(buffered_inputs));

    for (int i = 0; i < PIUIO_SENSOR_MASK_TOTAL_COUNT; i++) {
        // Player 1
        buffered_inputs[INPUT_PAD_P1_LD] |= input.pakets[i].piu.sensor_p1_ld;
        buffered_inputs[INPUT_PAD_P1_RD] |= input.pakets[i].piu.sensor_p1_rd;
        buffered_inputs[INPUT_PAD_P1_CN] |= input.pakets[i].piu.sensor_p1_cn;
        buffered_inputs[INPUT_PAD_P1_LU] |= input.pakets[i].piu.sensor_p1_lu;
        buffered_inputs[INPUT_PAD_P1_RU] |= input.pakets[i].piu.sensor_p1_ru;

        // Player 2
        buffered_inputs[INPUT_PAD_P2_LD] |= input.pakets[i].piu.sensor_p2_ld;
        buffered_inputs[INPUT_PAD_P2_RD] |= input.pakets[i].piu.sensor_p2_rd;
        buffered_inputs[INPUT_PAD_P2_CN] |= input.pakets[i].piu.sensor_p2_cn;
        buffered_inputs[INPUT_PAD_P2_LU] |= input.pakets[i].piu.sensor_p2_lu;
        buffered_inputs[INPUT_PAD_P2_RU] |= input.pakets[i].piu.sensor_p2_ru;

        // Operator
        buffered_inputs[INPUT_TEST] |= input.pakets[i].piu.test;
        buffered_inputs[INPUT_SERVICE] |= input.pakets[i].piu.service;
        buffered_inputs[INPUT_COIN] |= input.pakets[i].piu.coin_1;
        buffered_inputs[INPUT_COIN] |= input.pakets[i].piu.coin_2;
    }

    // Generate input events
    for (int i = 0; i < INPUT_TOTAL_COUNT; i++) {
        Input input = static_cast<Input>(i);

        if (!m_prev_buffered_inputs[input] && buffered_inputs[input]) {
            emit inputPressed(input);
        } else if (m_prev_buffered_inputs[input] && !buffered_inputs[input]) {
            emit inputReleased(input);
        } else if (m_prev_buffered_inputs[input] && buffered_inputs[input]) {
            emit inputHeld(input);
        }
    }

    // Buffer shift
    memcpy(m_prev_buffered_inputs, buffered_inputs, sizeof(m_prev_buffered_inputs));

    return true;
}

bool IODevicePiuio::close()
{
    Q_ASSERT(m_handle != nullptr);

    piuio_usb_close(m_handle);

    m_handle = nullptr;

    return true;
}

void IODevicePiuio::setOutput(Output output, bool active)
{
    Q_ASSERT(output < OUTPUT_TOTAL_COUNT);

    m_buffered_outputs[output] = active;
}

} // namespace model