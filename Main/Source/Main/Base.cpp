// Copyright 2010 Peter Stegemann

#include "Base.h"

#include "AVR/Components/Utility.h"

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

Main_Base::Main_Base( void)
		 : lastStoreModifiedTime( 0)
{
}

Main_Base::~Main_Base( void)
{
}

void Main_Base::Run( void)
{
	// Initialize status.
	StatusService.Initialize();

	// Initialize input.
	InputService.Initialize();

	// Init SPI.
	Spi.Initialize();
	
	// Set up head lcd.
	StatusDisplay.Initialize( SetupService.GetStatusBacklight(), SetupService.GetStatusContrast(),
							  SetupService.GetStatusInverted());

	// Show intro on status screen.
	StatusScreen.ShowIntro();

	// Enable interrupts.
	sei();

	// Run ppm engine.
	SignalService.Start();

	//	StatusService.Beep();

	// Initialize processing.
	SignalProcessor.Initialize();

	// Run timer.
	StatusTime.Initialize();

	// Run interrupt service last.
	InterruptService.Start();

	// Wait a moment for all services to come up.
	UTILITY::Pause( 5);

	run();
}

void Main_Base::Update( void)
{
	uint16_t Uptime = StatusTime.GetUptime();

	// Store modified sources on a regular basis.
	if(( Uptime - lastStoreModifiedTime) >= SYSTEM_STORE_MODIFIED_DELAY)
	{
		lastStoreModifiedTime = Uptime;

		GLOBAL.SignalProcessor.StoreModifiedSources();
	}
}

void Main_Base::ClearScreens( void)
{
	StatusDisplay.Clear();
}
