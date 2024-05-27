// Copyright 2013 Peter Stegemann

#include "Proxy.h"

#include "Main/Global.h"
#include "GUI/Setup/Select.h"
#include "Signal/Processor.h"
#include "Signal/Utility.h"
#include "Text/Text.h"

#include "AVR/Source/Utility.h"

Screen_Setup_Source_Proxy::Screen_Setup_Source_Proxy( uint8_t SignalSourceId)
						 : Screen_Setup_Source_Base( SignalSourceId, 0b10101, Text::Proxy)
						 , sourceProxy( NULL)
{
	sourceProxy = &( source->Body.Proxy);

	sourceVolumeLabel.SetText_P( Text::FourDigitPercentFormat);
	sourceVolumeLabel.SetOptions( GUI_Setup_Label::O_Fixed);
}

void Screen_Setup_Source_Proxy::display( void)
{
	Screen_Setup_Source_Base::display();

	// Adjust gauges to frame and set them up.
	const FONT_Type* Font = FONT::GetFont( SCREEN_SETUP_BASE_MAIN_FONT);

	uint16_t ValueLeft = menuLeft + 7 * Font->getCellWidth();
	uint16_t VolumeLeft = ValueLeft + ( SETUP_SOURCE_NAME_SIZE + 1) * Font->getCellWidth();

	uint8_t Line = 4;

	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Source);
	sourceNameLabel.SetDimensions( ValueLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT));

	SetSourceLabel( &sourceNameLabel, sourceName, sourceProxy->SignalSourceId);

	sourceVolumeLabel.SetDimensions( VolumeLeft,
									 frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT));
	sourceVolumeLabel.SetValue( sourceProxy->ReferenceSetup.Volume /
							    SIGNAL_SOURCE_PROXY_SIGNAL_PER_VALUE);
	sourceVolumeLabel.Display();
}

bool Screen_Setup_Source_Proxy::processMenu( DoMenuResult Result)
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
						&( sourceProxy->SignalSourceId), &( sourceProxy->ReferenceSetup.Source),
						&menuMarker, &sourceNameLabel, NULL, sourceName, this, &staticUpdate, true,
						Signal_Source_Source::L_Model);

					ValueChanged = GUI_Setup_Select::DoSelect16(
						&( sourceProxy->ReferenceSetup.Volume), SIGNAL_MINIMUM_VALUE,
						SIGNAL_MAXIMUM_VALUE, SIGNAL_SOURCE_PROXY_SIGNAL_PER_VALUE, &menuMarker,
						&sourceVolumeLabel, this, &staticUpdate, &updateVolume);

					ValueChanged = SourceChanged || ValueChanged;
				}
				break;

				default :
				{
					return( Screen_Setup_Source_Base::processMenu( Result));
				}
				break;
			}

			// Store new values.
			if( ValueChanged == true)
			{
				GLOBAL.SetupService.SetProxyReference( sourceProxy->Setup.Slot,
													   &( sourceProxy->ReferenceSetup));
			}
		}
		break;

		default : break;
	}

	return( true);
}

void Screen_Setup_Source_Proxy::updateVolume( void* Object, GUI_Setup_Label* Label, int16_t Value)
{
	// Refresh label.
	Label->SetValue( Value / SIGNAL_SOURCE_PROXY_SIGNAL_PER_VALUE);
	Label->Display();
}
