// Copyright 2007 Peter Stegemann

#include "Service.h"

#include "Ports.h"
#include "Setup/PPM.h"
#include "Signal/Utility.h"

#include "AVR/Source/Utility.h"

#include <avr/io.h>	
#include <avr/interrupt.h>
#include <util/delay.h>

// We're using the /8 clock divider.
//#define SIGNAL_PPM_MS_CLOCK_FACTOR	( F_CPU / 100) / 800
#define SIGNAL_PPM_MS_CLOCK_FACTOR	( F_CPU / 100) / 100

// 22.5ms
#define SIGNAL_PPM_SIGNAL_FRAME		((( uint32_t) 225) * SIGNAL_PPM_MS_CLOCK_FACTOR)
//  0.3ms
#define SIGNAL_PPM_SIGNAL_PAUSE		((( uint32_t)   3) * SIGNAL_PPM_MS_CLOCK_FACTOR)
//  1.2ms (Multiplex)
#define SIGNAL_PPM_SIGNAL_CENTER	((( uint32_t)  12) * SIGNAL_PPM_MS_CLOCK_FACTOR)
//  1.1ms (Spektrum Futaba)
//#define SIGNAL_PPM_SIGNAL_NEUTRAL	((( uint32_t)  11) * SIGNAL_PPM_MS_CLOCK_FACTOR)
//  1.1ms
#define SIGNAL_PPM_SIGNAL_RANGE		((( uint32_t)  11) * SIGNAL_PPM_MS_CLOCK_FACTOR)

#define SIGNAL_PPM_STATE_PAUSE				0
#define SIGNAL_PPM_STATE_SIGNAL				1

Signal_PPM::Signal_PPM( uint8_t Id)
		  : id( Id)
		  , rfEnabled( false)
		  , inverted( false)
		  , center( SIGNAL_PPM_SIGNAL_CENTER)
		  , channelsValid( false)
		  , waited( 0)
		  , sequence( 0)
{
	// Reset all channels.
	for( uint8_t Count = 0; Count < SIGNAL_PPM_CHANNELS; Count++)
	{
		channel[ Count] = center;
	}

	// Reset all channel mappings.
	for( uint8_t Count = 0; Count < SIGNAL_PPM_CHANNELS; Count++)
	{
		channelMapping[ Count] = Count;
	}

	// Reset all signals.
	// To do this, we just call copyChannels once. This will also calculate the correct sync time.
	channelsValid = true;
	copyChannels();
}

void Signal_PPM::copyChannels( void)
{
	if( channelsValid != true)
	{
		// No valid data set yet.
		return;
	}

	// Add up all pauses. One for each channel, one for the sync.
	uint32_t TotalSignalTime = SIGNAL_PPM_SIGNAL_PAUSE * ( SIGNAL_PPM_CHANNELS + 1);

	for( uint8_t Count = 0; Count < SIGNAL_PPM_CHANNELS; Count++)
	{
		uint16_t currentSignal = channel[ channelMapping[ Count]];
		signal[ Count] = currentSignal;
		TotalSignalTime += currentSignal;
	}

	// Sync time is the first signal.
	syncCopy = sync = SIGNAL_PPM_SIGNAL_FRAME - TotalSignalTime;

	channelsValid = false;
}

