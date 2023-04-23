// Copyright 2007 Peter Stegemann

#ifndef SIGNAL_UTILITY_H
#define SIGNAL_UTILITY_H

#include "AVR/Source/Types.h"

class Signal_Processor;

class Signal_Utility
{
	public:
		static int16_t AddValues( int16_t First, int16_t Second);

		static int16_t VolumizeValue( int16_t Value, int16_t HundretPercentVolume, int16_t Volume);

		static int16_t GetVolumizedSourceValue( Signal_Processor* SignalProcessor,
											    uint8_t SignalSourceId,
											    int16_t HundretPercentVolume, int16_t Volume);
};

#endif
