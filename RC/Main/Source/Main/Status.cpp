// Copyright 2010 Peter Stegemann

#include "Status.h"

#include "Screen/Status/Menu/Main.h"

#include <avr/wdt.h>

Main_Status MainInstance;

int main( void)
{
	// Disable watchdog.
	wdt_disable();

	MainInstance.Run();

	return( 0);
}

Main_Status::Main_Status( void)
		   : statusBlankTime( SetupService.GetStatusBlankTime())
{
}

Main_Status::~Main_Status( void)
{
}

void Main_Status::run( void)
{
	// Loop between status and menu forever.
	while( true)
	{
		doStatusStatus();
		doStatusMain();
	}
}

void Main_Status::doStatusStatus( void)
{
	StatusScreen.Run();
}

void Main_Status::doStatusMain( void)
{
	Screen_Status_Menu_Main MainScreen;
	MainScreen.Run();
}

void Main_Status::Update( void)
{
	Main_Base::Update();
	
	if( statusBlankTime != 0)
	{
		uint16_t CurrentUptime = GLOBAL.StatusTime.GetUptime();
		uint16_t LastActivityUptime = GLOBAL.InputService.GetLastActivityUptime();

		// Blank status screen if the user sleeps.
		if(( CurrentUptime - LastActivityUptime) >= statusBlankTime)
		{
			StatusDisplay.SetBlanked( true);
		}
		else
		{
			StatusDisplay.SetBlanked( false);
		}
	}
}

void Main_Status::SetStatusBlankTime( uint16_t StatusBlankTime)
{
	statusBlankTime = StatusBlankTime;
}
