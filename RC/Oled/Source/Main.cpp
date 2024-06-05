// Copyright 2010 Peter Stegemann

#include "Main.h"

#include <avr/wdt.h>

Main MainInstance;

uint8_t ResetReason __attribute__ (( section ( ".noinit")));

void ResetWatchdog( void) __attribute__(( naked)) __attribute__(( section( ".init3")));

void ResetWatchdog( void)
{
	ResetReason = MCUSR;
	MCUSR = 0;
	wdt_disable();
}

int main( void)
{
	// Disable watchdog.
	wdt_disable();

	MainInstance.Run();

	return( 0);
}
