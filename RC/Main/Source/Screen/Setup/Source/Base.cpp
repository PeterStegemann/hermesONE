// Copyright 2012 Peter Stegemann

#include "Base.h"

#include "GUI/Setup/Select.h"
#include "Main/Global.h"
#include "Signal/Processor.h"
#include "Text/Text.h"

#include "AVR/Source/Utility.h"

Screen_Setup_Source_Base::Screen_Setup_Source_Base( uint8_t SignalSourceId, uint32_t MenuPattern,
												    const flash_char* Title, bool HasGauge)
						: Screen_Setup_Base( MenuPattern, Title)
						, hasGauge( HasGauge)
						, signalSourceId( SignalSourceId)
{
	source = GLOBAL.SignalProcessor.GetSource( signalSourceId);

	setupSourceId = source->GetSetupSourceId();

	currentNameLabel.SetText_P( Text::Name);
	currentNameInput.SetOptions(( GUI_Setup_TextInput::Options)
							    ( GUI_Setup_TextInput::O_LimitAlphaNumeric));

	currentGauge.SetOptions(( GUI_Setup_Gauge::Options)( GUI_Setup_Gauge::O_Percentage |
														 GUI_Setup_Gauge::O_DualPercentage |
														 GUI_Setup_Gauge::O_CenterLine |
														 GUI_Setup_Gauge::O_Marker));
}

void Screen_Setup_Source_Base::display( void)
{
	// Adjust gauges to frame and set them up.
	const FONT_Type* Font = FONT::GetFont( SCREEN_SETUP_BASE_MAIN_FONT);
	
	uint16_t ValueLeft = menuLeft + 7 * Font->CellWidth;

	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop, SCREEN_SETUP_BASE_MAIN_FONT,
								 LCD_65K_RGB::C_WarmYellow, LCD_65K_RGB::C_Black,
								 LCD::PO_Proportional, Text::Exit);

	uint8_t Line = 2;

	currentNameLabel.SetDimensions( menuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT));
	currentNameLabel.Display();

	currentNameValueLabel.SetDimensions( ValueLeft,
										 frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT));
	currentNameInput.SetDimensions( ValueLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT));

	SetSourceLabel( &currentNameValueLabel, currentName, signalSourceId);

	if( hasGauge == true)
	{
		uint16_t CurrentGaugeLeft = markerLeft;
		uint16_t CurrentGaugeWidth = frameWidth - ( CurrentGaugeLeft - frameLeft) - 1;

		currentGauge.SetDimensions( CurrentGaugeLeft,
								    frameTop + frameHeight - SCREEN_SETUP_BASE_LINE_HEIGHT + 1,
								    CurrentGaugeWidth, SCREEN_SETUP_BASE_GAUGE_THICKNESS);
	}
}

void Screen_Setup_Source_Base::update( void)
{
	Screen_Setup_Base::update();

	if( hasGauge == true)
	{
		currentGauge.Display( SIGNAL_MINIMUM_VALUE, SIGNAL_MAXIMUM_VALUE,
							  GLOBAL.SignalProcessor.GetSourceValue( signalSourceId));
	}
}

bool Screen_Setup_Source_Base::processMenu( DoMenuResult Result)
{
	switch( Result)
	{
		case DMR_Selected :
		{
			switch( currentMenuEntry)
			{
				case 0 : return( false);

				case 2 : doCurrentName();	break;

				default : break;
			}
		}
		break;

		case DMR_Changed :	break;

		default : break;
	}

	return( true);
}

void Screen_Setup_Source_Base::doCurrentName( void)
{
	currentNameValueLabel.Clear();

	currentNameInput.ReadText( currentName, sizeof( currentName) - 1);

	strncpy( currentName, currentNameInput.GetText(), sizeof( currentName));
	currentName[ sizeof( currentName) - 1] = 0;

	GLOBAL.SetupService.SetSourceName( setupSourceId, currentName);

	currentNameInput.Clear();

	currentNameLabel.Display();
	currentNameValueLabel.Display();
}
