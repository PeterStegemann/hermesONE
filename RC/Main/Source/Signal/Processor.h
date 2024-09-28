// Copyright 2007 Peter Stegemann

#pragma once

#include "Calibration.h"
#include "Channel.h"
#include "Defines.h"
#include "Ports.h"

#include "Setup/Model.h"
#include "Setup/Service.h"
#include "Signal/Service.h"
#include "Source/Source.h"

#define SIGNAL_PROCESSOR_CALCULATIONS_PER_SECOND	45
#define SIGNAL_PROCESSOR_ANALOG_INPUTS				INPUT_ANALOG_PORTS - 2
#define SIGNAL_PROCESSOR_DIGITAL_INPUTS				INPUT_DIGITAL_PORTS

class Signal_Processor
{
  private:
    Input_Service* inputService;

    // This signals the process routine that it is already running. It's used for overrun protection.
    volatile bool inProcessing;
    // This signals the process routine that it should pause the processing. It's used to make sure
    // no calculation is running while a new model is loaded.
    volatile bool pauseProcessing;

    volatile Setup_Service::RFMode rfMode;

    // Raw and calibrated values.
    int16_t rawAnalogValue[ INPUT_ANALOG_PORTS];
    int16_t calibratedAnalogValue[ SIGNAL_PROCESSOR_ANALOG_INPUTS];

    // Calibration engines.
    Signal_Calibration analogCalibration[ SIGNAL_PROCESSOR_ANALOG_INPUTS];

    // All signal sources for the current model.
    Signal_Source_Source source[ SIGNAL_SOURCES];

    // All channels for the current model.
    Signal_Channel channel[ SIGNAL_PPM_CHANNELS];

    bool isRFModuleActive( Setup_Service::RFMode RFMode);

  public:
    Signal_Processor( Input_Service* InputService)
        : inputService( InputService)
    {
    }

    // Initialize processing.
    void Initialize( void);

    // Load model data.
    void LoadModel( void);

    // Store modified sources.
    void StoreModifiedSources( void);

    // This is for the interrupt, not for you.
    void Process( void);

    // Set rf mode.
    void SetRFMode( Setup_Service::RFMode RFMode);

    // Get calibration object.
    Signal_Calibration* GetCalibration( uint8_t CalibrationId);

    // Get channel object.
    Signal_Channel* GetChannel( uint8_t ChannelId);

    // Find next free source slot in the signal memory. Returns false if all slots are used.
    bool FindNextEmptySource( uint8_t* SignalSourceId);

    // Get source object.
    Signal_Source_Source* GetSource( uint8_t SignalSourceId);
/*
		// Calculate the value of a given source.
		int16_t CalculateSourceValue( uint8_t SignalSourceId);
*/
    // Get the current value of a given source.
    int16_t GetSourceValue( uint8_t SignalSourceId);
/*
		// Calculate the value of a given channel.
		int16_t CalculateChannelValue( uint8_t ChannelId);
*/
    // Get the current value of a given channel.
    int16_t GetChannelValue( uint8_t ChannelId);

    // Get a raw input value.
    int16_t GetRawAnalogValue( uint8_t InputId) const;
    // Get a calibrated input value.
    int16_t GetCalibratedAnalogValue( uint8_t InputId) const;

    // Support stuff...
    // Find the next valid source by the given offset. May start with SIGNAL_SOURCE_NONE and will
    // return SIGNAL_SOURCE_NONE at the end.
    uint8_t FindNextSource
    (
        uint8_t SignalSourceId, int8_t Offset, bool HasFixed,
        Signal_Source_Source::Level SourceLevel,
        Signal_Source_Source::Type SourceType = Signal_Source_Source::T_All
    );

    // Get local source id for setup source id.
    uint8_t GetSignalSourceId( uint16_t SetupSourceId);
};
