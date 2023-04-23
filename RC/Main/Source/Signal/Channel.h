// Copyright 2008 Peter Stegemann

#ifndef SIGNAL_CHANNEL_H
#define SIGNAL_CHANNEL_H

#include "Setup/Channel.h"

#define SIGNAL_CHANNEL_INPUT_SIGNAL_PER_VALUE		( SIGNAL_VALUE_RANGE / 600)
#define SIGNAL_CHANNEL_INPUT_0_PERCENT_VALUE		( SIGNAL_CHANNEL_INPUT_SIGNAL_PER_VALUE * 0)
#define SIGNAL_CHANNEL_INPUT_100_PERCENT_VALUE		( SIGNAL_CHANNEL_INPUT_SIGNAL_PER_VALUE * 100)

#define SIGNAL_CHANNEL_SIGNAL_PER_VALUE		( SIGNAL_VALUE_RANGE / 200)
#define SIGNAL_CHANNEL_100_PERCENT_VALUE	( SIGNAL_CHANNEL_SIGNAL_PER_VALUE * 100)
#define SIGNAL_CHANNEL_OFF_VALUE			( SIGNAL_MINIMUM_VALUE - SIGNAL_CHANNEL_SIGNAL_PER_VALUE)

class Signal_Processor;

class Signal_Channel
{
  private:
	mutable int16_t cachedValue;

	void updatePoint( uint8_t PointId);

  public:
	Setup_Channel Setup;

	uint8_t InputSignalSourceId;
	uint8_t TrimSignalSourceId;
	uint8_t LimitSignalSourceId;

	// Be aware that to connect to sources, they all of them have to be loaded by the
	// SignalProcessor.
	void Initialize( void);
	void Reset( void);

	int16_t CalculateValue( Signal_Processor* SignalProcessor);
	// Get last calculated value.
	int16_t GetValue( void);
};

#endif
