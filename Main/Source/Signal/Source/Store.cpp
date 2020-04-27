// Copyright 2009 Peter Stegemann

#include "Store.h"

#include "../Processor.h"
#include "../Utility.h"
#include "Setup/Defines.h"

void Signal_Source_Store::Initialize( void)
{
	offset = Setup.InitVolume;
	value = offset; 

	SignalSourceId = GLOBAL.SignalProcessor.GetSignalSourceId( Setup.InputSource);

	modified = false;
}

void Signal_Source_Store::Reset( void)
{
	offset = SIGNAL_NEUTRAL_VALUE;
	value = offset;

	SignalSourceId = SIGNAL_SOURCE_NONE;
	Setup.InitVolume = offset;
	Setup.InputSource = SETUP_SOURCE_NONE;

	modified = true;
}

void Signal_Source_Store::LoadSetup( uint16_t SetupSourceId)
{
	GLOBAL.SetupService.GetSourceStore( SetupSourceId, &Setup);
}

void Signal_Source_Store::StoreIfModified( uint16_t SetupSourceId)
{
	if( modified == true)
	{
		modified = false;

		GLOBAL.SetupService.SetSourceStore( SetupSourceId, &Setup);
	}
}

int16_t Signal_Source_Store::CalculateValue( Signal_Processor* SignalProcessor)
{
	int16_t NewValue =
		Signal_Utility::AddValues( offset, SignalProcessor->GetSourceValue( SignalSourceId));

	if( NewValue != value)
	{
		Setup.InitVolume = NewValue;
		modified = true;
	}

	value = NewValue;

	return( NewValue);
}

void Signal_Source_Store::ResetValue( void)
{
	offset = SIGNAL_NEUTRAL_VALUE;
	value = offset;
	Setup.InitVolume = offset;

	modified = true;
}
