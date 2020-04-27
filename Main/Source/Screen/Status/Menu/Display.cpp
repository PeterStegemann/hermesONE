// Copyright 2010 Peter Stegemann

#include "Display.h"

#include "Text/Text.h"

#include "AVR/Components/Utility.h"
#include "AVR/Components/Font/Font.h"
#include "AVR/Components/LCD/LCD_DOG.h"

enum MenuEntry
{
	MENU_EXIT,
	MENU_BACKLIGHT,
	MENU_CONTRAST,
	MENU_BLANK,
	MENU_INVERTED,
	MENU_COUNT
};

Screen_Status_Menu_Display::Screen_Status_Menu_Display( void)
						  : Screen_Status_Menu_Base( MENU_COUNT, L_Three)
{
}

Screen_Status_Menu_Display::~Screen_Status_Menu_Display( void)
{
}

void Screen_Status_Menu_Display::display( void)
{
	pickMenu( MENU_BACKLIGHT);
}

void Screen_Status_Menu_Display::updateBacklight( int8_t Value)
{
	char String[ 6];

	snprintf_P( String, sizeof( String), Text::FourDigitPercentFormat,
			    Value * 100 / LCD_DOG_BACKLIGHT_FULL);

	SetText(( Level)( menuLevel + 1), String);

	GLOBAL.StatusDisplay.SetBacklight( Value);
}

void Screen_Status_Menu_Display::updateBacklight( void* Object, int8_t Value)
{
	(( Screen_Status_Menu_Display*) Object)->updateBacklight( Value);
}

void Screen_Status_Menu_Display::updateContrast( int8_t Value)
{
	char String[ 6];
	
	snprintf_P( String, sizeof( String), Text::FourDigitPercentFormat,
			    Value * 100 / LCD_DOG_CONTRAST_FULL);

	SetText(( Level)( menuLevel + 1), String);

	GLOBAL.StatusDisplay.SetContrast( Value);
}

void Screen_Status_Menu_Display::updateContrast( void* Object, int8_t Value)
{
	(( Screen_Status_Menu_Display*) Object)->updateContrast( Value);
}

void Screen_Status_Menu_Display::updateBlankTime( uint16_t Value)
{
	char String[ 6];

	snprintf_P( String, sizeof( String), Text::TimeFormat, Value / 60, Value % 60);

	SetText(( Level)( menuLevel + 1), String);

	GLOBAL.SetStatusBlankTime( Value);
}

void Screen_Status_Menu_Display::updateBlankTime( void* Object, uint16_t Value)
{
	(( Screen_Status_Menu_Display*) Object)->updateBlankTime( Value);
}

bool Screen_Status_Menu_Display::processMenu( DoMenuResult Result)
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

				case MENU_BACKLIGHT :
				{
					SetText_P( Text::Backlight);

					updateBacklight( GLOBAL.StatusDisplay.GetBacklight());
				}
				break;

				case MENU_CONTRAST :
				{
					SetText_P( Text::Contrast);

					updateContrast( GLOBAL.StatusDisplay.GetContrast());
				}
				break;

				case MENU_BLANK :
				{
					SetText_P( Text::BlankTime);

					updateBlankTime( GLOBAL.SetupService.GetStatusBlankTime());
				}
				break;

				case MENU_INVERTED :
				{
					SetText_P( Text::StatusInverted);

					updateBoolean( GLOBAL.StatusDisplay.GetInverted());
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

				case MENU_BACKLIGHT :
				{
					int8_t Value = GLOBAL.StatusDisplay.GetBacklight();

					if( selectValue( &Value, LCD_DOG_BACKLIGHT_OFF, LCD_DOG_BACKLIGHT_FULL,
									 LCD_DOG_BACKLIGHT_STEPS, this, &updateBacklight) == true)
					{
						GLOBAL.SetupService.SetStatusBacklight( Value);
					}
				}
				break;

				case MENU_CONTRAST :
				{
					int8_t Value = GLOBAL.StatusDisplay.GetContrast();

					if( selectValue( &Value, LCD_DOG_CONTRAST_OFF, LCD_DOG_CONTRAST_FULL,
									 LCD_DOG_CONTRAST_STEPS, this, &updateContrast) == true)
					{
						GLOBAL.SetupService.SetStatusContrast( Value);
					}
				}
				break;

				case MENU_BLANK :
				{
					uint16_t Value = GLOBAL.SetupService.GetStatusBlankTime();

					if( selectValue( &Value, 0, 300, 5, this, &updateBlankTime) == true)
					{
						GLOBAL.SetupService.SetStatusBlankTime( Value);
					}
				}
				break;
					
				case MENU_INVERTED :
				{
					bool Value = GLOBAL.SetupService.GetStatusInverted();

					Value = UTILITY::Invert( Value);

					updateBoolean( Value);

					GLOBAL.StatusDisplay.SetInverted( Value);
					GLOBAL.SetupService.SetStatusInverted( Value);
				}
				break;
			}

			drawMenuMarker();
		}
		break;

		default : break;
	}

	return( true);
}
