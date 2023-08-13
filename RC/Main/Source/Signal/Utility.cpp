// Copyright 2010 Peter Stegemann

#include "Utility.h"

#include "Processor.h"

int16_t Signal_Utility::AddValues( int16_t First, int16_t Second)
{
	int32_t ResultValue = First;
	ResultValue += Second;
	
	if( ResultValue < SIGNAL_MINIMUM_VALUE)
	{
		ResultValue = SIGNAL_MINIMUM_VALUE;
	}
	else if( ResultValue > SIGNAL_MAXIMUM_VALUE)
	{
		ResultValue = SIGNAL_MAXIMUM_VALUE;
	}
	
	return( ResultValue);
}

int16_t Signal_Utility::VolumizeValue( int16_t Value, int16_t HundredPercentVolume, int16_t Volume)
{
	int32_t ResultValue = Value;

	ResultValue *= Volume;
	ResultValue /= HundredPercentVolume;

	if( ResultValue < SIGNAL_MINIMUM_VALUE)
	{
		ResultValue = SIGNAL_MINIMUM_VALUE;
	}
	else if( ResultValue > SIGNAL_MAXIMUM_VALUE)
	{
		ResultValue = SIGNAL_MAXIMUM_VALUE;
	}

	return( ResultValue);
}

int16_t Signal_Utility::GetVolumizedSourceValue( Signal_Processor* SignalProcessor,
												 uint8_t SignalSourceId,
												 int16_t HundredPercentVolume, int16_t Volume)
{
	int16_t SourceValue;

	if( SignalSourceId == SIGNAL_SOURCE_NONE)
	{
		// No source, use neutral value.
		return( SIGNAL_NEUTRAL_VALUE);
	}
	else if( SignalSourceId == SIGNAL_SOURCE_FIXED)
	{
		int32_t ResultValue = Volume;
		ResultValue *= SIGNAL_MAXIMUM_VALUE;
		ResultValue /= HundredPercentVolume;

		return( ResultValue);
	}
	else
	{
		SourceValue = SignalProcessor->GetSourceValue( SignalSourceId);

		return( VolumizeValue( SourceValue, HundredPercentVolume, Volume));
	}
}
