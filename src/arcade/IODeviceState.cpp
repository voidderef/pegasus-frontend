#include "IODeviceState.h"

namespace model {

IODeviceState::IODeviceState(IODevice* device) :
    m_device(device)
{
    memset(&m_buffer_prev_inputs, 0, sizeof(m_buffer_prev_inputs));
}

IODeviceState::~IODeviceState()
{
    delete m_device;
}

void IODeviceState::capture_current_input_states()
{
    for (int i = 0; i < IODevice::INPUT_TOTAL_COUNT; i++) {
        m_buffer_cur_inputs[i] = m_device->get_input(static_cast<IODevice::Input>(i));
    }
}

bool IODeviceState::inputIsPressed(IODevice::Input input) const
{
    Q_ASSERT(input < IODevice::INPUT_TOTAL_COUNT);

    return !m_buffer_prev_inputs[input] && m_buffer_cur_inputs[input];
}

bool IODeviceState::inputIsReleased(IODevice::Input input) const
{
    Q_ASSERT(input < IODevice::INPUT_TOTAL_COUNT);

    return m_buffer_prev_inputs[input] && !m_buffer_cur_inputs[input];
}

bool IODeviceState::inputIsHeld(IODevice::Input input) const
{
    Q_ASSERT(input < IODevice::INPUT_TOTAL_COUNT);

    return m_buffer_prev_inputs[input] && m_buffer_cur_inputs[input];
}

void IODeviceState::input_buffer_shift()
{
    memcpy(&m_buffer_prev_inputs, &m_buffer_cur_inputs, sizeof(m_buffer_prev_inputs));
    memset(&m_buffer_cur_inputs, 0, sizeof(m_buffer_cur_inputs));
}

}