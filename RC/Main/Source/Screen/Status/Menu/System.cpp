// Copyright 2010 Peter Stegemann

#include "System.h"

#include "Battery.h"
#include "Display.h"
#include "Serial/DesktopConnection.h"
#include "Text/Text.h"

#include "AVR/Source/Utility.h"
#include "AVR/Source/Font/Font.h"
#include "AVR/Source/LCD/LCD_DOG.h"

enum MenuEntry
{
	MENU_EXIT,
	MENU_SERIAL,
	MENU_DISPLAY,
	MENU_BATTERY,
	MENU_RESET,
	MENU_COUNT
};

Screen_Status_Menu_System::Screen_Status_Menu_System( void)
						 : Screen_Status_Menu_Base( MENU_COUNT, L_Two)
{
}

Screen_Status_Menu_System::~Screen_Status_Menu_System( void)
{
}

void Screen_Status_Menu_System::display( void)
{
	pickMenu( MENU_SERIAL);
}

bool Screen_Status_Menu_System::processMenu( DoMenuResult Result)
{
	switch( Result)
	{
		case DMR_Changed :
		{
			switch( currentMenuEntry)
			{
				case MENU_EXIT :
				{
					SetText_P( Text::Exit);

					SetText_P(( Level)( menuLevel + 1), NULL);
				}
				break;

				case MENU_SERIAL :
				{
					SetText_P( Text::SerialShort);

					SetText_P(( Level)( menuLevel + 1), NULL);
				}
				break;

				case MENU_DISPLAY :
				{
					SetText_P( Text::Display);
					
					SetText_P(( Level)( menuLevel + 1), NULL);
				}
				break;

				case MENU_BATTERY :
				{
					SetText_P( Text::Battery);

					SetText_P(( Level)( menuLevel + 1), NULL);
				}
				break;

				case MENU_RESET :
				{
					SetText_P( Text::Reset);
				}
				break;
			}
		}
		break;

		case DMR_Selected :
		{
			switch( currentMenuEntry)
			{
				case MENU_EXIT : return( false);

				case MENU_SERIAL :
				{
					drawMenuMarker( menuLevel, false, false);

					SetText_P(( Level)( menuLevel + 1), Text::Cancel);

					Serial_DesktopConnection< SYSTEM_SERIAL_ID> UseConnection;
					// Set up serial.
					UseConnection.Initialize();

					UseConnection.DoSerialConnection();

					clearSubMenuLine();
				}
				break;

				case MENU_DISPLAY :
				{
					Screen_Status_Menu_Display DisplayScreen;
					DisplayScreen.Run();

					clearSubMenuLine();
				}
				break;

				case MENU_BATTERY :
				{
					Screen_Status_Menu_Battery BatteryScreen;
					BatteryScreen.Run();

					clearSubMenuLine();
				}
				break;

				case MENU_RESET :
				{
					GLOBAL.SetupService.Reset();

					GLOBAL.SignalProcessor.LoadModel();

					UTILITY::HardReset();
					// No comin' back.
				}
				break;
			}
		}
		break;

		default : break;
	}

	return( true);
}
