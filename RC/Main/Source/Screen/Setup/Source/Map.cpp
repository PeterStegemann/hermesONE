// Copyright 2008 Peter Stegemann

#include "Map.h"

#include "GUI/Setup/Select.h"
#include "Main/Global.h"
#include "Signal/Processor.h"
#include "Signal/Utility.h"
#include "Text/Text.h"

#include "AVR/Source/Utility.h"

Screen_Setup_Source_Map::Screen_Setup_Source_Map( uint8_t SignalSourceId)
					   : Screen_Setup_Source_Base( SignalSourceId, 0b11111110010101, Text::Map)
					   , sourceMap( NULL)
{
	sourceMap = &( source->Body.Map);

	sourceVolumeLabel.SetText_P( Text::FourDigitPercentFormat);
	sourceVolumeLabel.SetOptions( GUI_Setup_Label::O_Fixed);

	for( uint8_t PointId = 0; PointId < SETUP_SOURCE_MAP_POINTS; PointId++)
	{
		pointVolumeLabel[ PointId].SetOptions( GUI_Setup_Label::O_Fixed);
	}
}

void Screen_Setup_Source_Map::update( void)
{
	Screen_Setup_Source_Base::update();

	int16_t InputValue = 0;

	if( sourceMap->InputSignalSourceId < SIGNAL_SOURCES)
	{
		InputValue = Signal_Utility::GetVolumizedSourceValue(
			&( GLOBAL.SignalProcessor), sourceMap->InputSignalSourceId,
			SIGNAL_SOURCE_MAP_INPUT_100_PERCENT_VALUE, sourceMap->Setup.InputSource.Volume);
	}

	mapGraph.Display( InputValue, sourceMap->Setup.PointSource, SETUP_SOURCE_MAP_POINTS);
}

void Screen_Setup_Source_Map::display( void)
{
	Screen_Setup_Source_Base::display();

	// Adjust gauges to frame and set them up.
	const FONT_Type* Font = FONT::GetFont( SCREEN_SETUP_BASE_MAIN_FONT);

	uint16_t ValueLeft = menuLeft + 7 * Font->getCellWidth();
	uint16_t VolumeLeft = ValueLeft + ( SETUP_SOURCE_NAME_SIZE + 1) * Font->getCellWidth();

	uint8_t Line = 4;
	// Source
	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White, 
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Source);
	sourceNameLabel.SetDimensions( ValueLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT));

	SetSourceLabel( &sourceNameLabel, sourceName, sourceMap->InputSignalSourceId);
/*
	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
						  SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White, LCD_65K_RGB::C_Black,
						  LCD::PO_Proportional, Text::Volume);
*/
	sourceVolumeLabel.SetDimensions( VolumeLeft,
									 frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT));
	sourceVolumeLabel.SetValue( sourceMap->Setup.InputSource.Volume /
							    SIGNAL_SOURCE_MAP_INPUT_SIGNAL_PER_VALUE);
	sourceVolumeLabel.Display();

	Line += 2;

	VolumeLeft = menuLeft + 21 * Font->getCellWidth();
	uint16_t ChartLeft = VolumeLeft + ( 9 * Font->getCellWidth()) - 4;
	uint16_t MapTop = frameTop + (( Line + 2) * SCREEN_SETUP_BASE_LINE_HEIGHT) - 5;

	mapGraph.SetDimensions( ChartLeft, MapTop, ( frameWidth - ChartLeft - 2));

	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_WarmYellow,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Source);
	GLOBAL.SetupDisplay.Print_P( VolumeLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_WarmYellow,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Volume);

	Line++;

	for( uint8_t PointId = 0; PointId < SETUP_SOURCE_MAP_POINTS; PointId++)
	{
		uint16_t LineTop = frameTop + (( Line + ( PointId * 1)) * SCREEN_SETUP_BASE_LINE_HEIGHT);

		pointSourceNameLabel[ PointId].SetDimensions( menuLeft, LineTop);
		SetSourceLabel( &( pointSourceNameLabel[ PointId]), pointSourceName[ PointId],
						sourceMap->PointSignalSourceId[ PointId]);

		pointVolumeLabel[ PointId].SetDimensions( VolumeLeft, LineTop);

		pointVolumeLabel[ PointId].SetText_P( Text::FourDigitPercentFormat);
		pointVolumeLabel[ PointId].SetValue( sourceMap->Setup.PointSource[ PointId].Volume /
											 SIGNAL_SOURCE_MAP_SIGNAL_PER_VALUE);

		pointVolumeLabel[ PointId].Display();
	}
}

bool Screen_Setup_Source_Map::processMenu( DoMenuResult Result)
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
					bool SourceChanged = GUI_Setup_Select::DoSourceSelect(
						&( sourceMap->InputSignalSourceId), &( sourceMap->Setup.InputSource.Source),
						&menuMarker, &sourceNameLabel, NULL, sourceName, this, &staticUpdate, false,
						source->GetLevel());

					bool VolumeChanged = GUI_Setup_Select::DoSelect16(
						&( sourceMap->Setup.InputSource.Volume), SIGNAL_MINIMUM_VALUE,
						SIGNAL_MAXIMUM_VALUE, SIGNAL_SOURCE_MAP_INPUT_SIGNAL_PER_VALUE, &menuMarker,
						&sourceVolumeLabel, this, &staticUpdate, &updateVolume);

					ValueChanged = SourceChanged || VolumeChanged;
				}
				break;

				default :
				{
					if( currentMenuEntry < 7)
					{
						return( Screen_Setup_Source_Base::processMenu( Result));
					}
					else
					{
						currentPointId = currentMenuEntry - 7;

						bool SourceChanged = GUI_Setup_Select::DoSourceSelect(
							&( sourceMap->PointSignalSourceId[ currentPointId]),
							&( sourceMap->Setup.PointSource[ currentPointId].Source), &menuMarker,
							&( pointSourceNameLabel[ currentPointId]), NULL,
							pointSourceName[ currentPointId], this, &staticUpdate, true,
							source->GetLevel());

						ValueChanged = GUI_Setup_Select::DoSelect16(
							&( sourceMap->Setup.PointSource[ currentPointId].Volume),
							SIGNAL_MINIMUM_VALUE, SIGNAL_MAXIMUM_VALUE,
							SIGNAL_SOURCE_MAP_SIGNAL_PER_VALUE, &menuMarker,
							&( pointVolumeLabel[ currentPointId]), this, &staticUpdate,
							&updatePointVolume);

						ValueChanged = SourceChanged || ValueChanged;
					}
				}
				break;
			}

			// Store new values.
			if( ValueChanged)
			{
				GLOBAL.SetupService.SetSourceMap( setupSourceId, &( sourceMap->Setup));
			}
		}
		break;

		default : break;
	}

	return( true);
}

void Screen_Setup_Source_Map::updateVolume( void* Object, GUI_Setup_Label* Label, int16_t Value)
{
	// Refresh label.
	Label->SetValue( Value / SIGNAL_SOURCE_MAP_INPUT_SIGNAL_PER_VALUE);
	Label->Display();
}

void Screen_Setup_Source_Map::updatePointVolume( void* Object, GUI_Setup_Label* Label,
												 int16_t Value)
{
	// Refresh label.		
	Label->SetText_P( Text::FourDigitPercentFormat);
	Label->SetValue( Value / SIGNAL_SOURCE_MAP_SIGNAL_PER_VALUE);

	Label->Display();
}
