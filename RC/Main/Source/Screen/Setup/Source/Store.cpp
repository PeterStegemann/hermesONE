// Copyright 2009 Peter Stegemann

#include "Store.h"

#include "GUI/Setup/Select.h"
#include "Main/Global.h"
#include "Signal/Processor.h"
#include "Text/Text.h"

#include "AVR/Source/Utility.h"

Screen_Setup_Source_Store::Screen_Setup_Source_Store
(
    Input_Service* InputService,
    Interrupt_Service* InterruptService,
    Screen_Status_Status* StatusScreen,
    uint8_t SignalSourceId
)
    : Screen_Setup_Source_Base
    (
        InputService, InterruptService, StatusScreen, SignalSourceId, 0b0101101, Text::Store
    )
    , select( InputService, InterruptService)
    , sourceStore( NULL)
{
	sourceStore = &( source->Body.Store);
}

void Screen_Setup_Source_Store::display( void)
{
	Screen_Setup_Source_Base::display();

	// Adjust gauges to frame and set them up.
	const avr::font::Type* Font = avr::font::Font::Get( SCREEN_SETUP_BASE_MAIN_FONT);

	uint16_t ValueLeft = menuLeft + 7 * Font->GetCellWidth();

	uint8_t Line = 3;

	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Source);
	sourceNameLabel.SetDimensions( ValueLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT));

	SetSourceLabel( &sourceNameLabel, sourceName, sourceStore->SignalSourceId);

	Line++;

	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Reset);
}

bool Screen_Setup_Source_Store::processMenu( DoMenuResult Result)
{
	switch( Result)
	{
		case DMR_Selected :
		{
			bool ValueChanged = false;

			switch( currentMenuEntry)
			{
				case 3 :
				{
					ValueChanged = select.DoSourceSelect
					(
						&( sourceStore->SignalSourceId), &( sourceStore->Setup.InputSource),
						&menuMarker, &sourceNameLabel, NULL, sourceName, this, &staticUpdate,
						false, source->GetLevel()
                    );
				}
				break;

				case 5 :
				{
					sourceStore->ResetValue();
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
				GLOBAL.SetupService.SetSourceStore( setupSourceId, &( sourceStore->Setup));
			}
		}
		break;

		case DMR_Changed :	break;

		default : break;
	}

	return( true);
}
