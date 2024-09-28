// Copyright 2008 Peter Stegemann

#include "Channel.h"

#include "Main/Global.h"
#include "GUI/Setup/Select.h"
#include "Signal/Processor.h"
#include "Signal/Utility.h"
#include "Text/Text.h"

#include "AVR/Source/Utility.h"

Screen_Setup_Channel::Screen_Setup_Channel
(
    Input_Service* InputService,
    Interrupt_Service* InterruptService,
    Screen_Status_Status* StatusScreen,
    uint8_t ChannelId
)
    : Screen_Setup_Base( InputService, StatusScreen, 0b11011101110101, Text::Channel)
    , select( InputService, InterruptService)
    , channelId( ChannelId)
    , channelNameInput( InputService, InterruptService)
{
	channel = GLOBAL.SignalProcessor.GetChannel( channelId);

	GLOBAL.SetupService.GetChannelName( channelId, channelName, SETUP_CHANNEL_NAME_SIZE + 1);

	channelNameLabel.SetText_P( Text::Name);
	channelNameInput.SetOptions(( GUI_Setup_TextInput::Options)	( GUI_Setup_TextInput::O_LimitAlphaNumeric));

	sourceVolumeLabel.SetText_P( Text::FourDigitPercentFormat);
	sourceVolumeLabel.SetOptions( GUI_Setup_Label::O_Fixed);

	trimVolumeLabel.SetText_P( Text::FourDigitPercentFormat);
	trimVolumeLabel.SetOptions( GUI_Setup_Label::O_Fixed);

	limitVolumeLabel.SetText_P( Text::FourDigitPercentFormat);
	limitVolumeLabel.SetOptions( GUI_Setup_Label::O_Fixed);

	for( uint8_t PointId = 0; PointId < Setup_Channel::PV_PointVolumeCount; PointId++)
	{
		pointVolumeLabel[ PointId].SetOptions( GUI_Setup_Label::O_Fixed);
		pointVolumeLabel[ PointId].SetText_P( Text::FourDigitPercentFormat);
	}

	currentGauge.SetOptions(( GUI_Setup_Gauge::Options)( GUI_Setup_Gauge::O_Percentage |
														 GUI_Setup_Gauge::O_DualPercentage |
														 GUI_Setup_Gauge::O_CenterLine |
													     GUI_Setup_Gauge::O_Marker));
}

void Screen_Setup_Channel::display( void)
{	
	// Adjust gauges to frame and set them up.
	const avr::font::Type* Font = avr::font::Font::Get( SCREEN_SETUP_BASE_MAIN_FONT);

	uint16_t ContentLeft = frameLeft + 12 * Font->GetCellWidth();
	uint16_t ContentWidth = frameWidth - ( ContentLeft - frameLeft) - 1;

	uint16_t ValueLeft = menuLeft + 7 * Font->GetCellWidth();
	uint16_t VolumeLeft = ValueLeft + ( SETUP_SOURCE_NAME_SIZE * Font->GetCellWidth());

	uint8_t Line = 0;

	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop,
	                             SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_WarmYellow, LCD_65K_RGB::C_Black,
								 LCD::PO_Proportional, Text::Exit);

	Line += 2;

	channelNameLabel.SetDimensions( menuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT));
	channelNameLabel.Display();

	channelNameValueLabel.SetDimensions( ValueLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT));
	channelNameInput.SetDimensions( menuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT));

	channelNameValueLabel.Display( channelName);

	Line += 2;

	// Source
	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White, LCD_65K_RGB::C_Black,
								 LCD::PO_Proportional, Text::Source);
	sourceNameLabel.SetDimensions( ValueLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT));

	SetSourceLabel( &sourceNameLabel, sourceName, channel->InputSignalSourceId);

	sourceVolumeLabel.SetDimensions( VolumeLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT));
	sourceVolumeLabel.SetValue( channel->Setup.InputSource.Volume / SIGNAL_CHANNEL_INPUT_SIGNAL_PER_VALUE);
	sourceVolumeLabel.Display();

	Line++;

	// Trim
	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White, LCD_65K_RGB::C_Black,
								 LCD::PO_Proportional, Text::Trim);
	trimNameLabel.SetDimensions( ValueLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT));

	SetSourceLabel( &trimNameLabel, trimName, channel->TrimSignalSourceId);

	trimVolumeLabel.SetDimensions( VolumeLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT));
	trimVolumeLabel.SetValue( channel->Setup.TrimSource.Volume /
							  SIGNAL_CHANNEL_INPUT_SIGNAL_PER_VALUE);
	trimVolumeLabel.Display();

	Line++;

	// Limit
	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Limit);
	limitNameLabel.SetDimensions( ValueLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT));

	SetSourceLabel( &limitNameLabel, limitName, channel->LimitSignalSourceId);

	limitVolumeLabel.SetDimensions( VolumeLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT));
	limitVolumeLabel.SetValue( channel->Setup.LimitSource.Volume / SIGNAL_CHANNEL_INPUT_SIGNAL_PER_VALUE);
	limitVolumeLabel.Display();

	Line += 2;

	// Graph
	VolumeLeft = menuLeft + ( 16 * Font->GetCellWidth());

	uint16_t ChartLeft = VolumeLeft + 11 * Font->GetCellWidth();
	uint16_t ChartTop = frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT);

	channelGraph.SetDimensions( ChartLeft, ChartTop, UTILITY_Minimum( frameHeight - ChartTop, frameWidth - ChartLeft));

	for( uint8_t PointId = 0; PointId < Setup_Channel::PV_PointVolumeCount; PointId++)
	{
		uint16_t LineTop = frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT);

		const flash_char* LabelText = NULL;

		switch( PointId)
		{
			case 0 : LabelText = Text::Bottom;		break;
			case 1 : LabelText = Text::Center;		break;
			case 2 : LabelText = Text::Top;			break;
		}

		GLOBAL.SetupDisplay.PrintFormat_P( menuLeft, LineTop,
		                                   SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_WarmYellow, LCD_65K_RGB::C_Black,
										   LCD::PO_Proportional, LabelText);

		pointVolumeLabel[ PointId].SetDimensions( VolumeLeft, LineTop);
		pointVolumeLabel[ PointId].SetValue( channel->Setup.PointVolume[ PointId] / SIGNAL_CHANNEL_SIGNAL_PER_VALUE);
		pointVolumeLabel[ PointId].Display();

		Line++;
	}

	Line++;

	// Reverse
	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White, LCD_65K_RGB::C_Black,
								 LCD::PO_Proportional, Text::Reverse);
	
	reverseCheckBox.SetDimensions( ContentLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
								   ContentWidth, SCREEN_SETUP_BASE_GAUGE_THICKNESS);
	reverseCheckBox.Clear();
	reverseCheckBox.Display( channel->Setup.Reverse);

	// Mode
	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White, LCD_65K_RGB::C_Black,
								 LCD::PO_Proportional, Text::Mode);

	modeLabel.SetDimensions( ContentLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT));
	updateMode( &modeLabel, channel->Setup.Mode);

	// Result
	uint16_t CurrentGaugeLeft = markerLeft;
	uint16_t CurrentGaugeWidth = ChartLeft - markerLeft - Font->GetCellWidth();

	currentGauge.SetDimensions( CurrentGaugeLeft, frameTop + frameHeight - SCREEN_SETUP_BASE_LINE_HEIGHT + 1,
							    CurrentGaugeWidth, SCREEN_SETUP_BASE_GAUGE_THICKNESS);	
}