void Signal_PPM::ProcessSignal( void)
{
	bool Switch = false;
	uint16_t nextInterval = 0;

	// Every high signal/start phase is followed by a low pause.
	if( signalState == SIGNAL_PPM_STATE_PAUSE)
	{
		// Pause phase.
		signalState = SIGNAL_PPM_STATE_SIGNAL;

		// Set pause time.
		nextInterval = SIGNAL_PPM_SIGNAL_PAUSE;

		if( inverted == true)
		{
			// Switch to 1 after this one.
			UTILITY_SetBit( SIGNAL_SERVICE_TIMERA, toggleBit);
		}
		else
		{
			// Switch to 0 after this one.
			UTILITY_ClearBit( SIGNAL_SERVICE_TIMERA, toggleBit);
		}
	}
	else
	{
		// Signal phase.
		Switch = true;

		if( currentSignal == 0)
		{
			// Set signal width for sync signal.
			if( sync > 60000)
			{
				// We're not using 60k, but 50k. This leaves a minimum rest of 10k, which gives us
				// enough time for the interrupt code to run.
				nextInterval = 50000;
				sync -= 50000;

				// Don't switch level and signal on next interrupt.
				Switch = false;
			}
			else
			{
				nextInterval = sync;
			}
		}
		else
		{
			// Set signal width for current signal.
			nextInterval = signal[ currentSignal - 1];
		}
	}

	// Set new match value.
	uint32_t newMatchValue = *ocr;
	newMatchValue += nextInterval;

	if( newMatchValue > 0xffff)
	{
		newMatchValue -= 0xffff;
	}

	*ocr = newMatchValue;

	// Check if we switch to next signal / sync.
	if( Switch == true)
	{
		signalState = SIGNAL_PPM_STATE_PAUSE;

		if( inverted == true)
		{
			// Switch to 0 after this one.
			UTILITY_ClearBit( SIGNAL_SERVICE_TIMERA, toggleBit);
		}
		else
		{
			// Switch to 1 after this one.
			UTILITY_SetBit( SIGNAL_SERVICE_TIMERA, toggleBit);
		}

		// Skip to next signal.
		currentSignal++;

		// After the last signal, we start with sync again and copy all channels.
		if( currentSignal == ( SIGNAL_PPM_CHANNELS + 1))
		{
			currentSignal = 0;

			copyChannels();

			sequence++;
		}
	}
}

void Signal_PPM::Start( void)
{
	Setup_PPM PPM;

	GLOBAL.SetupService.GetPPM( id, &PPM);

	inverted = PPM.Inverted;
	SetCenter( PPM.Center);

	for( uint8_t Index = 0; Index < SIGNAL_PPM_CHANNELS; Index++)
	{
		channelMapping[ Index] = PPM.ChannelMapping[ Index];
	}

	// We will start with the start signal.
	currentSignal = 0;

	// First signal state is "up".
	signalState = SIGNAL_PPM_STATE_SIGNAL;

	if( id == 0)
	{
		// Setup RF module lines first: Set enable switch to output and turn RF off.
		UTILITY_SetBit( SIGNAL_SERVICE_DDR, SIGNAL_SERVICE_PPM_ENABLE_1);
		UTILITY_ClearBit( SIGNAL_SERVICE_PORT, SIGNAL_SERVICE_PPM_ENABLE_1);

		// Set up the output pin.
		UTILITY_SetBit( SIGNAL_SERVICE_DDR, SIGNAL_SERVICE_PPM_OUTPUT_1);

		toggleBit = COM3A0;
		ocr = &OCR3A;

		UTILITY_SetBit( SIGNAL_SERVICE_TIMERA, COM3A1);
	}
	else
	{
		// Setup RF module lines first: Set enable switch to output and turn RF off.
		UTILITY_SetBit( SIGNAL_SERVICE_DDR, SIGNAL_SERVICE_PPM_ENABLE_2);
		UTILITY_ClearBit( SIGNAL_SERVICE_PORT, SIGNAL_SERVICE_PPM_ENABLE_2);

		// Set up the output pin.
		UTILITY_SetBit( SIGNAL_SERVICE_DDR, SIGNAL_SERVICE_PPM_OUTPUT_2);

		toggleBit = COM3B0;
		ocr = &OCR3B;

		UTILITY_SetBit( SIGNAL_SERVICE_TIMERA, COM3B1);
	}

	if( inverted == true)
	{
		// Switch to 1 when matching.
		UTILITY_SetBit( SIGNAL_SERVICE_TIMERA, toggleBit);
	}
	else
	{
		// Switch to 0 when matching.
		UTILITY_ClearBit( SIGNAL_SERVICE_TIMERA, toggleBit);
	}

	// Set some lead in time before the first match.
	*ocr = SIGNAL_PPM_SIGNAL_PAUSE;
}

