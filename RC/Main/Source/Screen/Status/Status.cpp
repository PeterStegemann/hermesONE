// Copyright 2009 Peter Stegemann

#include "Status.h"

#include "Signal/Source/Timer.h"
#include "Text/Text.h"

#include "AVR/Source/Utility.h"
#include "AVR/Source/Font/Font.h"

void Screen_Status_Status::displayGauge( GUI_Status_Gauge* Gauge, uint8_t SourceId)
{
    uint16_t SetupSourceId = GLOBAL.SetupService.GetStatusSourceId( SourceId);
    uint8_t Source = GLOBAL.SignalProcessor.GetSignalSourceId( SetupSourceId);

    Gauge->Display( SIGNAL_MINIMUM_VALUE, SIGNAL_MAXIMUM_VALUE, GLOBAL.SignalProcessor.GetSourceValue( Source));
}

void Screen_Status_Status::display( void)
{
	statusDisplay->Clear();

	// Set gauge dimensions.
	leftSideStatusGauge.SetDimensions( 0, 2);
	rightSideStatusGauge.SetDimensions( statusDisplay->GetWidth() - ( 5 + 1), 2);
	leftBottomStatusGauge.SetDimensions( 4, 7);
	rightBottomStatusGauge.SetDimensions(
		statusDisplay->GetWidth() - ( SCREEN_STATUS_STATUS_HORIZONTAL_GAUGE_WIDTH + 1 + 4), 7);

	// Clear all gauges.
	leftSideStatusGauge.Clear();
	rightSideStatusGauge.Clear();
	leftBottomStatusGauge.Clear();
	rightBottomStatusGauge.Clear();

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

	batteryLabel.SetDimensions( statusDisplay->GetWidth() - 25, 2);
	batteryLabel.SetFont( FONT::FI_Mini);
	batteryLabel.Clear();

	timeLabel.SetDimensions( 30, 5);
	timeLabel.SetFont( FONT::FI_Large);

	batteryGauge.Display( GLOBAL.StatusBattery.GetMinimumVoltage(), GLOBAL.StatusBattery.GetMaximumVoltage(), 0);

	update();
}

void Screen_Status_Status::update( void)
{
	// Update all gauges.
	displayGauge( &leftSideStatusGauge, SETUP_STATUS_SOURCE_LEFT_SIDE);
	displayGauge( &rightSideStatusGauge, SETUP_STATUS_SOURCE_RIGHT_SIDE);
	displayGauge( &leftBottomStatusGauge, SETUP_STATUS_SOURCE_LEFT_BOTTOM);
	displayGauge( &rightBottomStatusGauge, SETUP_STATUS_SOURCE_RIGHT_BOTTOM);

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
