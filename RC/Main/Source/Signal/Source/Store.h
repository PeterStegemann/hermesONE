// Copyright 2009 Peter Stegemann

#ifndef SIGNAL_SOURCE_STORE_H
#define SIGNAL_SOURCE_STORE_H

#include "Setup/Source/Store.h"
/*
#define SIGNAL_SOURCE_STORE_SIGNAL_PER_VALUE		( SIGNAL_VALUE_RANGE / 600)
#define SIGNAL_SOURCE_STORE_100_PERCENT_VALUE		( SIGNAL_SOURCE_STORE_SIGNAL_PER_VALUE * 100)
#define SIGNAL_SOURCE_STORE_OFF_VALUE				( SIGNAL_MINIMUM_VALUE - SIGNAL_SOURCE_STORE_SIGNAL_PER_VALUE)
*/
class Signal_Processor;

class Signal_Source_Store
{
	private:
		int16_t offset;
		int16_t value;

		// Signals whether the setup of this source was modified since the last time it was stored
		// or loaded.
		bool modified;
	
	public:
		Setup_Source_Store Setup;

		uint8_t SignalSourceId;

		void Initialize( void);
		void Reset( void);
		void LoadSetup( uint16_t SetupSourceId);
		void StoreIfModified( uint16_t SetupSourceId);

		int16_t CalculateValue( Signal_Processor* SignalProcessor);

		void ResetValue( void);
};

#endif
