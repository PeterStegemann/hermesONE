// Copyright 2010 Peter Stegemann

#include "PPM.h"

#include "ChannelMapping.h"
#include "Display.h"
#include "Serial/DesktopConnection.h"
#include "Setup/Battery.h"
#include "Text/Text.h"

#include "AVR/Components/Utility.h"
#include "AVR/Components/Font/Font.h"
#include "AVR/Components/LCD/LCD_DOG.h"

enum MenuEntry
{
	MENU_EXIT,
	MENU_PPM_INVERTED,
	MENU_PPM_CENTER,
	MENU_CHANNEL_MAPPING,
	MENU_COUNT
};

Screen_Status_Menu_PPM::Screen_Status_Menu_PPM( uint8_t PPMId)
					  : Screen_Status_Menu_Base( MENU_COUNT, L_Two)
					  , ppmId( PPMId)
{
	GLOBAL.SetupService.GetPPM( ppmId, &ppmSetup);
}

Screen_Status_Menu_PPM::~Screen_Status_Menu_PPM( void)
{
}

void Screen_Status_Menu_PPM::display( void)
{
	pickMenu( MENU_PPM_INVERTED);
}

bool Screen_Status_Menu_PPM::processMenu( DoMenuResult Result)
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

				case MENU_PPM_INVERTED :
				{
					SetText_P( Text::Inverted);

					updateBoolean( ppmSetup.Inverted);
				}
				break;

				case MENU_PPM_CENTER :
				{
					SetText_P( Text::Center);

//					updateBoolean( PPMInverted);
				}
				break;

				case MENU_CHANNEL_MAPPING :
				{
					SetText_P( Text::ChannelMapping);

					SetText_P(( Level)( menuLevel + 1), NULL);
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

				case MENU_PPM_INVERTED :
				{
					ppmSetup.Inverted = UTILITY::Invert( ppmSetup.Inverted);

					GLOBAL.SetupService.SetPPM( ppmId, &ppmSetup);
					GLOBAL.SignalService.GetPPM( ppmId)->SetInverted( ppmSetup.Inverted);

					updateBoolean( ppmSetup.Inverted);
				}
				break;

				case MENU_PPM_CENTER :
				{
//					PPMInverted = UTILITY_Invert( PPMInverted);

//					GLOBAL.SetupService.SetPPM( ppmId, &ppmSetup);
//					GLOBAL.SignalService.SetPPMInverted( PPMInverted);
					
//					updateBoolean( PPMInverted);
				}
				break;

				case MENU_CHANNEL_MAPPING :
				{
					Screen_Status_Menu_ChannelMapping ChannelMappingScreen( ppmId);
					ChannelMappingScreen.Run();

					GLOBAL.SetupService.GetPPM( ppmId, &ppmSetup);

					clearSubMenuLine();
				}
				break;
			}
		}
		break;

		default : break;
	}

	return( true);
}