void Screen_Setup_Channel::update( void)
{
	Screen_Setup_Base::update();

	int16_t SourceValue = Signal_Utility::GetVolumizedSourceValue
	(
		&( GLOBAL.SignalProcessor), channel->InputSignalSourceId,
		SIGNAL_CHANNEL_INPUT_100_PERCENT_VALUE, channel->Setup.InputSource.Volume
    );

	int16_t TrimValue = Signal_Utility::GetVolumizedSourceValue
	(
		&( GLOBAL.SignalProcessor), channel->TrimSignalSourceId,
		SIGNAL_CHANNEL_INPUT_0_PERCENT_VALUE, channel->Setup.TrimSource.Volume
    );

	int16_t LimitValue = Signal_Utility::GetVolumizedSourceValue
	(
		&( GLOBAL.SignalProcessor), channel->LimitSignalSourceId,
		SIGNAL_CHANNEL_INPUT_100_PERCENT_VALUE, channel->Setup.LimitSource.Volume
    );

	points[ Setup_Channel::PV_Low] = Signal_Utility::VolumizeValue(
		channel->Setup.PointVolume[ Setup_Channel::PV_Low], SIGNAL_CHANNEL_100_PERCENT_VALUE,
		LimitValue);
	points[ Setup_Channel::PV_Center] = Signal_Utility::AddValues(
		channel->Setup.PointVolume[ Setup_Channel::PV_Center], TrimValue);
	points[ Setup_Channel::PV_High] = Signal_Utility::VolumizeValue(
		channel->Setup.PointVolume[ Setup_Channel::PV_High], SIGNAL_CHANNEL_100_PERCENT_VALUE,
		LimitValue);

	channelGraph.Display( SourceValue, points, Setup_Channel::PV_PointVolumeCount);

	currentGauge.Display( SIGNAL_MINIMUM_VALUE, SIGNAL_MAXIMUM_VALUE,
						  GLOBAL.SignalProcessor.GetChannelValue( channelId));
}

