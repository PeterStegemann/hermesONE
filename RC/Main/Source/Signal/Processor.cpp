// Copyright 2007 Peter Stegemann

#include "Processor.h"

#include "Main/Global.h"
#include "Setup/Defines.h"

#include "AVR/Source/Utility.h"

bool Signal_Processor::isRFModuleActive( Setup_Service::RFMode RFMode)
{
    return(( rfMode == RFMode) || ( rfMode == Setup_Service::RF_Both));
}

void Signal_Processor::Initialize( void)
{
	inProcessing = false;
	pauseProcessing = false;

	// Load calibration.
	for( uint8_t ChannelId = 0; ChannelId < SIGNAL_PROCESSOR_ANALOG_INPUTS; ChannelId++)
	{
		GLOBAL.SetupService.GetCalibration( ChannelId, &( analogCalibration[ ChannelId].Setup));
	}

	LoadModel();

	// Set direction of analog ports and turn pull downs off.
	INPUT_ANALOG_A_D_DDR = 0x00;
	INPUT_ANALOG_A_D_PORT = 0x00;

	// Activate ADC with pre-scaler 128 --> 14Mhz/128 = 115.2kHz
	ADCSRA = UTILITY_BitValue( ADEN) | UTILITY_BitValue( ADPS2) | UTILITY_BitValue( ADPS1) | UTILITY_BitValue( ADPS0);
}

void Signal_Processor::LoadModel( void)
{
	// Tell the processing routine to pause the processing and wait until no calculations are running.
	pauseProcessing = true;

	while( inProcessing == true);

	// Now load the model.
	uint8_t SelectedModelId = GLOBAL.SetupService.GetSelectedModelId();
	uint8_t SelectedTypeId = GLOBAL.SetupService.GetSelectedTypeId( SelectedModelId);
	SetRFMode(( Setup_Service::RFMode) GLOBAL.SetupService.GetRFMode( SelectedModelId));

	// Read all channels.
	for( uint8_t ChannelId = 0; ChannelId < SIGNAL_PPM_CHANNELS; ChannelId++)
	{
		GLOBAL.SetupService.GetChannel( SelectedModelId, ChannelId, &( channel[ ChannelId].Setup));
	}

	// Read all sources.
	uint8_t SignalSourceId = 0;

	// Read sources.
	for
	(
        uint16_t SetupSourceId = 0;
	    ( SetupSourceId < SETUP_SOURCES) && ( SignalSourceId < SIGNAL_SOURCES);
	    SetupSourceId++
	)
	{
		uint8_t SourceModelId = GLOBAL.SetupService.GetSourceModelId( SetupSourceId);

		// Make sure the source model id is in the valid range.
		if( !(
			  (/*( SourceModelId >= SETUP_MODELS_START) &&*/ ( SourceModelId < SETUP_MODELS_END)) ||
			  (( SourceModelId >= SETUP_MODEL_TYPES_START) && ( SourceModelId < SETUP_MODEL_TYPES_END)) ||
			  ( SourceModelId == SETUP_MODEL_GLOBAL)
		     )
		  )
		{
			// Bad entry, recover this slot by deleting it.
			GLOBAL.SetupService.SetSourceType( SetupSourceId, Signal_Source_Source::T_Empty);

			continue;
		}

		// Only load sources owned by the current model, type and global ones.
		if
		(
		    ( SourceModelId != SelectedModelId) &&
		    ( SourceModelId != SelectedTypeId) &&
		    ( SourceModelId != SETUP_MODEL_GLOBAL)
        )
		{
			continue;
		}

		// Don't try to load empty sources, this would mix everything up.
		if( source[ SignalSourceId].LoadSetup( SetupSourceId, SourceModelId) == false)
		{
			continue;
		}

		SignalSourceId++;
	}

	// Clear unused sources.
	for( ; SignalSourceId < SIGNAL_SOURCES; SignalSourceId++)
	{
		source[ SignalSourceId].SetType( Signal_Source_Source::T_Empty);
	}

	// Loop channels and set connections between sources.
	for( uint8_t ChannelId = 0; ChannelId < SIGNAL_PPM_CHANNELS; ChannelId++)
	{
		channel[ ChannelId].Initialize();
	}

	// Loop sources and initialize them. This will set start values from the setup as well as
	// connections between sources.
	for( uint8_t CurrentSourceId = 0; CurrentSourceId < SIGNAL_SOURCES; CurrentSourceId++)
	{
		// Tell source to pick up its source connections.
		source[ CurrentSourceId].Initialize();
	}

	// Allow processing again.
	pauseProcessing = false;
}

