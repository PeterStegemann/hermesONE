// Copyright 2010 Peter Stegemann

#ifndef SIGNAL_SOURCE_FOLLOWER_H
#define SIGNAL_SOURCE_FOLLOWER_H

#include "Setup/Source/Follower.h"

#define SIGNAL_SOURCE_FOLLOWER_TARGET_SIGNAL_PER_VALUE		( SIGNAL_VALUE_RANGE / 600)
#define SIGNAL_SOURCE_FOLLOWER_TARGET_100_PERCENT_VALUE		( SIGNAL_SOURCE_FOLLOWER_TARGET_SIGNAL_PER_VALUE * 100)
//#define SIGNAL_SOURCE_FOLLOWER_OFF_VALUE					( SIGNAL_MINIMUM_VALUE - SIGNAL_SOURCE_FOLLOWER_SIGNAL_PER_VALUE)

#define SIGNAL_SOURCE_FOLLOWER_STEP_SIGNAL_PER_VALUE		( SIGNAL_VALUE_RANGE / 200)
#define SIGNAL_SOURCE_FOLLOWER_STEP_100_PERCENT_VALUE		( SIGNAL_SOURCE_FOLLOWER_STEP_SIGNAL_PER_VALUE * 100)

#define SIGNAL_SOURCE_FOLLOWER_TRIGGER_SIGNAL_PER_VALUE		( SIGNAL_VALUE_RANGE / 200)
#define SIGNAL_SOURCE_FOLLOWER_TRIGGER_100_PERCENT_VALUE	( SIGNAL_SOURCE_FOLLOWER_TRIGGER_SIGNAL_PER_VALUE * 100)

class Signal_Processor;

class Signal_Source_Follower
{
	private:
		int16_t value;

	public:
		Setup_Source_Follower Setup;

		uint8_t TargetSignalSourceId;
		uint8_t StepSignalSourceId;
		uint8_t TriggerSignalSourceId;

		void Initialize( void);
		void Reset( void);
		void LoadSetup( uint16_t SetupSourceId);

		int16_t CalculateValue( Signal_Processor* SignalProcessor);
};

#endif
