// Copyright 2008 Peter Stegemann

#include "Mix.h"

#include "Main/Global.h"
#include "Setup/Defines.h"
#include "Signal/Processor.h"

void Signal_Source_Mix::Initialize( void)
{
	for( uint8_t Index = 0; Index < SETUP_SOURCE_MIX_INPUTS; Index++)
	{
		SignalSourceId[ Index] =
			GLOBAL.SignalProcessor.GetSignalSourceId( Setup.InputSource[ Index].Source);
	}
}

void Signal_Source_Mix::Reset( void)
{
	for( uint8_t Index = 0; Index < SETUP_SOURCE_MIX_INPUTS; Index++)
	{
		SignalSourceId[ Index] = SIGNAL_SOURCE_NONE;
		Setup.InputSource[ Index].Source = SETUP_SOURCE_NONE;
		Setup.InputSource[ Index].Volume = SIGNAL_SOURCE_MIX_100_PERCENT_VALUE;
	}
}

void Signal_Source_Mix::LoadSetup( uint16_t SetupSourceId)
{
	GLOBAL.SetupService.GetSourceMix( SetupSourceId, &Setup);
}

int16_t Signal_Source_Mix::CalculateValue( Signal_Processor* SignalProcessor)
{
	uint8_t Inputs = 0;
	int32_t Result = 0;

	for( uint8_t Index = 0; Index < SETUP_SOURCE_MIX_INPUTS; Index++)
	{
		uint8_t UseSignalSourceId = SignalSourceId[ Index];

		if( UseSignalSourceId < SIGNAL_SOURCES)
		{
			int32_t Value = SignalProcessor->GetSourceValue( UseSignalSourceId);
			Value *= Setup.InputSource[ Index].Volume;
			Value /= SIGNAL_SOURCE_MIX_100_PERCENT_VALUE;

			Result += Value;

			Inputs++;
		}
		else if( UseSignalSourceId == SIGNAL_SOURCE_FIXED)
		{
			int32_t Value = SIGNAL_MAXIMUM_VALUE;
			Value *= Setup.InputSource[ Index].Volume;
			Value /= SIGNAL_SOURCE_MIX_100_PERCENT_VALUE;

			Result += Value;

			Inputs++;
		}
	}

	if( Result > SIGNAL_MAXIMUM_VALUE)
	{
		Result = SIGNAL_MAXIMUM_VALUE;
	}
	else if( Result < SIGNAL_MINIMUM_VALUE)
	{
		Result = SIGNAL_MINIMUM_VALUE;
	}

	return( Result);
}
