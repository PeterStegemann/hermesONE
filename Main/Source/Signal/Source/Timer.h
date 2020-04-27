// Copyright 2011 Peter Stegemann

#ifndef SIGNAL_SOURCE_TIMER_H
#define SIGNAL_SOURCE_TIMER_H

#include "Setup/Source/Timer.h"

class Signal_Processor;

class Signal_Source_Timer
{
	private:
		volatile int16_t time;

		// Signals whether the setup of this source was modified since the last time it was stored
		// or loaded.
		bool modified;

		int16_t lastUptime;
		int16_t lastWarnTime;

	public:
		Setup_Source_Timer Setup;

		uint8_t TriggerSignalSourceId;

		void Initialize( void);
		void Reset( void);
		void LoadSetup( uint16_t SetupSourceId);
		void StoreIfModified( uint16_t SetupSourceId);

		int16_t CalculateValue( Signal_Processor* SignalProcessor);

		int16_t GetTime( void);
		void ResetTime( void);
};

#endif
