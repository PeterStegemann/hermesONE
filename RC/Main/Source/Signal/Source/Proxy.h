// Copyright 2013 Peter Stegemann

#pragma once

#include "Setup/Source/Proxy.h"

#define SIGNAL_SOURCE_PROXY_SLOT_MINIMUM			0
#define SIGNAL_SOURCE_PROXY_SLOT_MAXIMUM			( SIGNAL_SOURCE_PROXY_SLOT_MINIMUM + \
													  SETUP_MODEL_PROXY_SOURCES - 1)

#define SIGNAL_SOURCE_PROXY_SIGNAL_PER_VALUE		( SIGNAL_VALUE_RANGE / 200)
#define SIGNAL_SOURCE_PROXY_100_PERCENT_VALUE		( SIGNAL_SOURCE_PROXY_SIGNAL_PER_VALUE * 100)

class Signal_Processor;

class Signal_Source_Proxy
{
  public:
    Setup_Source_Proxy Setup;
    Setup_Source_Tuple ReferenceSetup;

    uint8_t SignalSourceId;

    void Initialize( void);
    void Reset( void);
    void LoadSetup( uint16_t SetupSourceId);

    int16_t CalculateValue( Signal_Processor* SignalProcessor);
};
