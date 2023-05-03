// Copyright 2008 Peter Stegemann

#pragma once

#include "Setup/Source/Mix.h"

#define SIGNAL_SOURCE_MIX_SIGNAL_PER_VALUE		( SIGNAL_VALUE_RANGE / 600)
#define SIGNAL_SOURCE_MIX_100_PERCENT_VALUE		( SIGNAL_SOURCE_MIX_SIGNAL_PER_VALUE * 100)
//#define SIGNAL_SOURCE_MIX_OFF_VALUE				( SIGNAL_MINIMUM_VALUE - SIGNAL_SOURCE_MIX_SIGNAL_PER_VALUE)

class Signal_Processor;

class Signal_Source_Mix
{
	public:
		Setup_Source_Mix Setup;

		uint8_t SignalSourceId[ SETUP_SOURCE_MIX_INPUTS];

		void Initialize( void);
		void Reset( void);
		void LoadSetup( uint16_t SetupSourceId);

		int16_t CalculateValue( Signal_Processor* SignalProcessor);
};
