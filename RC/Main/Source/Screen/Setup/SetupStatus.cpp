// Copyright 2009 Peter Stegemann

#include "SetupStatus.h"

#include "Main/Setup.h"
#include "GUI/Setup/Select.h"
#include "Text/Text.h"

#include "AVR/Source/Utility.h"

Screen_Setup_SetupStatus::Screen_Setup_SetupStatus( void)
						: Screen_Setup_Base( 0b1010101001101, Text::Status)
{
	currentSource = 0;

	for( uint8_t SourceLine = 0; SourceLine < SETUP_MODEL_STATUS_SOURCES; SourceLine++)
	{
		statusGauge[ SourceLine].SetOptions(
			( GUI_Setup_Gauge::Options)( GUI_Setup_Gauge::O_Percentage |
										 GUI_Setup_Gauge::O_DualPercentage |
										 GUI_Setup_Gauge::O_CenterLine |
										 GUI_Setup_Gauge::O_Marker));
	}
}

void Screen_Setup_SetupStatus::display( void)
{
	// Adjust menu entries to frame and set them up.
	const FONT_Type* Font = FONT::GetFont( SCREEN_SETUP_BASE_MAIN_FONT);

	uint16_t ValueLeft = menuLeft + 12 * Font->CellWidth;
	uint16_t SourceLeft = menuLeft + 1 * Font->CellWidth;
	uint16_t GaugeLeft = SourceLeft + ( SETUP_SOURCE_NAME_SIZE + 1) * Font->CellWidth;
	uint16_t GaugeWidth = frameWidth - ( GaugeLeft - frameLeft) - 1;	

	uint8_t Line = 0;

	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop, SCREEN_SETUP_BASE_MAIN_FONT,
								 LCD_65K_RGB::C_WarmYellow, LCD_65K_RGB::C_Black,
								 LCD::PO_Proportional, Text::Exit);

	Line += 2;

	const flash_char* TimerName[] =
	{
		Text::TimerTop, Text::TimerBottom
	};

	uint16_t LineTop = frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT);

	for( uint8_t TimerLine = 0; TimerLine < SETUP_MODEL_STATUS_TIMERS; TimerLine++)
	{
		GLOBAL.SetupDisplay.Print_P( menuLeft, LineTop, SCREEN_SETUP_BASE_MAIN_FONT,
									 LCD_65K_RGB::C_White, LCD_65K_RGB::C_Black,
									 LCD::PO_Proportional, TimerName[ TimerLine]);

		timerLabel[ TimerLine].SetDimensions( ValueLeft, LineTop);

		LineTop += SCREEN_SETUP_BASE_LINE_HEIGHT;
	}

	Line += SETUP_MODEL_STATUS_TIMERS + 1;

	const flash_char* SourceName[] =
	{
		Text::LeftSide, Text::LeftBottom, Text::RightSide, Text::RightBottom
	};

	LineTop = frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT);

	for( uint8_t SourceLine = 0; SourceLine < SETUP_MODEL_STATUS_SOURCES; SourceLine++)
	{
		GLOBAL.SetupDisplay.Print_P( menuLeft, LineTop, SCREEN_SETUP_BASE_MAIN_FONT,
									 LCD_65K_RGB::C_WarmYellow, LCD_65K_RGB::C_Black,
									 LCD::PO_Proportional, SourceName[ SourceLine]);

		LineTop += SCREEN_SETUP_BASE_LINE_HEIGHT;

		sourceLabel[ SourceLine].SetDimensions( SourceLeft, LineTop);

		statusGauge[ SourceLine].SetDimensions( GaugeLeft, LineTop, GaugeWidth,
											    SCREEN_SETUP_BASE_GAUGE_THICKNESS);

		LineTop += SCREEN_SETUP_BASE_LINE_HEIGHT;
	}

	reDisplay();
}

void Screen_Setup_SetupStatus::reDisplay( void)
{
	// Find timers to display.
	for( uint8_t TimerLine = 0; TimerLine < SETUP_MODEL_STATUS_TIMERS; TimerLine++)
	{
		uint16_t SetupTimerId = GLOBAL.SetupService.GetStatusTimerId( TimerLine);

		timerId[ TimerLine] = GLOBAL.SignalProcessor.GetSignalSourceId( SetupTimerId);

		SetSourceLabel( &( timerLabel[ TimerLine]), timerName[ TimerLine], timerId[ TimerLine]);
	}

	// Clear all gauges.
	for( uint8_t SourceLine = 0; SourceLine < SETUP_MODEL_STATUS_SOURCES; SourceLine++)
	{
		statusGauge[ SourceLine].Clear();
	}

	// Find sources to display.
	for( uint8_t SourceLine = 0; SourceLine < SETUP_MODEL_STATUS_SOURCES; SourceLine++)
	{
		uint16_t SetupSourceId = GLOBAL.SetupService.GetStatusSourceId( SourceLine);

		sourceId[ SourceLine] = GLOBAL.SignalProcessor.GetSignalSourceId( SetupSourceId);

		SetSourceLabel( &( sourceLabel[ SourceLine]), sourceName[ SourceLine],
					    sourceId[ SourceLine]);
	}
}

void Screen_Setup_SetupStatus::update( void)
{
//	GLOBAL.StatusScreen.SetSource( currentSource, sourceId[ currentSource]);

	Screen_Setup_Base::update();

	// Print all channels.
	for( uint8_t SourceLine = 0; SourceLine < SETUP_MODEL_STATUS_SOURCES; SourceLine++)
	{
		statusGauge[ SourceLine].Display( SIGNAL_MINIMUM_VALUE, SIGNAL_MAXIMUM_VALUE,
										  GLOBAL.SignalProcessor.GetSourceValue(
											sourceId[ SourceLine]));
	}
}

bool Screen_Setup_SetupStatus::processMenu( DoMenuResult Result)
{
	switch( Result)
	{
		case DMR_Selected :
		{
			switch( currentMenuEntry)
			{
				case 0 : return( false);

				case 2 :
				case 3 :
				{
					uint8_t CurrentTimer = currentMenuEntry - 2;

					uint16_t SetupSourceId = GLOBAL.SetupService.GetStatusTimerId( CurrentTimer);

					bool SourceChanged = GUI_Setup_Select::DoSourceSelect(
						&( timerId[ CurrentTimer]), &SetupSourceId, &menuMarker,
						&( timerLabel[ CurrentTimer]), NULL,
						timerName[ CurrentTimer], this, &staticUpdate, false,
						Signal_Source_Source::L_Model, Signal_Source_Source::T_Timer);

					if( SourceChanged == true)
					{
						GLOBAL.SetupService.SetStatusTimerId( CurrentTimer, SetupSourceId);
					}
				}
				break;

				default :
				{
					currentSource = ( currentMenuEntry - 6) / 2;

					uint16_t SetupSourceId = GLOBAL.SetupService.GetStatusSourceId( currentSource);

					bool SourceChanged = GUI_Setup_Select::DoSourceSelect(
						&( sourceId[ currentSource]), &SetupSourceId, &menuMarker,
						&( sourceLabel[ currentSource]), &( statusGauge[ currentSource]),
						sourceName[ currentSource], this, &staticUpdate, false,
						Signal_Source_Source::L_Model);

					if( SourceChanged == true)
					{
						GLOBAL.SetupService.SetStatusSourceId( currentSource, SetupSourceId);
					}
				}
				break;
			}
		}
		break;

		default : break;
	}

	return( true);
}
