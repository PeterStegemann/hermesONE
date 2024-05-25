// Copyright 2007 Peter Stegemann

#include "Status.h"

#include "Main/Setup.h"
#include "Setup/Defines.h"
#include "Setup/Service.h"
#include "Setup/Utility.h"
#include "Signal/Service.h"
#include "Text/Text.h"

#include "AVR/Source/Utility.h"

Screen_Setup_Status::Screen_Setup_Status( void)
				   : Screen_Setup_Base( 0b1, Text::Status, false)
{
	statusGauge[ SETUP_STATUS_SOURCE_LEFT_SIDE].SetOptions
	(
		( GUI_Setup_Gauge::Options)( GUI_Setup_Gauge::O_Vertical |
									 GUI_Setup_Gauge::O_CenterLine |
									 GUI_Setup_Gauge::O_Reverse |
									 GUI_Setup_Gauge::O_Marker |
									 GUI_Setup_Gauge::O_MarkerLookRight)
    );

	statusGauge[ SETUP_STATUS_SOURCE_RIGHT_SIDE].SetOptions
	(
		( GUI_Setup_Gauge::Options)( GUI_Setup_Gauge::O_Vertical |
									 GUI_Setup_Gauge::O_CenterLine |
									 GUI_Setup_Gauge::O_Reverse |
									 GUI_Setup_Gauge::O_Marker |
									 GUI_Setup_Gauge::O_MarkerLookLeft)
    );

	statusGauge[ SETUP_STATUS_SOURCE_LEFT_BOTTOM].SetOptions
	(
		( GUI_Setup_Gauge::Options)( GUI_Setup_Gauge::O_CenterLine |
									 GUI_Setup_Gauge::O_Marker |
									 GUI_Setup_Gauge::O_MarkerLookUp)
    );

	statusGauge[ SETUP_STATUS_SOURCE_RIGHT_BOTTOM].SetOptions
	(
		( GUI_Setup_Gauge::Options)( GUI_Setup_Gauge::O_CenterLine |
									 GUI_Setup_Gauge::O_Marker |
									 GUI_Setup_Gauge::O_MarkerLookUp)
    );

	// Find sources to display.
	for( uint8_t SourceId = 0; SourceId < SETUP_MODEL_STATUS_SOURCES; SourceId++)
	{
		uint16_t SetupSourceId = GLOBAL.SetupService.GetStatusSourceId( SourceId);

		statusSource[ SourceId] = GLOBAL.SignalProcessor.GetSignalSourceId( SetupSourceId);
	}

	// Find timers to display.
	for( uint8_t TimerId = 0; TimerId < SETUP_MODEL_STATUS_TIMERS; TimerId++)
	{
		uint16_t SetupSourceId = GLOBAL.SetupService.GetStatusTimerId( TimerId);

		timerSource[ TimerId] = GLOBAL.SignalProcessor.GetSignalSourceId( SetupSourceId);
	}
}

