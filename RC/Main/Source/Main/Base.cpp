// Copyright 2010 Peter Stegemann

#include "Base.h"

#include <avr/interrupt.h>
#include <avr/wdt.h>

uint8_t ResetReason __attribute__ (( section ( ".noinit")));

void ResetWatchdog( void) __attribute__(( naked)) __attribute__(( section( ".init3")));

void ResetWatchdog( void)
{
	ResetReason = MCUSR;
	MCUSR = 0;
	wdt_disable();
}
