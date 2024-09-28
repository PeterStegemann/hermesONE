// Copyright 2011 Peter Stegemann

#include "ChannelMapping.h"

#include "Main/Global.h"
#include "Text/Text.h"

#include "AVR/Source/Utility.h"
#include "AVR/Source/Font/Font.h"
#include "AVR/Source/LCD/DOG/DOG.h"

#define MENU_EXIT				0
#define MENU_CHANNELS_START		1
#define MENU_COUNT				MENU_CHANNELS_START + SIGNAL_PPM_CHANNELS

Screen_Status_Menu_ChannelMapping::Screen_Status_Menu_ChannelMapping
(
    Input_Service* InputService, uint8_t PPMId
)
    : Screen_Status_Menu_Base( InputService, MENU_COUNT, L_Three)
    , ppmId( PPMId)
{
	GLOBAL.SetupService.GetPPM( ppmId, &ppmSetup);
}

Screen_Status_Menu_ChannelMapping::~Screen_Status_Menu_ChannelMapping( void)
{
}

void Screen_Status_Menu_ChannelMapping::display( void)
{
	pickMenu( MENU_CHANNELS_START);
}

void Screen_Status_Menu_ChannelMapping::updateChannelMapping( int8_t Value)
{
	if( currentMenuEntry >= MENU_CHANNELS_START)
	{
		uint8_t ChannelIndex = currentMenuEntry - MENU_CHANNELS_START;

		char String[ 2];

		snprintf_P( String, sizeof( String), Text::Int8Format, Value + 1);

		SetText(( Level)( menuLevel + 1), String);

		GLOBAL.SignalService.GetPPM( ppmId)->SetChannelMapping( ChannelIndex, Value);
	}
}

void Screen_Status_Menu_ChannelMapping::updateChannelMapping( void* Object, int8_t Value)
{
	(( Screen_Status_Menu_ChannelMapping*) Object)->updateChannelMapping( Value);
}

bool Screen_Status_Menu_ChannelMapping::processMenu( DoMenuResult Result)
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
					SetText(( Level)( menuLevel + 1), NULL);
				}
				break;

				default :
				{
					uint8_t ChannelIndex = currentMenuEntry - MENU_CHANNELS_START;

					char String[ 10];

					snprintf_P( String, sizeof( String), Text::ChannelNameFormat, ChannelIndex + 1);

					SetText( String);
					updateChannelMapping( ppmSetup.ChannelMapping[ ChannelIndex]);
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
					uint8_t ChannelIndex = currentMenuEntry - MENU_CHANNELS_START;

					int8_t* SourceChannel = ( int8_t*) &( ppmSetup.ChannelMapping[ ChannelIndex]);

					if( selectValue( SourceChannel, 0, SIGNAL_PPM_CHANNELS - 1, 1, this,
									 &updateChannelMapping) == true)
					{
						GLOBAL.SetupService.SetPPM( ppmId, &ppmSetup);
					}

					drawMenuMarker();
				}
				break;
			}
		}
		break;

		default : break;
	}

	return( true);
}
