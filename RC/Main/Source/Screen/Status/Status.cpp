// Copyright 2009 Peter Stegemann

#include "Status.h"

#include "Signal/Source/Timer.h"
#include "Text/Text.h"

#include "AVR/Source/Utility.h"
#include "AVR/Source/Font/Font.h"
#include "AVR/Source/LCD/LCD_DOG.h"

Screen_Status_Status::Screen_Status_Status( void)
		            : Screen_Base()
{
	statusGauge[ SETUP_STATUS_SOURCE_LEFT_SIDE].SetOptions(
		( GUI_Status_Gauge::Options)( GUI_Status_Gauge::O_Vertical));
	statusGauge[ SETUP_STATUS_SOURCE_RIGHT_SIDE].SetOptions(
		( GUI_Status_Gauge::Options)( GUI_Status_Gauge::O_Vertical));
	statusGauge[ SETUP_STATUS_SOURCE_LEFT_BOTTOM].SetOptions(
		( GUI_Status_Gauge::Options)( GUI_Status_Gauge::O_Horizontal));
	statusGauge[ SETUP_STATUS_SOURCE_RIGHT_BOTTOM].SetOptions(
		( GUI_Status_Gauge::Options)( GUI_Status_Gauge::O_Horizontal));

	// Find sources to display.
	uint8_t SourceLine = 0;

	while( SourceLine < SETUP_MODEL_STATUS_SOURCES)
	{
		uint16_t SetupSourceId = GLOBAL.SetupService.GetStatusSourceId( SourceLine);

		source[ SourceLine] = GLOBAL.SignalProcessor.GetSignalSourceId( SetupSourceId);

		SourceLine++;
	}
}

void Screen_Status_Status::display( void)
{
	GLOBAL.StatusDisplay.Clear();

	statusGauge[ SETUP_STATUS_SOURCE_LEFT_SIDE].SetDimensions( 0, 2);
	statusGauge[ SETUP_STATUS_SOURCE_RIGHT_SIDE].SetDimensions(
		GLOBAL.StatusDisplay.GetWidth() - ( 5 + 1), 2);
	statusGauge[ SETUP_STATUS_SOURCE_LEFT_BOTTOM].SetDimensions( 4, 7);
	statusGauge[ SETUP_STATUS_SOURCE_RIGHT_BOTTOM].SetDimensions(
		GLOBAL.StatusDisplay.GetWidth() - ( SCREEN_STATUS_STATUS_HORIZONTAL_GAUGE_WIDTH + 1 + 4), 7);

	// Clear all channels.
	for( uint8_t SourceLine = 0; SourceLine < SETUP_MODEL_STATUS_SOURCES; SourceLine++)
	{
		statusGauge[ SourceLine].Clear();
	}

	uint8_t SelectedModelId = GLOBAL.SetupService.GetSelectedModelId();
	GLOBAL.SetupService.GetModelName( SelectedModelId, modelName, sizeof( modelName));

	modelNameLabel.SetDimensions( 0, 0);
	modelNameLabel.SetFont( FONT::FI_Large);
	modelNameLabel.SetText( modelName);
	modelNameLabel.Display();

	uint8_t SelectedTypeId = GLOBAL.SetupService.GetSelectedTypeId( SelectedModelId);
	GLOBAL.SetupService.GetTypeName( SelectedTypeId, typeName, sizeof( typeName));

	typeNameLabel.SetDimensions( 7, 2);
	typeNameLabel.SetFont( FONT::FI_Mini);
	typeNameLabel.SetText( typeName);
	typeNameLabel.Display();

	batteryLabel.SetDimensions( GLOBAL.StatusDisplay.GetWidth() - 25, 2);
	batteryLabel.SetFont( FONT::FI_Mini);
	batteryLabel.Clear();

	timeLabel.SetDimensions( 30, 5);
	timeLabel.SetFont( FONT::FI_Large);

	batteryGauge.Display( GLOBAL.StatusBattery.GetMinimumVoltage(),
						  GLOBAL.StatusBattery.GetMaximumVoltage(), 0);

	update();
}

void Screen_Status_Status::update( void)
{
	// Find sources to display.
	uint8_t SourceLine = 0;

	while( SourceLine < SETUP_MODEL_STATUS_SOURCES)
	{
		uint16_t SetupSourceId =
			GLOBAL.SetupService.GetStatusSourceId( SourceLine);

		source[ SourceLine] = GLOBAL.SignalProcessor.GetSignalSourceId( SetupSourceId);

		SourceLine++;
	}

	// Print all channels.
	for( uint8_t SourceLine = 0; SourceLine < SETUP_MODEL_STATUS_SOURCES; SourceLine++)
	{
		statusGauge[ SourceLine].Display( SIGNAL_MINIMUM_VALUE, SIGNAL_MAXIMUM_VALUE,
										  GLOBAL.SignalProcessor.GetSourceValue(
												source[ SourceLine]));
	}

	uint8_t Voltage = GLOBAL.StatusBattery.GetVoltage();

	batteryGauge.Display( Voltage);
	batteryLabel.SetVoltage( Voltage);

	uint16_t SetupSourceId = GLOBAL.SetupService.GetStatusTimerId( SETUP_STATUS_TIMER_TOP);

	if( SetupSourceId == SETUP_SOURCE_NONE)
	{
		timeLabel.SetTime( GLOBAL.StatusTime.GetUptime());
	}
	else
	{
		uint8_t TimeId = GLOBAL.SignalProcessor.GetSignalSourceId( SetupSourceId);

		Signal_Source_Source* Source = GLOBAL.SignalProcessor.GetSource( TimeId);

		if( Source->GetType() == Signal_Source_Source::T_Timer)
		{
			timeLabel.SetTime( Source->Body.Timer.GetTime());
		}
	}
}

bool Screen_Status_Status::processMenu( DoMenuResult Result)
{
	switch( Result)
	{
		case DMR_Selected :	return( false);

		default : break;
	}

	return( true);
}

void Screen_Status_Status::ShowIntro( void)
{
	GLOBAL.StatusDisplay.Clear();

	GLOBAL.StatusDisplay.Print( 15, 1, FONT::FI_Large, "hermesONE");
}

void Screen_Status_Status::ShowRfOff( void)
{
	GLOBAL.StatusDisplay.Clear();

	GLOBAL.StatusDisplay.Print_P( 5, 1, FONT::FI_Large, Text::RFDisabled);
}

void Screen_Status_Status::Display( void)
{
	display();
}

void Screen_Status_Status::Update( void)
{
	update();
}
