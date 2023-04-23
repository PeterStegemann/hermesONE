// Copyright 2012 Peter Stegemann

#include "Trimmer.h"

#include "Main/Setup.h"
#include "GUI/Setup/Select.h"
#include "Signal/Processor.h"
#include "Signal/Utility.h"
#include "Text/Text.h"

#include "AVR/Source/Utility.h"

Screen_Setup_Source_Trimmer::Screen_Setup_Source_Trimmer( uint8_t SignalSourceId)
						   : Screen_Setup_Source_Base( SignalSourceId, 0b101010101110101,
													   Text::Trimmer)
						   , sourceTrimmer( NULL)
{
	sourceTrimmer = &( source->Body.Trimmer);

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
}

void Screen_Setup_Source_Trimmer::display( void)
{
	Screen_Setup_Source_Base::display();

	// Adjust gauges to frame and set them up.
	const FONT_Type* Font = FONT::GetFont( SCREEN_SETUP_BASE_MAIN_FONT);

	uint16_t ContentLeft = frameLeft + 12 * Font->CellWidth;
	uint16_t ContentWidth = frameWidth - ( ContentLeft - frameLeft) - 1;

	uint16_t ValueLeft = menuLeft + 7 * Font->CellWidth;
	uint16_t VolumeLeft = ValueLeft + ( SETUP_SOURCE_NAME_SIZE * Font->CellWidth);

	uint8_t Line = 4;

	// Source
	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Source);
	sourceNameLabel.SetDimensions( ValueLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT));

	SetSourceLabel( &sourceNameLabel, sourceName, sourceTrimmer->InputSignalSourceId);

	sourceVolumeLabel.SetDimensions( VolumeLeft,
									 frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT));
	sourceVolumeLabel.SetValue( sourceTrimmer->Setup.InputSource.Volume /
							    SIGNAL_CHANNEL_INPUT_SIGNAL_PER_VALUE);
	sourceVolumeLabel.Display();

	Line++;

	// Trim
	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Trim);
	trimNameLabel.SetDimensions( ValueLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT));

	SetSourceLabel( &trimNameLabel, trimName, sourceTrimmer->TrimSignalSourceId);

	trimVolumeLabel.SetDimensions( VolumeLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT));
	trimVolumeLabel.SetValue( sourceTrimmer->Setup.TrimSource.Volume /
							  SIGNAL_CHANNEL_INPUT_SIGNAL_PER_VALUE);
	trimVolumeLabel.Display();

	Line++;

	// Limit
	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
							 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White, LCD_65K_RGB::C_Black,
							 LCD::PO_Proportional, Text::Limit);
	limitNameLabel.SetDimensions( ValueLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT));

	SetSourceLabel( &limitNameLabel, limitName, sourceTrimmer->LimitSignalSourceId);

	limitVolumeLabel.SetDimensions( VolumeLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT));
	limitVolumeLabel.SetValue( sourceTrimmer->Setup.LimitSource.Volume /
							   SIGNAL_CHANNEL_INPUT_SIGNAL_PER_VALUE);
	limitVolumeLabel.Display();

	Line += 2;

	// Graph
	VolumeLeft = menuLeft + ( 16 * Font->CellWidth);
	uint16_t ChartLeft = VolumeLeft + 13 * Font->CellWidth;

	uint16_t ChartTop = frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT);

	trimmerGraph.SetDimensions( ChartLeft, ChartTop,
							    UTILITY_Minimum( 7 * SCREEN_SETUP_BASE_LINE_HEIGHT,
											/* frameHeight - ( ChartTop + SCREEN_SETUP_BASE_GAUGE_THICKNESS),*/
											    ( frameWidth - ChartLeft)));

//	channelGraph.Display( sourceTrimmer->Setup.Point, SETUP_CHANNEL_POINTS);

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

		GLOBAL.SetupDisplay.PrintFormat_P( menuLeft, LineTop, SCREEN_SETUP_BASE_MAIN_FONT,
										   LCD_65K_RGB::C_WarmYellow, LCD_65K_RGB::C_Black,
										   LCD::PO_Proportional, LabelText);

		pointVolumeLabel[ PointId].SetDimensions( VolumeLeft, LineTop);
		pointVolumeLabel[ PointId].SetValue( sourceTrimmer->Setup.PointVolume[ PointId] /
											 SIGNAL_CHANNEL_SIGNAL_PER_VALUE);
		pointVolumeLabel[ PointId].Display();

		Line += 2;
	}

	// Reverse
	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Reverse);
	
	reverseCheckBox.SetDimensions( ContentLeft,
								   frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
								   ContentWidth, SCREEN_SETUP_BASE_GAUGE_THICKNESS);
	reverseCheckBox.Clear();
	reverseCheckBox.Display( sourceTrimmer->Setup.Reverse);
}

