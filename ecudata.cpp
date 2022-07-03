#include "ecudata.h"

EcuData::EcuData()
{
    //////// Init Channels ////////
    // Realtime variables
    channelsLive[static_cast<int>(eChannelLive::CH_RPM)]           = new VocalObject("RPM", "", "Revolutions per minute", 5);
    channelsLive[static_cast<int>(eChannelLive::CH_PW)]            = new VocalObject("PW", "μs", "Current injector pulse width", 9);
    channelsLive[static_cast<int>(eChannelLive::CH_IGN_ADVANCE)]   = new VocalObject("Ign Advance", "°BTDC", "Ignition advance before top dead centre");
    channelsLive[static_cast<int>(eChannelLive::CH_IGN_DWELL)]     = new VocalObject("Ign Dwell", "μs", "Ignition dwell");
    channelsLive[static_cast<int>(eChannelLive::CH_LOOP_COUNT)]    = new VocalObject("Loop Count", "l/s", "Loops per second of the main loop", 15);

    // ADC
    channelsLive[static_cast<int>(eChannelLive::CH_ADC0)]  = new VocalObject("ADC0", "raw", "Analog to digital channel 0");
    channelsLive[static_cast<int>(eChannelLive::CH_ADC1)]  = new VocalObject("ADC1", "raw", "Analog to digital channel 1");
    channelsLive[static_cast<int>(eChannelLive::CH_ADC2)]  = new VocalObject("ADC2", "raw", "Analog to digital channel 2");
    channelsLive[static_cast<int>(eChannelLive::CH_ADC3)]  = new VocalObject("ADC3", "raw", "Analog to digital channel 3");
    channelsLive[static_cast<int>(eChannelLive::CH_ADC4)]  = new VocalObject("ADC4", "raw", "Analog to digital channel 4");
    channelsLive[static_cast<int>(eChannelLive::CH_ADC5)]  = new VocalObject("ADC5", "raw", "Analog to digital channel 5");
    channelsLive[static_cast<int>(eChannelLive::CH_ADC6)]  = new VocalObject("ADC6", "raw", "Analog to digital channel 6");
    channelsLive[static_cast<int>(eChannelLive::CH_ADC7)]  = new VocalObject("ADC7", "raw", "Analog to digital channel 7");
    channelsLive[static_cast<int>(eChannelLive::CH_ADC8)]  = new VocalObject("ADC8", "raw", "Analog to digital channel 8");
    channelsLive[static_cast<int>(eChannelLive::CH_ADC9)]  = new VocalObject("ADC9", "raw", "Analog to digital channel 9");
    channelsLive[static_cast<int>(eChannelLive::CH_ADC10)] = new VocalObject("ADC10", "raw", "Analog to digital channel 10");
    channelsLive[static_cast<int>(eChannelLive::CH_ADC11)] = new VocalObject("ADC11", "raw", "Analog to digital channel 11");
    channelsLive[static_cast<int>(eChannelLive::CH_ADC12)] = new VocalObject("ADC12", "raw", "Analog to digital channel 12");
    channelsLive[static_cast<int>(eChannelLive::CH_ADC13)] = new VocalObject("ADC13", "raw", "Analog to digital channel 13");
    channelsLive[static_cast<int>(eChannelLive::CH_ADC14)] = new VocalObject("ADC14", "raw", "Analog to digital channel 14");
    channelsLive[static_cast<int>(eChannelLive::CH_ADC15)] = new VocalObject("ADC15", "raw", "Analog to digital channel 15");

    channelsLive[static_cast<int>(eChannelLive::CH_SERIAL_FRAMES_RECEIVED)] = new VocalObject("Serial Frames Received", "", "", 10);
    channelsLive[static_cast<int>(eChannelLive::CH_SERIAL_FRAMES_SENT)]     = new VocalObject("Serial Frames Sent", "", "", 10);

    channelsLive[static_cast<int>(eChannelLive::CH_ADC_VOLTS_CPU_TEMP)]    = new VocalObject("ADC Volts: CPU Temp", "V");
    channelsLive[static_cast<int>(eChannelLive::CH_ADC_VOLTS_CPU_VREFINT)] = new VocalObject("ADC Volts: CPU VrefInt", "V");
    channelsLive[static_cast<int>(eChannelLive::CH_ADC_VOLTS_CPU_VBAT)]    = new VocalObject("ADC Volts: CPU VBat", "V");

    // Config variables
    channelsConfig[CH_REQ_FUEL]              = new VocalObject("Required Fuel Constant", "μs");
    channelsConfig[CH_ENGINE_STROKE]         = new VocalObject("Engine Stroke", "", "2 stroke or 4 stroke", 1);
    channelsConfig[CH_TRIGGERS_PER_CYCLE]    = new VocalObject("Triggers per Cycle", "", "Crank triggers per engine cycle", 1);
    channelsConfig[CH_INJ_SQUIRTS_PER_CYCLE] = new VocalObject("Squirts per cycle", "", "", 1);
    channelsConfig[CH_TPS_MIN_ADC]           = new VocalObject("TPS Min ADC", "raw");
    channelsConfig[CH_TPS_MAX_ADC]           = new VocalObject("TPS Max ADC", "raw");
    channelsConfig[CH_INJ_DEAD_TIME]         = new VocalObject("Injector Dead Time", "μs", "", 8);
    channelsConfig[CH_FUEL_ALGORITHM]        = new VocalObject("Fuel Algorithm", "", "", 1);
    channelsConfig[CH_ENGINE_DISPLACEMENT]   = new VocalObject("Engine Displacement", "cc", "", 6);
    channelsConfig[CH_INJ_SIZE]              = new VocalObject("Injector Flow Rate", "cc/min", "", 5);
    channelsConfig[CH_REV_LIMIT]             = new VocalObject("Rev Limit", "RPM", "", 5);
}

EcuData::~EcuData()
{
    for (int i = 0; i < static_cast<int>(eChannelLive::CHANNEL_T_COUNT); i++)
    {
        delete channelsLive[i];
    }
}
