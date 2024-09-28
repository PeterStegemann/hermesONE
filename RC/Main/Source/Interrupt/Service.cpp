// Copyright 2009 Peter Stegemann

#include "Interrupt/Service.h"

#include "Ports.h"
#include "Main/Global.h"

#include "AVR/Source/Utility.h"

#include <avr/interrupt.h>

#define INTERRUPT_SERVICE_TICKS_PER_SECOND	1000
#define INTERRUPT_SERVICE_CLOCK_FACTOR		( F_CPU / INTERRUPT_SERVICE_TICKS_PER_SECOND) / 64

static Interrupt_Service* InterruptSingleton;

void Interrupt_Service::Start( void)
{
	timeMillis = 0;

	InterruptSingleton = this;

	// Clear counter before use. This will also clear all other settings.
	TCNT0 = 0;
	// Wakeup often.
	OCR0A = INTERRUPT_SERVICE_CLOCK_FACTOR;
	// Clear timer by match and split clock for timer by 64.
	INPUT_TIMERA = UTILITY_BitValue( WGM01);
	INPUT_TIMERB = UTILITY_BitValue( CS01) | UTILITY_BitValue( CS00);
	// Switch interrupt for compare match on.
	TIMSK0 = UTILITY_BitValue( OCIE0A);
}

void Interrupt_Service::Process( void)
{
	// Time calculation is _always_ done.
	uint16_t NewTimeMillis = timeMillis + 1;

	// Second?
	if( NewTimeMillis == 1000)
	{
		timeMillis = 0;

		statusTime->Process();
	}
	else
	{
		timeMillis = NewTimeMillis;
	}

	inputService->Process();
	signalProcessor->Process();
	statusService->Process();

	// Dezisecond?
	if(( timeMillis % 100) == 0)
	{
		statusBattery->Process();
	}
}

uint16_t Interrupt_Service::GetMillis( void)
{
	return( timeMillis);
}

ISR( TIMER0_COMPA_vect, ISR_NOBLOCK)
{
	InterruptSingleton->Process();
}