void Signal_PPM::SetRFEnabled( bool RFEnabled)
{
	if( rfEnabled == RFEnabled)
	{
		return;
	}

	rfEnabled = RFEnabled;

	if( rfEnabled == true)
	{
		if( id == 0)
		{
			UTILITY_SetBit( SIGNAL_SERVICE_PORT, SIGNAL_SERVICE_PPM_ENABLE_1);
		}
		else
		{
			UTILITY_SetBit( SIGNAL_SERVICE_PORT, SIGNAL_SERVICE_PPM_ENABLE_2);
		}
	}
	else
	{
		if( id == 0)
		{
			UTILITY_ClearBit( SIGNAL_SERVICE_PORT, SIGNAL_SERVICE_PPM_ENABLE_1);
		}
		else
		{
			UTILITY_ClearBit( SIGNAL_SERVICE_PORT, SIGNAL_SERVICE_PPM_ENABLE_2);
		}
	}
}

bool Signal_PPM::GetRFEnabled( void)
{
	return( rfEnabled);
}

void Signal_PPM::SetInverted( bool Inverted)
{
	inverted = Inverted;
}

void Signal_PPM::SetCenter( int8_t Center)
{
	int16_t Value = Center;
	Value *= SIGNAL_PPM_MS_CLOCK_FACTOR;
	Value += SIGNAL_PPM_SIGNAL_CENTER;

	center = Value;
}

void Signal_PPM::SetChannelMapping( uint8_t Index, uint8_t ChannelId)
{
	channelMapping[ Index] = ChannelId;
}

void Signal_PPM::SetChannel( uint8_t ChannelId, int16_t Value)
{
	if( ChannelId >= SIGNAL_PPM_CHANNELS)
	{
		// Invalid channel!
		return;
	}

	// Clip to valid values.
	if( Value > SIGNAL_MAXIMUM_VALUE)
	{
		Value = SIGNAL_MAXIMUM_VALUE;
	}
	else if( Value < SIGNAL_MINIMUM_VALUE)
	{
		Value = SIGNAL_MINIMUM_VALUE;
	}

	int32_t SignalValue = Value * SIGNAL_PPM_SIGNAL_RANGE;
	SignalValue /= SIGNAL_VALUE_RANGE;
	SignalValue += center;

	// Wait for the interrupt routine to be ready with fetching the last data set.
	while( ReadyForData() == false)
	{
		_delay_ms( 1);
		waited++;
	}

	channel[ ChannelId] = SignalValue;
}

uint16_t Signal_PPM::GetChannel( uint8_t ChannelId)
{
	if(( rfEnabled == false) || ( ChannelId >= SIGNAL_PPM_CHANNELS))
	{
		// Invalid channel!
		return( SIGNAL_NEUTRAL_VALUE);
	}

	uint16_t SignalValue = signal[ ChannelId];
//	SignalValue -= ppmCenter;
//	SignalValue *= SIGNAL_VALUE_RANGE;
//	SignalValue /= SIGNAL_PPM_SIGNAL_RANGE;

	return( SignalValue);
}

uint32_t Signal_PPM::GetSync( void)
{
	return( syncCopy);
}

void Signal_PPM::SetChannelsValid( void)
{
	channelsValid = true;
}

bool Signal_PPM::ReadyForData( void)
{
	// As long as the old data was not picked up, we're not ready for new data.
	return( channelsValid != true);
}

uint8_t Signal_PPM::GetWaited( void)
{
	return( waited);
}

uint16_t Signal_PPM::GetSequence( void)
{
	return( sequence);
}
