// Copyright 2008 Peter Stegemann

#pragma once

#include "Input/Rotary.h"
#include "Input/Service.h"
#include "Setup/Source/Input.h"
#include "Status/Service.h"

#define SIGNAL_SOURCE_INPUT_INIT_SIGNAL_PER_VALUE		( SIGNAL_VALUE_RANGE / 200)
#define SIGNAL_SOURCE_INPUT_INIT_100_PERCENT_VALUE		( SIGNAL_SOURCE_INPUT_INIT_SIGNAL_PER_VALUE * 100)
#define SIGNAL_SOURCE_INPUT_STEP_SIGNAL_PER_VALUE		( SIGNAL_VALUE_RANGE / 200)
#define SIGNAL_SOURCE_INPUT_STEP_100_PERCENT_VALUE		( SIGNAL_SOURCE_INPUT_STEP_SIGNAL_PER_VALUE * 100)
#define SIGNAL_SOURCE_INPUT_VOLUME_SIGNAL_PER_VALUE		( SIGNAL_VALUE_RANGE / 200)
#define SIGNAL_SOURCE_INPUT_VOLUME_100_PERCENT_VALUE	( SIGNAL_SOURCE_INPUT_VOLUME_SIGNAL_PER_VALUE * 100)

class Signal_Processor;

class Signal_Source_Input
{
  private:
    Input_Service* inputService;
    Status_Service* statusService;

    Input_Rotary rotary;

    int16_t value;

    bool lastLowInput, lastHighInput;

    // Signals whether the setup of this source was modified since the last time it was stored /
    // loaded.
    bool modified;

  public:
    Setup_Source_Input Setup;

    void Initialize( Input_Service* InputService, Status_Service* StatusService);
    void Reset( void);
    void LoadSetup( uint16_t SetupSourceId);
    void StoreIfModified( uint16_t SetupSourceId);

    int16_t CalculateValue( const Signal_Processor* SignalProcessor);
};
