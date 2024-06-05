// Copyright 2011 Peter Stegemann

#include "PPM.h"

#include "GUI/Setup/Select.h"
#include "Main/Global.h"
#include "Text/Text.h"

#include "AVR/Source/Utility.h"

#include <string.h>

enum Menu
{
	M_Exit			= 0,
	M_Name			= 2,
	M_Inverted		= 3,
	M_Center		= 4,
	M_ChannelStart	= 6
};

static const flash_char* getTitle( uint8_t PPMId)
{
	if( PPMId == 0)
	{
		return( Text::RFModule0);
	}
	else
	{
		return( Text::RFModule1);
	}
}

Screen_Setup_System_PPM::Screen_Setup_System_PPM( uint8_t PPMId)
					   : Screen_Setup_BaseList( getTitle( PPMId))
					   , ppmId( PPMId)
{
	GLOBAL.SetupService.GetPPM( ppmId, &ppmSetup);

	visibleLines = SIGNAL_PPM_CHANNELS;

	GLOBAL.SetupService.GetPPMName( ppmId, ppmName, SETUP_PPM_NAME_SIZE + 1);

	ppmNameLabel.SetText_P( Text::Name);
	ppmNameInput.SetOptions(( GUI_Setup_TextInput::Options)
							( GUI_Setup_TextInput::O_LimitAlphaNumeric));

	for( uint8_t ChannelLine = 0; ChannelLine < visibleLines; ChannelLine++)
	{
		targetChannelLabel[ ChannelLine].SetText( targetChannelName[ ChannelLine]);
		sourceChannelLabel[ ChannelLine].SetText_P( Text::Int8Format);
	}
}

void Screen_Setup_System_PPM::display( void)
{
	displayMarker();

	// Adjust gauges to frame and set them up.
	const avr::font::Type* Font = avr::font::Font::Get( SCREEN_SETUP_BASE_MAIN_FONT);

	uint16_t SourceLeft = menuLeft + SETUP_CHANNEL_NAME_SIZE * Font->GetCellWidth();
	uint16_t ContentLeft = frameLeft + 18 * Font->GetCellWidth();
	uint16_t ContentWidth = frameWidth - ( ContentLeft - frameLeft) - 1;

	uint8_t Line = 0;

	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop, SCREEN_SETUP_BASE_MAIN_FONT,
								 LCD_65K_RGB::C_WarmYellow, LCD_65K_RGB::C_Black,
								 LCD::PO_Proportional, Text::Exit);

	Line += 2;

	ppmNameLabel.SetDimensions( menuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT));
	ppmNameLabel.Display();

	ppmNameValueLabel.SetDimensions( ContentLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT));
	ppmNameInput.SetDimensions( menuLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT));

	ppmNameValueLabel.Display( ppmName);
	

	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White, LCD_65K_RGB::C_Black,
								 LCD::PO_Proportional, Text::Inverted);

	invertedCheckBox.SetDimensions( ContentLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
									ContentWidth, SCREEN_SETUP_BASE_GAUGE_THICKNESS);

	invertedCheckBox.Clear();
	invertedCheckBox.Display( ppmSetup.Inverted);

	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White, LCD_65K_RGB::C_Black,
								 LCD::PO_Proportional, Text::Center);

	centerLabel.SetDimensions( ContentLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT));
	centerLabel.SetMillisecond( ppmSetup.Center + 15);
	centerLabel.Display();

	Line++;

	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_WarmYellow, LCD_65K_RGB::C_Black,
								 LCD::PO_Proportional, Text::ChannelMapping);

	uint16_t SubMenuLeft = menuLeft + Font->GetCellWidth();

	setupScrollMarkers( Line);

	for( uint8_t ChannelLine = 0; ChannelLine < visibleLines; ChannelLine++)
	{
		uint16_t LineTop = frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT);

		targetChannelLabel[ ChannelLine].SetDimensions( SubMenuLeft, LineTop);
		sourceChannelLabel[ ChannelLine].SetDimensions( SourceLeft, LineTop);

		Line++;
	}

	reDisplay();
}

void Screen_Setup_System_PPM::reDisplay( void)
{
	// Update labels.
	for( uint8_t ChannelLine = 0; ChannelLine < visibleLines; ChannelLine++)
	{
		uint8_t ChannelIndex = firstLine + ChannelLine;

		targetChannelLabel[ ChannelLine].Clear();
		sourceChannelLabel[ ChannelLine].Clear();

		snprintf_P( targetChannelName[ ChannelLine], SETUP_CHANNEL_NAME_SIZE + 1,
				    Text::ChannelNameFormat, ChannelIndex + 1);
		sourceChannelLabel[ ChannelLine].SetValue( ppmSetup.ChannelMapping[ ChannelIndex] + 1);

		targetChannelLabel[ ChannelLine].Display();
		sourceChannelLabel[ ChannelLine].Display();
	}

	reDisplayMarkers( SIGNAL_PPM_CHANNELS);
}

