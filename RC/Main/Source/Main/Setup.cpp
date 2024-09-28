// Copyright 2010 Peter Stegemann

#include "Setup.h"

#include "Screen/Setup/Main.h"
#include "Screen/Setup/Status.h"
#include "Screen/Status/Menu/Main.h"
#include "Text/Text.h"

#include "AVR/Source/Utility.h"

Main_Setup::Main_Setup( void)
{
}

Main_Setup::~Main_Setup( void)
{
}

void Main_Setup::run( void)
{
    setupBlankTime = SetupService.GetSetupBlankTime();
	statusBlankTime = SetupService.GetStatusBlankTime();

  	// Set up setup lcd and check if it's there.
    hasSetupDisplay = SetupDisplay.Initialize( 5);

	if(	hasSetupDisplay == false)
	{
		runOnStatusDisplay();
	}
	else
	{
	    avr::Utility::Pause( 50);

		runOnSetupDisplay();
	}
}

void Main_Setup::runOnStatusDisplay( void)
{
	// Loop between status and menu forever.
	while( true)
	{
		doStatusStatus();
		doStatusMain();
	}
}

void Main_Setup::doStatusStatus( void)
{
	statusScreen.Run();
}

void Main_Setup::doStatusMain( void)
{
	Screen_Status_Menu_Main MainScreen( &inputService, &statusBattery);
	MainScreen.Run();
}

void Main_Setup::runOnSetupDisplay( void)
{
	SetupDisplay.SetOrientation(( LCD::Orientation)( LCD::O_Landscape));
	SetupDisplay.SetBacklight( SetupService.GetSetupBacklight());

	// Set up head screen.
	statusScreen.ReDisplay();

	// Loop between status and start menu forever.
	while( true)
	{
		doSetupStatus();
		doSetupMain();
	}
}

void Main_Setup::doSetupStatus( void)
{
	Screen_Setup_Status SetupStatusScreen( &inputService, &statusBattery, &statusTime, &statusScreen);
	SetupStatusScreen.Run();
}

void Main_Setup::doSetupMain( void)
{
	Screen_Setup_Main SetupMainScreen
	(
	    &inputService, &interruptService, &statusBattery, &statusScreen
    );
	SetupMainScreen.Run();
}

void Main_Setup::Update( void)
{
	Main_Base::Update();

	if(( setupBlankTime == 0) && ( statusBlankTime == 0))
	{
	    return;
	}

    uint16_t CurrentUptime = statusTime.GetUptime();
    uint16_t LastActivityUptime = inputService.GetLastActivityUptime();

    if( hasSetupDisplay == true)
    {
        // Blank setup screen if the user sleeps.
        if(( setupBlankTime != 0) && (( CurrentUptime - LastActivityUptime) >= setupBlankTime))
        {
            SetupDisplay.SetBlanked( true);
        }
        else
        {
            SetupDisplay.SetBlanked( false);
        }
    }

    // Blank status screen if the user sleeps.
    if(( statusBlankTime != 0) && (( CurrentUptime - LastActivityUptime) >= statusBlankTime))
    {
        StatusDisplay.SetBlanked( true);
    }
    else
    {
        StatusDisplay.SetBlanked( false);
    }
}

void Main_Setup::ClearScreens( void)
{
	Main_Base::ClearScreens();

	GLOBAL.SetupDisplay.Clear( LCD_65K_RGB::C_Black);
}

void Main_Setup::SetSetupBlankTime( uint16_t SetupBlankTime)
{
	setupBlankTime = SetupBlankTime;
}

void Main_Setup::SetStatusBlankTime( uint16_t StatusBlankTime)
{
	statusBlankTime = StatusBlankTime;
}
