// Copyright 2010 Peter Stegemann

#include "Main.h"

#include "Calibrations.h"
#include "Channels.h"
#include "PPM.h"
#include "Status.h"
#include "System.h"
#include "Types.h"
#include "Text/Text.h"

#include "AVR/Components/Utility.h"
#include "AVR/Components/Font/Font.h"
#include "AVR/Components/LCD/LCD_DOG.h"

enum MenuEntry
{
	MENU_EXIT,
	MENU_MODELS,
	MENU_CHANNELS,
	MENU_STATUS,
	MENU_SYSTEM,
	MENU_PPM_0,
	MENU_PPM_1,
	MENU_CALIBRATIONS,
	MENU_COUNT
};

Screen_Status_Menu_Main::Screen_Status_Menu_Main( void)
					   : Screen_Status_Menu_Base( MENU_COUNT, L_One)
{
}

Screen_Status_Menu_Main::~Screen_Status_Menu_Main( void)
{
}

void Screen_Status_Menu_Main::display( void)
{
	GLOBAL.StatusDisplay.Clear();

	pickMenu( MENU_MODELS);
}

bool Screen_Status_Menu_Main::processMenu( DoMenuResult Result)
{
	switch( Result)
	{
		case DMR_Changed :
		{
			switch( currentMenuEntry)
			{
				case MENU_EXIT :			SetText_P( Text::Exit);			break;
				case MENU_MODELS :			SetText_P( Text::Models);		break;
				case MENU_CHANNELS :		SetText_P( Text::Channels);		break;
				case MENU_STATUS :			SetText_P( Text::Status);		break;
				case MENU_SYSTEM :			SetText_P( Text::System);		break;
				case MENU_PPM_0 :			SetText_P( Text::PPM);			break;
				case MENU_PPM_1 :			SetText_P( Text::PPM);			break;
				case MENU_CALIBRATIONS :	SetText_P( Text::Calibration);	break;
			}
		}
		break;

		case DMR_Selected :
		{
			switch( currentMenuEntry)
			{
				case MENU_EXIT : return( false);

				case MENU_MODELS :
				{
					Screen_Status_Menu_Types TypesScreen;
					TypesScreen.Run();
				}
				break;

				case MENU_CHANNELS :
				{
					Screen_Status_Menu_Channels ChannelsScreen;
					ChannelsScreen.Run();
				}
				break;

				case MENU_STATUS :
				{
					Screen_Status_Menu_Status StatusScreen;
					StatusScreen.Run();
				}
				break;

				case MENU_SYSTEM :
				{
					Screen_Status_Menu_System SystemScreen;
					SystemScreen.Run();
				}
				break;

				case MENU_PPM_0 :
				{
					Screen_Status_Menu_PPM PPMScreen( 0);
					PPMScreen.Run();
				}
				break;

				case MENU_PPM_1 :
				{
					Screen_Status_Menu_PPM PPMScreen( 1);
					PPMScreen.Run();
				}
				break;

				case MENU_CALIBRATIONS :
				{
					Screen_Status_Menu_Calibrations CalibrationsScreen;
					CalibrationsScreen.Run();
				}
				break;
			}

			clearSubMenuLine();
		}
		break;

		default : break;
	}

	return( true);
}
