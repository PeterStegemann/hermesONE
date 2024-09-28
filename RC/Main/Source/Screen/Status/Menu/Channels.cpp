// Copyright 2010 Peter Stegemann

#include "Channels.h"

#include "Channel.h"
#include "Main/Global.h"
#include "Text/Text.h"

#include "AVR/Source/Utility.h"
#include "AVR/Source/Font/Font.h"
#include "AVR/Source/LCD/DOG/DOG.h"

#define MENU_EXIT				0
#define MENU_CHANNELS_START		1
#define MENU_COUNT				9

Screen_Status_Menu_Channels::Screen_Status_Menu_Channels( Input_Service* InputService)
						   : Screen_Status_Menu_Base( InputService, MENU_COUNT, L_Two)
{
}

Screen_Status_Menu_Channels::~Screen_Status_Menu_Channels( void)
{
}

void Screen_Status_Menu_Channels::display( void)
{
	pickMenu( MENU_CHANNELS_START);
}

void Screen_Status_Menu_Channels::update( void)
{
	if( currentMenuEntry >= MENU_CHANNELS_START)
	{
		int32_t Value =
			GLOBAL.SignalProcessor.GetChannelValue( currentMenuEntry - MENU_CHANNELS_START);
		Value *= 200;
		Value /= SIGNAL_VALUE_RANGE;

		char String[ 6];

		snprintf_P( String, sizeof( String), Text::FourDigitPercentFormat, Value);

		SetText(( Level)( menuLevel + 2), String);
	}
}

bool Screen_Status_Menu_Channels::processMenu( DoMenuResult Result)
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
					SetText(( Level)( menuLevel + 2), NULL);
				}
				break;

				default :
				{
					char ChannelName[ SETUP_CHANNEL_NAME_SIZE + 1];

					GLOBAL.SetupService.GetChannelName( currentMenuEntry - MENU_CHANNELS_START,
													    ChannelName, sizeof( ChannelName));

					SetText( ChannelName);
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

				default :
				{
					SetText_P(( Level)( menuLevel + 2), NULL);

					Screen_Status_Menu_Channel ChannelScreen
					(
					    inputService, currentMenuEntry - MENU_CHANNELS_START
                    );
					ChannelScreen.Run();

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
