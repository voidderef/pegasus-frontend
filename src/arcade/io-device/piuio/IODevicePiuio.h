#pragma once

#include "../../api/IODevice.h"

namespace arcade {

// TODO have two sub classes that provide abstraction for ITG vs. PIU inputs
class IODevicePiuio : public QObject, public IODevice {
    Q_OBJECT
    Q_INTERFACES(arcade::IODevice)
    Q_PLUGIN_METADATA(IID "org.djhackers.pegasus.IODevicePiuio")

public:
    enum Input {
        INPUT_PAD_P1_LU = 0,
        INPUT_PAD_P1_RU = 1,
        INPUT_PAD_P1_CN = 2,
        INPUT_PAD_P1_LD = 3,
        INPUT_PAD_P1_RD = 4,

        INPUT_PAD_P2_LU = 5,
        INPUT_PAD_P2_RU = 6,
        INPUT_PAD_P2_CN = 7,
        INPUT_PAD_P2_LD = 8,
        INPUT_PAD_P2_RD = 9,

        INPUT_TEST = 10,
        INPUT_SERVICE = 11,
        INPUT_COIN = 12,

        INPUT_TOTAL_COUNT = 13,
    };
    Q_ENUMS(Input)

    enum Output {
        OUTPUT_PAD_P1_LU = 0,
        OUTPUT_PAD_P1_RU = 1,
        OUTPUT_PAD_P1_CN = 2,
        OUTPUT_PAD_P1_LD = 3,
        OUTPUT_PAD_P1_RD = 4,

        OUTPUT_PAD_P2_LU = 5,
        OUTPUT_PAD_P2_RU = 6,
        OUTPUT_PAD_P2_CN = 7,
        OUTPUT_PAD_P2_LD = 8,
        OUTPUT_PAD_P2_RD = 9,

        OUTPUT_CABINET_TOP_LAMP_L1 = 10,
        OUTPUT_CABINET_TOP_LAMP_L2 = 11,
        OUTPUT_CABINET_TOP_LAMP_R1 = 12,
        OUTPUT_CABINET_TOP_LAMP_R2 = 13,
        OUTPUT_CABINET_BASS_NEON = 14,

        OUTPUT_TOTAL_COUNT = 15,
    };
    Q_ENUMS(Output)

    IODevicePiuio();
    ~IODevicePiuio();

    const char* name() override;

    bool open() override;
    bool update() override;
    bool close() override;

    Q_INVOKABLE void setOutput(Output output, bool active);

signals:
    void inputPressed(Input input);
    void inputReleased(Input input);
    void inputHeld(Input input);

private:
    void* m_handle;
    bool m_prev_buffered_inputs[INPUT_TOTAL_COUNT];
    bool m_buffered_outputs[OUTPUT_TOTAL_COUNT];
};

} // namespace arcade