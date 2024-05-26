// Copyright 2008 Peter Stegemann

#include "Mix.h"

#include "GUI/Setup/Select.h"
#include "Main/Global.h"
#include "Signal/Processor.h"
#include "Text/Text.h"

#include "AVR/Source/Utility.h"

Screen_Setup_Source_Mix::Screen_Setup_Source_Mix( uint8_t SignalSourceId)
					   : Screen_Setup_Source_Base( SignalSourceId, 0b111111100101, Text::Mix)
					   , sourceMix( NULL)
{
	sourceMix = &( source->Body.Mix);

	for( uint8_t Index = 0; Index < SETUP_SOURCE_MIX_INPUTS; Index++)
	{
		volumeLabel[ Index].SetText_P( Text::FourDigitPercentFormat);

		valueGauge[ Index].SetOptions(( GUI_Setup_Gauge::Options)( GUI_Setup_Gauge::O_CenterLine |
																   GUI_Setup_Gauge::O_Marker));
	}
}

void Screen_Setup_Source_Mix::display( void)
{
	Screen_Setup_Source_Base::display();

	// Adjust gauges to frame and set them up.
	const FONT_Type* Font = FONT::GetFont( SCREEN_SETUP_BASE_MAIN_FONT);

	uint16_t SourceLeft = menuLeft + 2 * Font->CellWidth;
	uint16_t VolumeLeft = SourceLeft + ( SETUP_SOURCE_NAME_SIZE + 1) * Font->CellWidth;
	uint16_t GaugeLeft = VolumeLeft + 6 * Font->CellWidth;
	uint16_t GaugeWidth = frameWidth - ( GaugeLeft - frameLeft) - 1;

	uint8_t Line = 4;

	GLOBAL.SetupDisplay.Print_P( SourceLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_WarmYellow,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Source);
	GLOBAL.SetupDisplay.Print_P( VolumeLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_WarmYellow,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Volume);

	Line++;

	for( uint8_t SourceLine = 0; SourceLine < SETUP_SOURCE_MIX_INPUTS; SourceLine++)
	{
		uint16_t LineTop = frameTop + (( Line + SourceLine) * SCREEN_SETUP_BASE_LINE_HEIGHT);

		GLOBAL.SetupDisplay.PrintFormat_P( menuLeft, LineTop, SCREEN_SETUP_BASE_MAIN_FONT,
										   LCD_65K_RGB::C_WarmYellow, LCD_65K_RGB::C_Black,
										   LCD::PO_Proportional, Text::CharacterFormat,
										   '1' + SourceLine);

		sourceLabel[ SourceLine].SetDimensions( SourceLeft, LineTop);
		SetSourceLabel( &( sourceLabel[ SourceLine]), sourceName[ SourceLine],
						sourceMix->SignalSourceId[ SourceLine]);

		volumeLabel[ SourceLine].SetDimensions( VolumeLeft, LineTop);
		volumeLabel[ SourceLine].SetOptions( GUI_Setup_Label::O_Fixed);
		volumeLabel[ SourceLine].SetValue( sourceMix->Setup.InputSource[ SourceLine].Volume /
										   SIGNAL_SOURCE_MIX_SIGNAL_PER_VALUE);
		volumeLabel[ SourceLine].Display();

		valueGauge[ SourceLine].SetDimensions( GaugeLeft, LineTop, GaugeWidth,
											   SCREEN_SETUP_BASE_GAUGE_THICKNESS);
	}
}

void Screen_Setup_Source_Mix::update( void)
{
    Screen_Setup_Source_Base::update();

	// Print all channels.
	for( uint8_t SourceLine = 0; SourceLine < SETUP_SOURCE_MIX_INPUTS; SourceLine++)
	{
		if( sourceMix->SignalSourceId[ SourceLine] < SIGNAL_SOURCES)
		{
			valueGauge[ SourceLine].Display( SIGNAL_MINIMUM_VALUE, SIGNAL_MAXIMUM_VALUE,
											 GLOBAL.SignalProcessor.GetSourceValue(
												sourceMix->SignalSourceId[ SourceLine]));
		}
	}
}

bool Screen_Setup_Source_Mix::processMenu( DoMenuResult Result)
{
	switch( Result)
	{
		case DMR_Selected :
		{
			if( currentMenuEntry < 5)
			{
				return( Screen_Setup_Source_Base::processMenu( Result));
			}
			else
			{
				doSource( currentMenuEntry - 5);
			}
		}
		break;

		case DMR_Changed :	break;

		default : break;
	}

	return( true);
}

void Screen_Setup_Source_Mix::doSource( uint8_t Index)
{
	// Do source.
	bool SourceChanged = GUI_Setup_Select::DoSourceSelect(
		&( sourceMix->SignalSourceId[ Index]), &( sourceMix->Setup.InputSource[ Index].Source),
		&menuMarker, &( sourceLabel[ Index]), &( valueGauge[ Index]), sourceName[ Index], this,
		&staticUpdate, true, source->GetLevel());

	// Do volume.
	currentVolumeIndex = Index;

	bool VolumeChanged = GUI_Setup_Select::DoSelect16(
		&( sourceMix->Setup.InputSource[ currentVolumeIndex].Volume), SIGNAL_MINIMUM_VALUE,
		SIGNAL_MAXIMUM_VALUE, SIGNAL_SOURCE_MIX_SIGNAL_PER_VALUE, &menuMarker,
		&( volumeLabel[ currentVolumeIndex]), this, &staticUpdate, &updateVolume);

	// Store new values.
	if( SourceChanged || VolumeChanged)
	{
		GLOBAL.SetupService.SetSourceMix( setupSourceId, &( sourceMix->Setup));
	}
}

void Screen_Setup_Source_Mix::selectColumn( uint8_t Index, uint8_t Column, bool Selected)
{
	GUI_Setup_Label* Label = NULL;

	// Set selected column.
	switch( Column)
	{
		case 1 : Label = &( sourceLabel[ Index]);	break;
		case 2 : Label = &( volumeLabel[ Index]);	break;
	}

	if( Label != NULL)
	{
		if( Selected)
		{
			Label->ForegroundColor = LCD_65K_RGB::C_Red;
		}
		else
		{
			Label->ForegroundColor = LCD_65K_RGB::C_White;
		}

		Label->Display();
	}
}

void Screen_Setup_Source_Mix::updateVolume( void* Object, GUI_Setup_Label* Label, int16_t Value)
{
	// Refresh label.
	Label->SetValue( Value / SIGNAL_SOURCE_MIX_SIGNAL_PER_VALUE);
	Label->Display();
}
