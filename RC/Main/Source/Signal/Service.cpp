// Copyright 2007 Peter Stegemann

#include "Service.h"

#include "Ports.h"
#include "Setup/PPM.h"
#include "Signal/Utility.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

static Signal_Service* SignalServiceSingleton;

Signal_Service::Signal_Service( void)
			  : ppm0( 0)
			  , ppm1( 1)
{
	SignalServiceSingleton = this;
}

ISR( SIGNAL_SERVICE_MATCH_INTERUPT_1)
{
	SignalServiceSingleton->ProcessSignal( 0);
}

ISR( SIGNAL_SERVICE_MATCH_INTERUPT_2)
{
	SignalServiceSingleton->ProcessSignal( 1);
}
