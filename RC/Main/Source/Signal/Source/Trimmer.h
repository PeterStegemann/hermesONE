// Copyright 2012 Peter Stegemann

#pragma once

#include "Setup/Source/Trimmer.h"

#define SIGNAL_SOURCE_TRIMMER_INPUT_SIGNAL_PER_VALUE	( SIGNAL_VALUE_RANGE / 600)
#define SIGNAL_SOURCE_TRIMMER_INPUT_0_PERCENT_VALUE		( SIGNAL_SOURCE_TRIMMER_INPUT_SIGNAL_PER_VALUE * 0)
#define SIGNAL_SOURCE_TRIMMER_INPUT_100_PERCENT_VALUE	( SIGNAL_SOURCE_TRIMMER_INPUT_SIGNAL_PER_VALUE * 100)

#define SIGNAL_SOURCE_TRIMMER_SIGNAL_PER_VALUE			( SIGNAL_VALUE_RANGE / 200)
#define SIGNAL_SOURCE_TRIMMER_100_PERCENT_VALUE			( SIGNAL_SOURCE_TRIMMER_SIGNAL_PER_VALUE * 100)
#define SIGNAL_SOURCE_TRIMMER_OFF_VALUE					( SIGNAL_MINIMUM_VALUE - SIGNAL_SOURCE_TRIMMER_SIGNAL_PER_VALUE)

class Signal_Processor;

class Signal_Source_Trimmer
{
	private:
		void updatePoint( uint8_t PointId);

	public:
		Setup_Source_Trimmer Setup;

		uint8_t InputSignalSourceId;
		uint8_t TrimSignalSourceId;
		uint8_t LimitSignalSourceId;

		void Initialize( void);
		void Reset( void);
		void LoadSetup( uint16_t SetupSourceId);

		int16_t CalculateValue( Signal_Processor* SignalProcessor);
};