bool Screen_Setup_Channel::processMenu( DoMenuResult Result)
{
	switch( Result)
	{
		case DMR_Selected :
		{
			bool ValueChanged = false;

			switch( currentMenuEntry)
			{
				case 0 : return( false);

				case 2 : doChannelName(); break;

				case 4 :
				{
					ValueChanged =
						doSource( &( channel->InputSignalSourceId), &( channel->Setup.InputSource),
								  sourceName, &sourceNameLabel, &sourceVolumeLabel);
				}
				break;

				case 5 :
				{
					ValueChanged =
						doSource( &( channel->TrimSignalSourceId), &( channel->Setup.TrimSource),
								  trimName, &trimNameLabel, &trimVolumeLabel);
				}
				break;

				case 6 :
				{
					ValueChanged =
						doSource( &( channel->LimitSignalSourceId), &( channel->Setup.LimitSource),
								  limitName, &limitNameLabel, &limitVolumeLabel);
				}
				break;

				case 8 :
				case 9 :
				case 10 :
				{
					currentPointId = currentMenuEntry - 8;

					ValueChanged = select.DoSelect16
					(
						&( channel->Setup.PointVolume[ currentPointId]), SIGNAL_MINIMUM_VALUE,
						SIGNAL_MAXIMUM_VALUE, SIGNAL_CHANNEL_SIGNAL_PER_VALUE, &menuMarker,
						&( pointVolumeLabel[ currentPointId]), this, &staticUpdate,
						&updatePointVolume
                    );
				}
				break;

				case 12 :
				{
					channel->Setup.Reverse = avr::Utility::Invert( channel->Setup.Reverse);

					ValueChanged = true;

					reverseCheckBox.Display( channel->Setup.Reverse);
				}
				break;

				case 13 :
				{
					ValueChanged = select.DoSelect8
					(
						( int8_t*) &( channel->Setup.Mode), Setup_Channel::M_Warp,
						Setup_Channel::M_Clip, 1, &menuMarker, &modeLabel, this, &staticUpdate,
						&updateMode
					);
				}
				break;
			}

			// Store new values.
			if( ValueChanged == true)
			{
				GLOBAL.SetupService.SetChannel( channelId, &( channel->Setup));
			}
		}
		break;

		default : break;
	}

	return( true);
}

void Screen_Setup_Channel::doChannelName( void)
{
	channelNameValueLabel.Clear();

	channelNameInput.ReadText( channelName, sizeof( channelName) - 1);

	strncpy( channelName, channelNameInput.GetText(), sizeof( channelName));
	channelName[ sizeof( channelName) - 1] = 0;

	GLOBAL.SetupService.SetChannelName( channelId, channelName);

	channelNameInput.Clear();

	channelNameLabel.Display();
	channelNameValueLabel.Display();
}

bool Screen_Setup_Channel::doSource
(
    uint8_t* SignalSourceId,
    Setup_Source_Tuple* SourceTuple,
    char SourceName[ SETUP_SOURCE_NAME_SIZE + 1],
    GUI_Setup_Label* SourceNameLabel,
    GUI_Setup_Label* SourceVolumeLabel
)
{
	// Do source.
	bool SourceChanged = select.DoSourceSelect
	(
		SignalSourceId, &( SourceTuple->Source), &menuMarker, SourceNameLabel, NULL, SourceName,
		this, &staticUpdate, true, Signal_Source_Source::L_Model
    );

	// Do volume.
	currentVolumeLabel = SourceVolumeLabel;

	bool VolumeChanged = select.DoSelect16
	(
	    &( SourceTuple->Volume),
		SIGNAL_MINIMUM_VALUE, SIGNAL_MAXIMUM_VALUE, SIGNAL_CHANNEL_INPUT_SIGNAL_PER_VALUE,
		&menuMarker, SourceVolumeLabel, this, &staticUpdate, &updateVolume
    );

	return( SourceChanged || VolumeChanged);
}

void Screen_Setup_Channel::updateVolume( void* Object, GUI_Setup_Label* Label, int16_t Value)
{
	// Refresh label.
	Label->SetValue( Value / SIGNAL_CHANNEL_INPUT_SIGNAL_PER_VALUE);
	Label->Display();
}

void Screen_Setup_Channel::updatePointVolume( void* Object, GUI_Setup_Label* Label, int16_t Value)
{
	(( Screen_Setup_Channel*) Object)->updatePointVolume((( Screen_Setup_Channel*) Object)->currentPointId, Value);
}

void Screen_Setup_Channel::updatePointVolume( uint8_t CurrentPointId, int16_t Value)
{
	GUI_Setup_Label* UseLabel = &( pointVolumeLabel[ CurrentPointId]);

	// Refresh label.
	UseLabel->SetValue( Value / SIGNAL_CHANNEL_SIGNAL_PER_VALUE);
	UseLabel->Display();
}

void Screen_Setup_Channel::updateMode( void* Object, GUI_Setup_Label* Label, int8_t Value)
{
	(( Screen_Setup_Channel*) Object)->updateMode( Label, Value);
}

void Screen_Setup_Channel::updateMode( GUI_Setup_Label* Label, int8_t Value)
{
	Label->Clear();

	switch( Value)
	{
		case Setup_Channel::M_Warp :
		{
			Label->SetText_P( Text::Warp);

			channelGraph.SetMode( GUI_Setup_Graph::COWM_Warp);
		}
		break;

		case Setup_Channel::M_Clip :
		{
			Label->SetText_P( Text::Clip);

			channelGraph.SetMode( GUI_Setup_Graph::COWM_Clip);
		}
		break;
	}

	Label->Display();
}
