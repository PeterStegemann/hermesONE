// Copyright 2010 Peter Stegemann

#include "Channel.h"

#include "Setup/Channel.h"
#include "Text/Text.h"

#include "AVR/Components/Utility.h"
#include "AVR/Components/Font/Font.h"
#include "AVR/Components/LCD/LCD_DOG.h"

#define MENU_EXIT		0
#define MENU_REVERSE	1
#define MENU_LOW		2
#define MENU_CENTER		3
#define MENU_HIGH		4
#define MENU_INPUT		5
#define MENU_TRIM		6
#define MENU_LIMIT		7
#define MENU_COUNT		8

Screen_Status_Menu_Channel::Screen_Status_Menu_Channel( uint8_t ChannelId)
						  : Screen_Status_Menu_Base( MENU_COUNT, L_Three)
						  , channelId( ChannelId)
{
	channel = GLOBAL.SignalProcessor.GetChannel( channelId);
}

Screen_Status_Menu_Channel::~Screen_Status_Menu_Channel( void)
{
}

void Screen_Status_Menu_Channel::updateSource( void* Object, uint16_t SetupSourceId)
{
	(( Screen_Status_Menu_Channel*) Object)->SetSource(
		( Level)((( Screen_Status_Menu_Channel*) Object)->menuLevel + 1), SetupSourceId);
}

void Screen_Status_Menu_Channel::updatePoint( uint8_t PointId)
{
	updatePointValue( channel->Setup.PointVolume[ PointId]);
}

void Screen_Status_Menu_Channel::updatePointValue( int16_t Value)
{
	char String[ 6];

	snprintf_P( String, sizeof( String), Text::FourDigitPercentFormat,
				Value / SIGNAL_CHANNEL_SIGNAL_PER_VALUE);

	SetText(( Level)( menuLevel + 1), String);
}

void Screen_Status_Menu_Channel::updatePointValue( void* Object, int16_t Value)
{
	(( Screen_Status_Menu_Channel*) Object)->updatePointValue( Value);
}

bool Screen_Status_Menu_Channel::changePoint( uint8_t PointId)
{
	return( selectValue( &( channel->Setup.PointVolume[ PointId]), SIGNAL_MINIMUM_VALUE,
						 SIGNAL_MAXIMUM_VALUE, SIGNAL_CHANNEL_SIGNAL_PER_VALUE, this,
						 &updatePointValue));
}

void Screen_Status_Menu_Channel::display( void)
{
	pickMenu( MENU_REVERSE);
}

bool Screen_Status_Menu_Channel::processMenu( DoMenuResult Result)
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

				case MENU_REVERSE :
				{
					SetText_P( Text::Reverse);

					updateBoolean( channel->Setup.Reverse);
				}
				break;

				case MENU_LOW :
				{
					SetText_P( Text::Bottom);

					updatePoint( Setup_Channel::PV_Low);
				}
				break;

				case MENU_CENTER :
				{
					SetText_P( Text::Center);

					updatePoint( Setup_Channel::PV_Center);
				}
				break;

				case MENU_HIGH :
				{
					SetText_P( Text::Top);

					updatePoint( Setup_Channel::PV_High);
				}
				break;

				case MENU_INPUT :
				{
					SetText_P( Text::Input);

					SetSource(( Level)( menuLevel + 1), channel->Setup.InputSource.Source);
				}
				break;

				case MENU_TRIM :
				{
					SetText_P( Text::Trim);
					
					SetSource(( Level)( menuLevel + 1), channel->Setup.TrimSource.Source);
				}
				break;

				case MENU_LIMIT :
				{
					SetText_P( Text::Limit);
					
					SetSource(( Level)( menuLevel + 1), channel->Setup.LimitSource.Source);
				}
				break;
			}
		}
		break;

		case DMR_Selected :
		{
			bool SourceChanged = false;

			switch( currentMenuEntry)
			{
				case MENU_EXIT : return( false);

				case MENU_REVERSE :
				{
					channel->Setup.Reverse = UTILITY::Invert( channel->Setup.Reverse);

					updateBoolean( channel->Setup.Reverse);

					SourceChanged = true;
				}
				break;

				case MENU_INPUT :
				{
					SourceChanged = selectSource(
						&( channel->InputSignalSourceId), &( channel->Setup.InputSource.Source),
						this, &updateSource, Signal_Source_Source::L_Model);					
				}
				break;

				case MENU_TRIM :
				{
					SourceChanged = selectSource(
						&( channel->TrimSignalSourceId), &( channel->Setup.TrimSource.Source),
						this, &updateSource, Signal_Source_Source::L_Model);
				}
				break;

				case MENU_LIMIT :
				{
					SourceChanged = selectSource(
						&( channel->LimitSignalSourceId), &( channel->Setup.LimitSource.Source),
						this, &updateSource, Signal_Source_Source::L_Model);
				}
				break;

				default :
				{
					SourceChanged = changePoint( currentMenuEntry - MENU_LOW);
				}
				break;
			}

			if( SourceChanged == true)
			{
				GLOBAL.SetupService.SetChannel( GLOBAL.SetupService.GetSelectedModelId(),
											    channelId, &( channel->Setup));
			}
		}
		break;

		default : break;
	}

	return( true);
}
