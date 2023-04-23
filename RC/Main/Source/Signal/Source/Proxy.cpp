// Copyright 2013 Peter Stegemann

#include "Store.h"

#include "../Processor.h"
#include "../Utility.h"
#include "Setup/Defines.h"

void Signal_Source_Proxy::Initialize( void)
{
	SignalSourceId = GLOBAL.SignalProcessor.GetSignalSourceId( ReferenceSetup.Source);
}

void Signal_Source_Proxy::Reset( void)
{
	Setup.Slot = SIGNAL_SOURCE_PROXY_SLOT_MINIMUM;
	ReferenceSetup.Source = SETUP_SOURCE_FIXED;
	ReferenceSetup.Volume = SIGNAL_MAXIMUM_VALUE;
}

void Signal_Source_Proxy::LoadSetup( uint16_t SetupSourceId)
{
	GLOBAL.SetupService.GetSourceProxy( SetupSourceId, &Setup);
	GLOBAL.SetupService.GetProxyReference( Setup.Slot, &ReferenceSetup);
}

int16_t Signal_Source_Proxy::CalculateValue( Signal_Processor* SignalProcessor)
{
	int16_t Result =
		Signal_Utility::GetVolumizedSourceValue( SignalProcessor, SignalSourceId,
												 SIGNAL_SOURCE_PROXY_100_PERCENT_VALUE,
												 ReferenceSetup.Volume);

	return( Result);
}