void Screen_Setup_System_PPM::updateCenter( void)
{
	// Display with a center of 1.5ms.
	centerLabel.SetMillisecond( ppmSetup.Center + 15);

	GLOBAL.SignalService.GetPPM( ppmId)->SetCenter( ppmSetup.Center);
}

void Screen_Setup_System_PPM::updateCenter( void* Object, GUI_Setup_Label* Label, int8_t Value)
{
	(( Screen_Setup_System_PPM*) Object)->updateCenter();
}

void Screen_Setup_System_PPM::updateSourceChannel( GUI_Setup_Label* Label, int8_t Value)
{
	Label->SetValue( Value + 1);
	Label->Display();

	uint8_t ChannelIndex = ( currentMenuEntry - firstLine) - 4;

	GLOBAL.SignalService.GetPPM( ppmId)->SetChannelMapping( ChannelIndex, Value);
}

void Screen_Setup_System_PPM::updateSourceChannel( void* Object, GUI_Setup_Label* Label, int8_t Value)
{
	(( Screen_Setup_System_PPM*) Object)->updateSourceChannel( Label, Value);
}

bool Screen_Setup_System_PPM::processMenu( DoMenuResult Result)
{
	switch( Result)
	{
		case DMR_Selected :
		{
			switch( currentMenuEntry)
			{
				case 0 : return( false);

				case 1 : doPPMName();	break;
				case 2 : doInverted();	break;
				case 3 : doCenter();	break;

				default :
				{
					uint8_t ChannelIndex = ( currentMenuEntry - firstLine) - 4;
					int8_t* SourceChannel = ( int8_t*) &( ppmSetup.ChannelMapping[ ChannelIndex]);

					if( GUI_Setup_Select::DoSelect8( SourceChannel, 0, SIGNAL_PPM_CHANNELS - 1, 1, &menuMarker,
												    &( sourceChannelLabel[ ChannelIndex]),
												    this, NULL, &updateSourceChannel) == true)
					{
						GLOBAL.SetupService.SetPPM( ppmId, &ppmSetup);
					}
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

void Screen_Setup_System_PPM::displayMarker( void)
{
	uint8_t MarkerLine = currentMenuEntry;

	if( MarkerLine > M_Exit)
	{
		// Add one for the gap after exit.
		MarkerLine++;
	}

	if( MarkerLine > M_Center)
	{
		// Add another two for the gap after fixed entries.
		MarkerLine += 2;
	}

	menuMarker.Display( markerLeft, markerTop + ( MarkerLine * SCREEN_SETUP_BASE_LINE_HEIGHT));
}

void Screen_Setup_System_PPM::doPPMName( void)
{
	ppmNameValueLabel.Clear();

	ppmNameInput.ReadText( ppmName, sizeof( ppmName) - 1);

	strncpy( ppmName, ppmNameInput.GetText(), sizeof( ppmName));
	ppmName[ sizeof( ppmName) - 1] = 0;

	GLOBAL.SetupService.SetPPMName( ppmId, ppmName);

	ppmNameInput.Clear();

	ppmNameLabel.Display();
	ppmNameValueLabel.Display();
}

void Screen_Setup_System_PPM::doInverted( void)
{
	ppmSetup.Inverted = !ppmSetup.Inverted;

	GLOBAL.SetupService.SetPPM( ppmId, &ppmSetup);
	GLOBAL.SignalService.GetPPM( ppmId)->SetInverted( ppmSetup.Inverted);

	invertedCheckBox.Display( ppmSetup.Inverted);
}

void Screen_Setup_System_PPM::doCenter( void)
{
	if( GUI_Setup_Select::DoSelect8( &( ppmSetup.Center), SIGNAL_PPM_CENTER_MINIMUM,
									SIGNAL_PPM_CENTER_MAXIMUM, 1, &menuMarker, &centerLabel,
								    this, NULL, &updateCenter))
	{
		GLOBAL.SetupService.SetPPM( ppmId, &ppmSetup);
	}
}

void Screen_Setup_System_PPM::doChanged( void)
{
	if( Screen_Setup_BaseList::doChanged( SIGNAL_PPM_CHANNELS, 4))
	{
		reDisplay();
	}

	displayMarker();
}
