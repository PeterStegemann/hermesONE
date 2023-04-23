// Copyright 2008 Peter Stegemann

#ifndef SIGNAL_SOURCE_MAP_H
#define SIGNAL_SOURCE_MAP_H

#include "Setup/Source/Map.h"

#define SIGNAL_SOURCE_MAP_INPUT_SIGNAL_PER_VALUE	( SIGNAL_VALUE_RANGE / 600)
#define SIGNAL_SOURCE_MAP_INPUT_100_PERCENT_VALUE	( SIGNAL_SOURCE_MAP_INPUT_SIGNAL_PER_VALUE * 100)

#define SIGNAL_SOURCE_MAP_SIGNAL_PER_VALUE			( SIGNAL_VALUE_RANGE / 200)
#define SIGNAL_SOURCE_MAP_100_PERCENT_VALUE			( SIGNAL_SOURCE_MAP_SIGNAL_PER_VALUE * 100)

class Signal_Processor;

class Signal_Source_Map
{
	private:
		int16_t getPoint( uint8_t PointId, Signal_Processor* SignalProcessor);
		void updatePoints( void);

	public:
		uint8_t Points;

		Setup_Source_Map Setup;

		uint8_t InputSignalSourceId;
		uint8_t PointSignalSourceId[ SETUP_SOURCE_MAP_POINTS];

		void Initialize( void);
		void Reset( void);
		void LoadSetup( uint16_t SetupSourceId);

		int16_t CalculateValue( Signal_Processor* SignalProcessor);
};

#endif
