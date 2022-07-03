
#pragma once

#include <QObject>
#include "vocalObject.h"

class EcuData : public QObject
{
    Q_OBJECT
public:
    EcuData();
    ~EcuData();

    // Realtime variables
    enum class eChannelLive
    {
        // Inputs
        CH_RPM = 0,
        CH_LOOP_COUNT,

        // Outputs
        CH_PW,
        CH_IGN_ADVANCE,
        CH_IGN_DWELL,

        // ADC Raw
        CH_ADC0,
        CH_ADC1,
        CH_ADC2,
        CH_ADC3,
        CH_ADC4,
        CH_ADC5,
        CH_ADC6,
        CH_ADC7,
        CH_ADC8,
        CH_ADC9,
        CH_ADC10,
        CH_ADC11,
        CH_ADC12,
        CH_ADC13,
        CH_ADC14,
        CH_ADC15,

        // ADC Volts
        CH_ADC_VOLTS_CPU_TEMP,
        CH_ADC_VOLTS_CPU_VREFINT,
        CH_ADC_VOLTS_CPU_VBAT,

        CH_SERIAL_FRAMES_RECEIVED,
        CH_SERIAL_FRAMES_SENT,

        CHANNEL_T_COUNT // This must always be the last member
    };

    enum ChannelConfig_t
    {
        // Config variables
        CH_REQ_FUEL,
        CH_ENGINE_STROKE,
        CH_ENGINE_DISPLACEMENT,
        CH_TRIGGERS_PER_CYCLE,
        CH_INJ_SQUIRTS_PER_CYCLE,
        CH_INJ_DEAD_TIME,
        CH_INJ_SIZE,
        CH_TPS_MAX_ADC,
        CH_TPS_MIN_ADC,
        CH_FUEL_ALGORITHM,
        CH_REV_LIMIT,

        CHANNEL_CONFIG_T_COUNT // This must always be the last member
    };

    enum FuelAlgorithm_t
    {
        ALGORITHM_SPEED_DENSITY,
        ALGORITHM_ALPHA_N
    };

    enum TableSizeFuel_t
    {
        tableSizeVErows    = 32,
        tableSizeVEcolumns = 32
    };

    VocalObject *channelsLive[static_cast<int>(eChannelLive::CHANNEL_T_COUNT)];
    VocalObject *channelsConfig[CHANNEL_CONFIG_T_COUNT];
};
