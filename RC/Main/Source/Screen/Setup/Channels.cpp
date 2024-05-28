// Copyright 2008 Peter Stegemann

#include "Channels.h"

#include "Channel.h"
#include "GUI/Setup/Select.h"
#include "Main/Global.h"
#include "Setup/Model.h"
#include "Setup/Utility.h"
#include "Text/Text.h"

#include "AVR/Source/Utility.h"

#include <string.h>

enum Menu
{
	M_Exit			= 0,
	M_RFMode		= 2,
	M_ChannelStart	= 4
};

Screen_Setup_Channels::Screen_Setup_Channels( void)
					 : Screen_Setup_BaseList( Text::Channels)
					 , selectedModelId( GLOBAL.SetupService.GetSelectedModelId())
			         , rfMode( GLOBAL.SetupService.GetRFMode( selectedModelId))
{
	visibleLines = SIGNAL_PPM_CHANNELS;

	for( uint8_t ChannelLine = 0; ChannelLine < visibleLines; ChannelLine++)
	{
		channelLabel[ ChannelLine].SetText( channelName[ ChannelLine]);

		valueGauge[ ChannelLine].SetOptions(
			( GUI_Setup_Gauge::Options)( GUI_Setup_Gauge::O_Percentage |
										 GUI_Setup_Gauge::O_DualPercentage |
										 GUI_Setup_Gauge::O_CenterLine |
										 GUI_Setup_Gauge::O_Marker));
	}

	rfModeLabel.SetText( rfModeName);
}

void Screen_Setup_Channels::display( void)
{
	displayMarker();

	// Adjust gauges to frame and set them up.
	const avr::font::Type* Font = avr::font::Font::Get( SCREEN_SETUP_BASE_MAIN_FONT);

	uint16_t ContentLeft = frameLeft + 12 * Font->GetCellWidth();

	uint16_t GaugeLeft = menuLeft + SETUP_CHANNEL_NAME_SIZE * Font->GetCellWidth();
	uint16_t GaugeWidth = frameWidth - ( GaugeLeft - frameLeft) - 1;	

	uint8_t Line = 0;

	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop, SCREEN_SETUP_BASE_MAIN_FONT,
								 LCD_65K_RGB::C_WarmYellow, LCD_65K_RGB::C_Black,
								 LCD::PO_Proportional, Text::Exit);

	Line += 2;

	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::RFMode);

	rfModeLabel.SetDimensions( ContentLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT));
	updateRFMode( &rfModeLabel, rfMode);

	Line++;

	setupScrollMarkers( Line);

	for( uint8_t ChannelLine = 0; ChannelLine < visibleLines; ChannelLine++)
	{
		uint16_t LineTop = frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT);

		channelLabel[ ChannelLine].SetDimensions( menuLeft, LineTop);
		valueGauge[ ChannelLine].SetDimensions( GaugeLeft, LineTop, GaugeWidth,
												SCREEN_SETUP_BASE_GAUGE_THICKNESS);

		Line++;
	}

	reDisplay();
}

void Screen_Setup_Channels::reDisplay( void)
{
	// Clear all channels.
	for( uint8_t SourceLine = 0; SourceLine < visibleLines; SourceLine++)
	{
		valueGauge[ SourceLine].Clear();
	}

	// Update labels.
	for( uint8_t ChannelLine = 0; ChannelLine < visibleLines; ChannelLine++)
	{
		channelLabel[ ChannelLine].Clear();
		GLOBAL.SetupService.GetChannelName( firstLine + ChannelLine, channelName[ ChannelLine],
										    SETUP_CHANNEL_NAME_SIZE + 1);
		channelLabel[ ChannelLine].Display();
	}

	reDisplayMarkers( SIGNAL_PPM_CHANNELS);
}

void Screen_Setup_Channels::update( void)
{
	Screen_Setup_BaseList::update();

	// Print all channels.
	for( uint8_t ChannelLine = 0; ChannelLine < visibleLines; ChannelLine++)
	{
		valueGauge[ ChannelLine].Display( SIGNAL_MINIMUM_VALUE, SIGNAL_MAXIMUM_VALUE,
										  GLOBAL.SignalProcessor.GetChannelValue(
											  firstLine + ChannelLine));
	}
}

bool Screen_Setup_Channels::processMenu( DoMenuResult Result)
{
	switch( Result)
	{
		case DMR_Selected :
		{
			switch( currentMenuEntry)
			{
				case 0 : return( false);

				case 1 :
				{
					bool ValueChanged = GUI_Setup_Select::DoSelect(
						( int8_t*) &rfMode, Setup_Service::RF_None, Setup_Service::RF_Both, 1,
						&menuMarker, &rfModeLabel, this, &staticUpdate, &updateRFMode);

					// Store new values.
					if( ValueChanged == true)
					{
						GLOBAL.SetupService.SetRFMode( selectedModelId, rfMode);
					}
				}
				break;

				default :
				{
					uint8_t ChannelIndex = ( currentMenuEntry - firstLine) - 2;

					doSelect( ChannelIndex);
				}
				break;
			}
		}
		break;

		case DMR_Changed :
		{
			doChanged();
		}
		break;

		default : break;
	}

	return( true);
}

void Screen_Setup_Channels::displayMarker( void)
{
	uint8_t MarkerLine = currentMenuEntry;

	if( MarkerLine > M_Exit)
	{
		// Add one for the gap after exit.
		MarkerLine++;
	}

	if( MarkerLine > M_RFMode)
	{
		// Add another one for the gap after fixed entries.
		MarkerLine += 1;
	}

	menuMarker.Display( markerLeft, markerTop + ( MarkerLine * SCREEN_SETUP_BASE_LINE_HEIGHT));
}

void Screen_Setup_Channels::doSelect( uint8_t LineId)
{
	// Just to play safe...
	if( LineId >= visibleLines)
	{
		return;
	}

	Screen_Setup_Channel ChannelScreen( firstLine + LineId);
	ChannelScreen.Run();

	ReDisplay();
}

void Screen_Setup_Channels::doChanged( void)
{
	if( Screen_Setup_BaseList::doChanged( SIGNAL_PPM_CHANNELS, 2))
	{
		reDisplay();
	}

	displayMarker();
}

void Screen_Setup_Channels::updateRFMode( void* Object, GUI_Setup_Label* Label, int8_t Value)
{
	(( Screen_Setup_Channels*) Object)->updateRFMode( Label, Value);
}

void Screen_Setup_Channels::updateRFMode( GUI_Setup_Label* Label, int8_t Value)
{
	Label->Clear();

	Setup_Utility::GetRFModeName( Value, rfModeName, sizeof( rfModeName));

	GLOBAL.SignalProcessor.SetRFMode(( Setup_Service::RFMode) Value);

	Label->Display();
}