void Screen_Setup_Source_Trimmer::update( void)
{
	Screen_Setup_Source_Base::update();

	int16_t SourceValue = Signal_Utility::GetVolumizedSourceValue(
		&( GLOBAL.SignalProcessor), sourceTrimmer->InputSignalSourceId,
		SIGNAL_CHANNEL_INPUT_100_PERCENT_VALUE, sourceTrimmer->Setup.InputSource.Volume);

	int16_t TrimValue = Signal_Utility::GetVolumizedSourceValue(
		&( GLOBAL.SignalProcessor), sourceTrimmer->TrimSignalSourceId,
		SIGNAL_CHANNEL_INPUT_0_PERCENT_VALUE, sourceTrimmer->Setup.TrimSource.Volume);

	int16_t LimitValue = Signal_Utility::GetVolumizedSourceValue(
		&( GLOBAL.SignalProcessor), sourceTrimmer->LimitSignalSourceId,
		SIGNAL_CHANNEL_INPUT_100_PERCENT_VALUE, sourceTrimmer->Setup.LimitSource.Volume);

	point[ Setup_Channel::PV_Low] = Signal_Utility::VolumizeValue(
		sourceTrimmer->Setup.PointVolume[ Setup_Channel::PV_Low], SIGNAL_CHANNEL_100_PERCENT_VALUE,
		LimitValue);
	point[ Setup_Channel::PV_Center] = Signal_Utility::AddValues(
		sourceTrimmer->Setup.PointVolume[ Setup_Channel::PV_Center], TrimValue);
	point[ Setup_Channel::PV_High] = Signal_Utility::VolumizeValue(
		sourceTrimmer->Setup.PointVolume[ Setup_Channel::PV_High], SIGNAL_CHANNEL_100_PERCENT_VALUE,
		LimitValue);

	trimmerGraph.Display( SourceValue, point, Setup_Channel::PV_PointVolumeCount);
}

bool Screen_Setup_Source_Trimmer::processMenu( DoMenuResult Result)
{
	switch( Result)
	{
		case DMR_Selected :
		{
			bool ValueChanged = false;

			switch( currentMenuEntry)
			{
				case 4 :
				{
					ValueChanged =
						doSource( &( sourceTrimmer->InputSignalSourceId),
								  &( sourceTrimmer->Setup.InputSource),
								  sourceName, &sourceNameLabel, &sourceVolumeLabel);
				}
				break;

				case 5 :
				{
					ValueChanged =
						doSource( &( sourceTrimmer->TrimSignalSourceId),
								  &( sourceTrimmer->Setup.TrimSource),
								  trimName, &trimNameLabel, &trimVolumeLabel);
				}
				break;

				case 6 :
				{
					ValueChanged =
						doSource( &( sourceTrimmer->LimitSignalSourceId),
								  &( sourceTrimmer->Setup.LimitSource),
								  limitName, &limitNameLabel, &limitVolumeLabel);
				}
				break;

				case 8 :
				case 10 :
				case 12 :
				{
					currentPointId = ( currentMenuEntry - 8) / 2;

					ValueChanged = GUI_Setup_Select::DoSelect16(
						&( sourceTrimmer->Setup.PointVolume[ currentPointId]), SIGNAL_MINIMUM_VALUE,
						SIGNAL_MAXIMUM_VALUE, SIGNAL_CHANNEL_SIGNAL_PER_VALUE, &menuMarker,
						&( pointVolumeLabel[ currentPointId]), this, &staticUpdate,
						&updatePointVolume);
				}
				break;

				case 14 :
				{
					sourceTrimmer->Setup.Reverse = UTILITY::Invert( sourceTrimmer->Setup.Reverse);

					ValueChanged = true;

					reverseCheckBox.Display( sourceTrimmer->Setup.Reverse);
				}
				break;

				default :
				{
					return( Screen_Setup_Source_Base::processMenu( Result));
				}
				break;
			}

			// Store new values.
			if( ValueChanged)
			{
				GLOBAL.SetupService.SetSourceTrimmer( setupSourceId, &( sourceTrimmer->Setup));
			}
		}
		break;

		default : break;
	}

	return( true);
}

bool Screen_Setup_Source_Trimmer::doSource( uint8_t* SignalSourceId,
										    Setup_Source_Tuple* SourceTuple,
										    char SourceName[ SETUP_SOURCE_NAME_SIZE + 1],
										    GUI_Setup_Label* SourceNameLabel,
										    GUI_Setup_Label* SourceVolumeLabel)
{
	// Do source.
	bool SourceChanged = GUI_Setup_Select::DoSourceSelect(
		SignalSourceId, &( SourceTuple->Source), &menuMarker, SourceNameLabel, NULL, SourceName,
		this, &staticUpdate, true, Signal_Source_Source::L_Model);

	// Do volume.
	currentVolumeLabel = SourceVolumeLabel;

	bool VolumeChanged = GUI_Setup_Select::DoSelect16( &( SourceTuple->Volume),
		SIGNAL_MINIMUM_VALUE, SIGNAL_MAXIMUM_VALUE, SIGNAL_CHANNEL_INPUT_SIGNAL_PER_VALUE,
		&menuMarker, SourceVolumeLabel, this, &staticUpdate, &updateVolume);

	return( SourceChanged || VolumeChanged);
}

void Screen_Setup_Source_Trimmer::updateVolume( void* Object, GUI_Setup_Label* Label, int16_t Value)
{
	// Refresh label.
	Label->SetValue( Value / SIGNAL_SOURCE_TRIMMER_INPUT_SIGNAL_PER_VALUE);
	Label->Display();
}

void Screen_Setup_Source_Trimmer::updatePointVolume( void* Object, GUI_Setup_Label* Label,
													 int16_t Value)
{
	(( Screen_Setup_Source_Trimmer*) Object)->updatePointVolume(
	    (( Screen_Setup_Source_Trimmer*) Object)->currentPointId, Value);
}

void Screen_Setup_Source_Trimmer::updatePointVolume( uint8_t CurrentPointId, int16_t Value)
{
	GUI_Setup_Label* UseLabel = &( pointVolumeLabel[ CurrentPointId]);

	// Refresh label.
	UseLabel->SetValue( Value / SIGNAL_SOURCE_TRIMMER_SIGNAL_PER_VALUE);
	UseLabel->Display();
}
