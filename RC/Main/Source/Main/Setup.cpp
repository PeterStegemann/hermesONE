// Copyright 2010 Peter Stegemann

#include "Setup.h"

#include "Screen/Setup/Main.h"
#include "Screen/Setup/Status.h"
#include "Screen/Status/Menu/Main.h"
#include "Text/Text.h"

#include "AVR/Source/Utility.h"

extern unsigned int __data_start;
extern unsigned int __data_end;
extern unsigned int __bss_start;
extern unsigned int __bss_end;
extern unsigned int __noinit_start;
extern unsigned int __noinit_end;
extern unsigned int __heap_start;
extern unsigned int __heap_end;

Main_Setup::Main_Setup( void)
    : lastSequence( 0)
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
	Screen_Status_Menu_Main MainScreen( &inputService, &signalService, &statusBattery);
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
	Screen_Setup_Status SetupStatusScreen
	(
	    &inputService, &statusBattery, &statusTime, &statusScreen
    );
	SetupStatusScreen.Run();
}

void Main_Setup::doSetupMain( void)
{
	Screen_Setup_Main SetupMainScreen
	(
	    &inputService, &interruptService, &signalService, &statusBattery, &statusScreen
    );
	SetupMainScreen.Run();
}

void Main_Setup::blank( void)
{
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

void Main_Setup::Update( void)
{
	Main_Base::Update();

    blank();

    if( debug == false)
    {
        return;
    }

	uint16_t Sequence = signalService.GetPPM( 0)->GetSequence();

	if( Sequence != lastSequence)
	{
    	lastSequence = Sequence;

    	uint16_t Left = 1;
    	uint16_t Top = 1;

		SetupDisplay.PrintFormat
		(
		    Left, Top, avr::font::FI_Mini, LCD_65K_RGB::C_Red, LCD_65K_RGB::C_Black, LCD::PO_Fixed,
            "Stack: %04x", RAMEND - SP
        );
		SetupDisplay.PrintFormat
		(
		    Left, Top += 10, avr::font::FI_Mini, LCD_65K_RGB::C_Red, LCD_65K_RGB::C_Black, LCD::PO_Fixed,
            "Free:  %04x", SP - __data_start
        );
/*
		GLOBAL.SetupDisplay.PrintFormat
		(
		    Left, Top, avr::font::FI_Mini, LCD_65K_RGB::C_Red, LCD_65K_RGB::C_Black, LCD::PO_Fixed,
		    "data:   %04x %04x", __data_start, __data_end
        );
		GLOBAL.SetupDisplay.PrintFormat
		(
    		Left, Top += 10, avr::font::FI_Mini, LCD_65K_RGB::C_Red, LCD_65K_RGB::C_Black, LCD::PO_Fixed,
            "bss:    %04x %04x", __bss_start, __bss_end
        );
		GLOBAL.SetupDisplay.PrintFormat
		(
    		Left, Top += 10, avr::font::FI_Mini, LCD_65K_RGB::C_Red, LCD_65K_RGB::C_Black, LCD::PO_Fixed,
            "noinit: %04x %04x", __noinit_start, __noinit_end
        );
		GLOBAL.SetupDisplay.PrintFormat
		(
    		Left, Top += 10, avr::font::FI_Mini, LCD_65K_RGB::C_Red, LCD_65K_RGB::C_Black, LCD::PO_Fixed,
            "heap:   %04x %04x", __heap_start, __heap_end
        );
		GLOBAL.SetupDisplay.PrintFormat
		(
		    Left, Top += 10, avr::font::FI_Mini, LCD_65K_RGB::C_Red, LCD_65K_RGB::C_Black, LCD::PO_Fixed,
            "RAMEND: %04x SP: %04x size: %04x", RAMEND, SP, RAMEND - SP
        );
*/
/*
		//  	uint8_t Diff = Sequence - lastSequence;
        //		uint8_t Frames = 450 / Diff;

        GLOBAL.SetupDisplay.PrintFormat( frameLeft + 250, frameTop + 1, avr::font::FI_Mini,
										 LCD_65K_RGB::C_Red, LCD_65K_RGB::C_Black, LCD::PO_Fixed,
										 "%u %d %d.%01d  ", Sequence,
										 signalService->GetPPM( 0)->GetWaited(), Frames / 10,
										 Frames % 10);
*/
		//		avr::Utility::PrintByteBits( &( GLOBAL.SetupDisplay), frameLeft + 250, frameTop + 10, "A: ", PINA);
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