void Screen_Setup_Status::display( void)
{
	// Adjust gauges to frame and set them up.
	uint16_t LeftGaugeLeft = frameLeft + 1;
	uint16_t RightGaugeLeft = ( frameLeft + frameWidth + 5) - SCREEN_SETUP_BASE_GAUGE_THICKNESS;
	uint16_t BottomGaugeSize = ( frameWidth / 2) - 2;
	uint16_t BottomGaugeTop = frameTop + frameHeight - SCREEN_SETUP_BASE_LINE_HEIGHT + 1;
	uint16_t SideGaugeTop = BottomGaugeTop - BottomGaugeSize - 2;

	uint16_t InnerLeft = LeftGaugeLeft + SCREEN_SETUP_BASE_GAUGE_THICKNESS + 1;
	uint16_t InnerRight = RightGaugeLeft - 1;
	uint16_t InnerWidth = InnerRight - InnerLeft - 2;

	statusGauge[ SETUP_STATUS_SOURCE_LEFT_SIDE].SetDimensions
    (
        LeftGaugeLeft, SideGaugeTop, SCREEN_SETUP_BASE_GAUGE_THICKNESS, BottomGaugeSize
    );
	statusGauge[ SETUP_STATUS_SOURCE_RIGHT_SIDE].SetDimensions
	(
        RightGaugeLeft, SideGaugeTop, SCREEN_SETUP_BASE_GAUGE_THICKNESS, BottomGaugeSize
    );
	statusGauge[ SETUP_STATUS_SOURCE_LEFT_BOTTOM].SetDimensions
	(
        LeftGaugeLeft, BottomGaugeTop, BottomGaugeSize, SCREEN_SETUP_BASE_GAUGE_THICKNESS
    );
	statusGauge[ SETUP_STATUS_SOURCE_RIGHT_BOTTOM].SetDimensions
	(
        frameLeft + BottomGaugeSize + 3, BottomGaugeTop, BottomGaugeSize, SCREEN_SETUP_BASE_GAUGE_THICKNESS
    );

	uint8_t Line = 3;

	const FONT_Type* Font = FONT::GetFont( FONT::FI_Large);
	uint8_t TextLeft;

	char ModelName[ SETUP_MODEL_NAME_SIZE + 1];
	GLOBAL.SetupService.GetModelName( GLOBAL.SetupService.GetSelectedModelId(), ModelName, sizeof( ModelName));

	TextLeft = frameLeft + (( frameWidth - strlen( ModelName) * Font->CellWidth) / 2);
	GLOBAL.SetupDisplay.Print
	(
        TextLeft, frameTop + ( Line++ * Font->CellHeight), FONT::FI_Large,
        LCD_65K_RGB::C_White, LCD_65K_RGB::C_Black, LCD::PO_Proportional, ModelName
    );

	Line++;

	LCD_65K_RGB::Color RFModeColor;

	uint8_t RFMode = GLOBAL.SetupService.GetRFMode( GLOBAL.SetupService.GetSelectedModelId());

	switch( RFMode)
	{
		default :
		case Setup_Service::RF_None :
		{
			RFModeColor = LCD_65K_RGB::C_White;
		}
		break;

		case Setup_Service::RF_Both :
		{
			RFModeColor = LCD_65K_RGB::C_Yellow;
		}
		break;

		case Setup_Service::RF_Module_0 :
		{
			RFModeColor = LCD_65K_RGB::C_Red;
		}
		break;

		case Setup_Service::RF_Module_1 :
		{
			RFModeColor = LCD_65K_RGB::C_Green;
		}
		break;
	}

	char RFModeText[ SETUP_PPM_NAME_SIZE];

	Setup_Utility::GetRFModeName( RFMode, RFModeText, sizeof( RFModeText));

	TextLeft = frameLeft + (( frameWidth - strlen( RFModeText) * Font->CellWidth) / 2);
	GLOBAL.SetupDisplay.Print
	(
	    TextLeft, frameTop + ( Line++ * Font->CellHeight), FONT::FI_Large,
        RFModeColor, LCD_65K_RGB::C_Black, LCD::PO_Proportional, RFModeText
    );

	Line++;

	batteryLabel.SetDimensions( InnerLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT));
	batteryLabel.SetFont( FONT::FI_Large);
	batteryGauge.SetDimensions
	(
	    InnerLeft + 42, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT) + 1,
        InnerWidth - 42, SCREEN_SETUP_BASE_GAUGE_THICKNESS
    );

	Line++;

	TextLeft = frameLeft + (( frameWidth - ( 0 * Font->CellWidth)) / 2);
	uint16_t NameLeft = InnerLeft + ( 1 * Font->CellWidth);

	bool SystemTimeShown = false;

	for( uint8_t TimerId = 0; TimerId < SETUP_MODEL_STATUS_TIMERS; TimerId++)
	{
		uint16_t SetupSourceId = GLOBAL.SetupService.GetStatusTimerId( TimerId);

		uint16_t TextTop = frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT);

		if( SetupSourceId == SETUP_SOURCE_NONE)
		{
			if( SystemTimeShown == false)
			{
				GLOBAL.SetupDisplay.Print_P
				(
				    NameLeft, TextTop, FONT::FI_Large, LCD_65K_RGB::C_White, LCD_65K_RGB::C_Black,
                    LCD::PO_Proportional, Text::SystemTime
                );

				SystemTimeShown = true;
			}
		}
		else
		{
			char TimerName[ SETUP_SOURCE_NAME_SIZE + 1];

			GLOBAL.SetupService.GetSourceName( SetupSourceId, TimerName, SETUP_SOURCE_NAME_SIZE + 1);

			GLOBAL.SetupDisplay.Print
			(
			    NameLeft, TextTop, FONT::FI_Large, LCD_65K_RGB::C_White, LCD_65K_RGB::C_Black,
                LCD::PO_Proportional, TimerName
            );
		}

		timerLabel[ TimerId].SetDimensions( TextLeft, TextTop);
		timerLabel[ TimerId].SetFont( FONT::FI_Large);
	}

	GLOBAL.SetupDisplay.PrintFormat_P
	(
        frameLeft + 45, BottomGaugeTop - Font->CellHeight, FONT::FI_Large,
        LCD_65K_RGB::C_White, LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::StatusUsageFormat,
        GLOBAL.SetupService.CountModels( Setup_Service::CMO_Used), SETUP_MODELS,
        GLOBAL.SetupService.CountSources( Setup_Service::CSO_Used), SETUP_SOURCES
    );
}

void Screen_Setup_Status::update( void)
{
	Screen_Setup_Base::update();

	// Print all channels.
	for( uint8_t SourceLine = 0; SourceLine < SETUP_MODEL_STATUS_SOURCES; SourceLine++)
	{
		statusGauge[ SourceLine].Display
		(
		    SIGNAL_MINIMUM_VALUE, SIGNAL_MAXIMUM_VALUE,
            GLOBAL.SignalProcessor.GetSourceValue( statusSource[ SourceLine])
        );
	}

	// Battery
	batteryLabel.SetVoltage( GLOBAL.StatusBattery.GetVoltage());

	batteryGauge.Display
	(
        GLOBAL.StatusBattery.GetMinimumVoltage(),
        GLOBAL.StatusBattery.GetMaximumVoltage(),
        GLOBAL.StatusBattery.GetVoltage()
    );

	// Timer
	bool SystemTimeShown = false;

	for( uint8_t TimerId = 0; TimerId < SETUP_MODEL_STATUS_TIMERS; TimerId++)
	{
		uint8_t SignalSourceId = timerSource[ TimerId];

		if( SignalSourceId == SIGNAL_SOURCE_NONE)
		{
			if( SystemTimeShown == false)
			{
				timerLabel[ TimerId].SetTime( GLOBAL.StatusTime.GetUptime());

				SystemTimeShown = true;
			}
		}
		else
		{
			Signal_Source_Source* Source = GLOBAL.SignalProcessor.GetSource( SignalSourceId);

			if(( Source != NULL) && ( Source->GetType() == Signal_Source_Source::T_Timer))
			{
				timerLabel[ TimerId].SetTime( Source->Body.Timer.GetTime());
			}
		}
	}
}

bool Screen_Setup_Status::processMenu( DoMenuResult Result)
{
	switch( Result)
	{
		case DMR_Selected :	return( false);

		default : break;
	}

	return( true);
}
