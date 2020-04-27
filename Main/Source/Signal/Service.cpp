// Copyright 2007 Peter Stegemann

#include "Service.h"

#include "Ports.h"
#include "Setup/PPM.h"
#include "Signal/Utility.h"

#include "AVR/Components/Utility.h"

#include <avr/io.h>	
#include <avr/interrupt.h>
#include <util/delay.h>

#define SIGNAL_SERVICE_MATCH_INTERUPT_1				TIMER3_COMPA_vect
#define SIGNAL_SERVICE_MATCH_INTERUPT_2				TIMER3_COMPB_vect

#define SIGNAL_SERVICE_MATCH_INTERUPT_1_ENABLE		OCIE3A
#define SIGNAL_SERVICE_MATCH_INTERUPT_2_ENABLE		OCIE3B

static Signal_Service* SignalServiceSingleton;

Signal_Service::Signal_Service( void)
			  : ppm0( 0)
			  , ppm1( 1)
{
	SignalServiceSingleton = this;
}

void Signal_Service::ProcessSignal( uint8_t Index)
{
	if( Index == 0)
	{
		ppm0.ProcessSignal();
	}
	else
	{
		ppm1.ProcessSignal();
	}
}

ISR( SIGNAL_SERVICE_MATCH_INTERUPT_1)
{
	SignalServiceSingleton->ProcessSignal( 0);
}

ISR( SIGNAL_SERVICE_MATCH_INTERUPT_2)
{
	SignalServiceSingleton->ProcessSignal( 1);
}

void Signal_Service::Start( void)
{
	// Clear counter before use. This will also clear all other settings.
	TCNT3 = 0;

	ppm0.Start();
	ppm1.Start();

	// No clear of timer by match and split clock for timer by 8.
	TCCR3B = UTILITY_BitValue( CS30);
	// Switch interrupt for compare match on.
	TIMSK3 = UTILITY_BitValue( SIGNAL_SERVICE_MATCH_INTERUPT_1_ENABLE) |
			 UTILITY_BitValue( SIGNAL_SERVICE_MATCH_INTERUPT_2_ENABLE);
}

Signal_PPM* Signal_Service::GetPPM( uint8_t Index)
{
	if( Index == 0)
	{
		return &ppm0;
	}
	else
	{
		return &ppm1;
	}
}