void Signal_Processor::StoreModifiedSources( void)
{
	// Loop sources and ask them to store modifications.
	for( uint8_t CurrentSourceId = 0; CurrentSourceId < SIGNAL_SOURCES; CurrentSourceId++)
	{
		// Tell source to store itself if modified.
		source[ CurrentSourceId].StoreIfModified();
	}
}

void Signal_Processor::Process( void)
{
	if(( inProcessing == true) || ( pauseProcessing == true))
	{
		return;
	}

	inProcessing = true;

	// Precalculate inputs, these need to be called often for the rotaries.
	for( uint8_t SignalSourceId = 0; SignalSourceId < SIGNAL_SOURCES; SignalSourceId++)
	{
		if( source[ SignalSourceId].GetType() == Signal_Source_Source::T_Input)
		{
			source[ SignalSourceId].FlushCache();
			source[ SignalSourceId].GetValue( this);
		}
	}

	// If ppm services are not ready, we won't calculate new data.
	if(( GLOBAL.SignalService.GetPPM( 0)->ReadyForData() == false) ||
	   ( GLOBAL.SignalService.GetPPM( 1)->ReadyForData() == false))
	{
		// Give way for the next processing loop.
		inProcessing = false;

		return;
	}

	// Get input values.
	for( uint8_t Index = 0; Index < INPUT_ANALOG_PORTS; Index++)
	{
		// Select pin ADCx using MUX and AREF.
		ADMUX = Index;

		// Start conversion
		ADCSRA |= UTILITY_BitValue( ADSC);

		// Wait until conversion is completed.
		while( ADCSRA & UTILITY_BitValue( ADSC));

		// Get converted value.
		rawAnalogValue[ Index] = ADCW;

		// Get calibrated value.
//		int32_t NewValue = ( int32_t) calibratedValue[ Index] + ( int32_t) calibration[ Index].Calibrate( rawValue[ Index]);
//		calibratedValue[ Index] = NewValue / 2;

		if( Index < SIGNAL_PROCESSOR_ANALOG_INPUTS)
		{
			calibratedAnalogValue[ Index] =
				analogCalibration[ Index].Calibrate( rawAnalogValue[ Index]);
		}
	}

	// Prepare sources for new calculation round by flushing the cached values.
	for( uint8_t SignalSourceId = 0; SignalSourceId < SIGNAL_SOURCES; SignalSourceId++)
	{
		source[ SignalSourceId].FlushCache();
	}

	// Precalculate timers, as these always need to be called.
	for( uint8_t SignalSourceId = 0; SignalSourceId < SIGNAL_SOURCES; SignalSourceId++)
	{
		if( source[ SignalSourceId].GetType() == Signal_Source_Source::T_Timer)
		{
			source[ SignalSourceId].GetValue( this);
		}
	}

	// Calculate and set channel values.
	for( uint8_t ChannelId = 0; ChannelId < SIGNAL_PPM_CHANNELS; ChannelId++)
	{
		int16_t Value = channel[ ChannelId].CalculateValue( this);

		// Now set value.
		if( isRFModuleActive( Setup_Service::RF_Module_0))
		{
			GLOBAL.SignalService.GetPPM( 0)->SetChannel( ChannelId, Value);
		}

		if( isRFModuleActive( Setup_Service::RF_Module_1))
		{
			GLOBAL.SignalService.GetPPM( 1)->SetChannel( ChannelId, Value);
		}
	}

	// Activate new channels.
	if( isRFModuleActive( Setup_Service::RF_Module_0))
	{
		GLOBAL.SignalService.GetPPM( 0)->SetChannelsValid();
	}

	if( isRFModuleActive( Setup_Service::RF_Module_1))
	{
		GLOBAL.SignalService.GetPPM( 1)->SetChannelsValid();
	}

	// Give way for the next processing loop.
	inProcessing = false;
}

void Signal_Processor::SetRFMode( Setup_Service::RFMode RFMode)
{
	rfMode = RFMode;

	// Activate the used rf module(s).
    GLOBAL.SignalService.GetPPM( 0)->SetRFEnabled( isRFModuleActive( Setup_Service::RF_Module_0));
    GLOBAL.SignalService.GetPPM( 1)->SetRFEnabled( isRFModuleActive( Setup_Service::RF_Module_1));
}

Signal_Calibration* Signal_Processor::GetCalibration( uint8_t CalibrationId)
{
	return( &( analogCalibration[ CalibrationId]));
}

Signal_Channel* Signal_Processor::GetChannel( uint8_t ChannelId)
{
	if( ChannelId >= SIGNAL_PPM_CHANNELS)
	{
		// Invalid channel.
		return( NULL);
	}

	return( &( channel[ ChannelId]));
}

Signal_Source_Source* Signal_Processor::GetSource( uint8_t SourceId)
{
	if( SourceId >= SIGNAL_SOURCES)
	{
		// Invalid source.
		return( NULL);
	}
	
	return( &( source[ SourceId]));
}
/*
int16_t Signal_Processor::CalculateSourceValue( uint8_t SourceId)
{
	if( SourceId >= SIGNAL_SOURCES)
	{
		// Invalid source ids return the default value.
		return( SIGNAL_NEUTRAL_VALUE);
	}

	return( source[ SourceId].CalculateValue( this));
}
*/
int16_t Signal_Processor::GetSourceValue( uint8_t SourceId)
{
	if( SourceId >= SIGNAL_SOURCES)
	{
		// Invalid source ids return the default value.
		return( SIGNAL_NEUTRAL_VALUE);
	}

	return( source[ SourceId].GetValue( this));
}
/*
int16_t Signal_Processor::CalculateChannelValue( uint8_t ChannelId)
{
	return( channel[ ChannelId].CalculateValue( this));
}
*/
int16_t Signal_Processor::GetChannelValue( uint8_t ChannelId)
{
	return( channel[ ChannelId].GetValue());
}

int16_t Signal_Processor::GetRawAnalogValue( uint8_t InputId) const
{
	if( InputId >= INPUT_ANALOG_PORTS)
	{
		// Invalid input ids return the default value.
		return( 0);
	}

	return( rawAnalogValue[ InputId]);
}

int16_t Signal_Processor::GetCalibratedAnalogValue( uint8_t InputId) const
{
	if( InputId >= SIGNAL_PROCESSOR_ANALOG_INPUTS)
	{
		// Invalid input ids return the default value.
		return( SIGNAL_NEUTRAL_VALUE);
	}

	return( calibratedAnalogValue[ InputId]);
}

bool Signal_Processor::FindNextEmptySource( uint8_t* SourceId)
{
	// Loop sources.
	for( uint8_t CurrentSourceId = 0; CurrentSourceId < SIGNAL_SOURCES; CurrentSourceId++)
	{
		if( source[ CurrentSourceId].GetType() == Signal_Source_Source::T_Empty)
		{
			*SourceId = CurrentSourceId;

			return( true);
		}
	}

	return( false);
}

uint8_t Signal_Processor::FindNextSource( uint8_t Current, int8_t Offset, bool HasFixed,
										  Signal_Source_Source::Level SourceLevel,
										  Signal_Source_Source::Type SourceType)
{
	uint8_t StaticSourcesBottom;

	if( HasFixed == true)
	{
		StaticSourcesBottom = SIGNAL_SOURCE_FIXED;
	}
	else
	{
		StaticSourcesBottom = SIGNAL_SOURCE_NONE;
	}

	int8_t OffsetDirection;

	if( Offset > 0)
	{
		OffsetDirection = 1;
	}
	else
	{
		OffsetDirection = -1;
	}

	while( Offset != 0)
	{
		Current += OffsetDirection;

		if( Current == SIGNAL_SOURCES)
		{
			// Top reached, flip over.
			Current = StaticSourcesBottom;
		}
		else if( Current == ( StaticSourcesBottom - 1))
		{
			// Bottom reached, flip over.
			Current = SIGNAL_SOURCES - 1;
		}

		// Check if this a non-empty and legal place.
		if( Current < StaticSourcesBottom)
		{
			if( Current >= SIGNAL_SOURCES)
			{
				// Skip over illegal sources.
				continue;
			}
			else if( source[ Current].GetType() == Signal_Source_Source::T_Empty)
			{
				// Skip over empty sources.
				continue;
			}
			else if(( SourceType != Signal_Source_Source::T_All) &&
				    ( source[ Current].GetType() != SourceType))
			{
				// Skip over not matching sources.
				continue;
			}
			else if( source[ Current].GetLevel() < SourceLevel)
			{
				// Skip over sources above the given level.
				continue;
			}
		}

		Offset -= OffsetDirection;
	}

	return( Current);
}

uint8_t Signal_Processor::GetSignalSourceId( uint16_t SetupSourceId)
{
	if( SetupSourceId == SETUP_SOURCE_NONE)
	{
		return( SIGNAL_SOURCE_NONE);
	}
	else if( SetupSourceId == SETUP_SOURCE_FIXED)
	{
		return( SIGNAL_SOURCE_FIXED);
	}

	// Loop sources and find this setup source id.
	for( uint8_t CurrentSourceId = 0; CurrentSourceId < SIGNAL_SOURCES; CurrentSourceId++)
	{
		Signal_Source_Source::Type SourceType = source[ CurrentSourceId].GetType();

		if( SourceType == Signal_Source_Source::T_Empty)
		{
			continue;
		}

		if( source[ CurrentSourceId].GetSetupSourceId() == SetupSourceId)
		{
			return( CurrentSourceId);
		}
	}

	return( SIGNAL_SOURCE_NONE);
}
