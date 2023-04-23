// Copyright 2008 Peter Stegemann

#include "Calibration.h"

#include "Main/Setup.h"
#include "Text/Text.h"

#include "AVR/Source/Utility.h"

Screen_Setup_System_Calibration::Screen_Setup_System_Calibration( void)
							   : Screen_Setup_Base( 0b111111001, Text::Calibration)
{
	for( uint8_t Count = 0; Count < SIGNAL_PROCESSOR_ANALOG_INPUTS; Count++)
	{
		Signal_Calibration* Calibration = GLOBAL.SignalProcessor.GetCalibration( Count);

		high[ Count].SetText_P( Text::SixDigitInt16Format);
		high[ Count].SetValue( Calibration->Setup.Value[ Setup_Calibration::V_High]);

		center[ Count].SetText_P( Text::SixDigitInt16Format);
		center[ Count].SetValue( Calibration->Setup.Value[ Setup_Calibration::V_Center]);

		low[ Count].SetText_P( Text::SixDigitInt16Format);
		low[ Count].SetValue( Calibration->Setup.Value[ Setup_Calibration::V_Low]);
	}

	currentGauge.SetOptions(( GUI_Setup_Gauge::Options)( GUI_Setup_Gauge::O_Percentage |
														 GUI_Setup_Gauge::O_DualPercentage |
														 GUI_Setup_Gauge::O_CenterLine |
														 GUI_Setup_Gauge::O_Marker));
}

void Screen_Setup_System_Calibration::display( void)
{
	// Adjust gauges to frame and set them up.
	const FONT_Type* Font = FONT::GetFont( SCREEN_SETUP_BASE_MAIN_FONT);
	
	uint16_t LowLeft = menuLeft + 3 * Font->CellWidth;
	uint16_t ColumnWidth = ( frameWidth - ( LowLeft - frameLeft)) / 3;
	uint16_t CenterLeft = LowLeft + ColumnWidth;
	uint16_t HighLeft = CenterLeft + ColumnWidth;

	uint8_t Line = 0;

	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop, SCREEN_SETUP_BASE_MAIN_FONT,
							     LCD_65K_RGB::C_WarmYellow, LCD_65K_RGB::C_Black,
							     LCD::PO_Proportional, Text::Exit);

	Line += 2;

	GLOBAL.SetupDisplay.Print_P( LowLeft + Font->CellWidth * 1,
								 frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_WarmYellow,
								 LCD_65K_RGB::C_Black, LCD_65K_RGB::PO_Fixed, Text::Minus100Percent);
	GLOBAL.SetupDisplay.Print_P( CenterLeft + Font->CellWidth * 4,
								 frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_WarmYellow,
								 LCD_65K_RGB::C_Black, LCD_65K_RGB::PO_Fixed, Text::ZeroPercent);
	GLOBAL.SetupDisplay.Print_P( HighLeft + Font->CellWidth * 2,
								 frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_WarmYellow,
								 LCD_65K_RGB::C_Black, LCD_65K_RGB::PO_Fixed, Text::Plus100Percent);
	Line++;

	for( uint8_t Count = 0; Count < SIGNAL_PROCESSOR_ANALOG_INPUTS; Count++)
	{
		uint16_t LineTop = frameTop + (( Line + Count) * SCREEN_SETUP_BASE_LINE_HEIGHT);

		GLOBAL.SetupDisplay.PrintFormat_P( menuLeft, LineTop, SCREEN_SETUP_BASE_MAIN_FONT,
										   LCD_65K_RGB::C_WarmYellow, LCD_65K_RGB::C_Black,
										   LCD::PO_Proportional, Text::TwinCharacterFormat,
										   'A' + ( Count / 2), '0' + ( Count % 2));

		high[ Count].SetDimensions( HighLeft, LineTop);
		high[ Count].Display();

		center[ Count].SetDimensions( CenterLeft, LineTop);
		center[ Count].Display();

		low[ Count].SetDimensions( LowLeft, LineTop);
		low[ Count].Display();
	}

	currentLabel.SetDimensions( markerLeft,
							    frameTop + frameHeight - SCREEN_SETUP_BASE_LINE_HEIGHT + 1);
	currentLabel.SetText_P( Text::FourDigitInt16Format);

	uint16_t GaugeLeft = markerLeft + Font->CellWidth * 5;
	uint16_t GaugeWidth = frameWidth - ( GaugeLeft - frameLeft) - 1;

	currentGauge.SetDimensions( GaugeLeft,
							    frameTop + frameHeight - SCREEN_SETUP_BASE_LINE_HEIGHT + 1,
								GaugeWidth, SCREEN_SETUP_BASE_GAUGE_THICKNESS);

//	currentLabel.Clear();
//	currentGauge.Clear();
}

void Screen_Setup_System_Calibration::update( void)
{
	Screen_Setup_Base::update();
	
	if( currentMenuEntry >= 3)
	{
		currentGauge.Display( SIGNAL_MINIMUM_VALUE, SIGNAL_MAXIMUM_VALUE,
							  GLOBAL.SignalProcessor.GetCalibratedAnalogValue( currentMenuEntry - 3));
		currentLabel.SetValue( GLOBAL.SignalProcessor.GetRawAnalogValue( currentMenuEntry - 3));
		currentLabel.Display();
	}
}
	
bool Screen_Setup_System_Calibration::processMenu( DoMenuResult Result)
{
	switch( Result)
	{
		case DMR_Selected :
		{
			switch( currentMenuEntry)
			{
				case 0 :	return( false);

				default :	doCalibration( currentMenuEntry - 3);	break;
			}
		}
		break;

		case DMR_Changed :
		{
			menuMarker.Display( markerLeft,
								markerTop + ( currentMenuEntry * SCREEN_SETUP_BASE_LINE_HEIGHT));

			switch( currentMenuEntry)
			{
				case 0 :
				{
					// Clear gauge.
					currentLabel.Clear();
					currentGauge.Clear();
				}
				break;

				default :
				{
//					GLOBAL.SetupService.GetCalibration( currentMenuEntry - 3, &( calibration.setup));
				}
				break;
			}
		}
		break;

		default : break;
	}

	return( true);
}

void Screen_Setup_System_Calibration::doCalibration( uint8_t Index)
{
	Signal_Calibration* Calibration = GLOBAL.SignalProcessor.GetCalibration( Index);

	// Save some write cycles with this flag.
	bool CalibrationChanged = false;

	// Start in the low column.
	int8_t Column = 1;

	// Select.
	selectColumn( Index, Column, true);

	bool Exit = false;

	while( Exit == false)
	{
		currentGauge.Display( SIGNAL_MINIMUM_VALUE, SIGNAL_MAXIMUM_VALUE,
				      	      GLOBAL.SignalProcessor.GetCalibratedAnalogValue( Index));
		currentLabel.SetValue( GLOBAL.SignalProcessor.GetRawAnalogValue( Index));
		currentLabel.Display();

		int8_t RotarySelect;
		uint8_t RotaryButton;

		GLOBAL.InputService.GetRotary( &RotarySelect, &RotaryButton);

		if( RotaryButton > 0)
		{
			// Get new value.
			switch( Column)
			{
				case 0 :
				{
					Exit = true;
				}
				break;

				case 1 :
				{
					Calibration->Setup.Value[ Setup_Calibration::V_Low] =
						GLOBAL.SignalProcessor.GetRawAnalogValue( Index);
					CalibrationChanged = true;

					low[ Index].SetValue( Calibration->Setup.Value[ Setup_Calibration::V_Low]);
					low[ Index].Display();

					// Skip next.
					RotarySelect = 1;
				}
				break;
					
				case 2 :
				{
					Calibration->Setup.Value[ Setup_Calibration::V_Center] =
						GLOBAL.SignalProcessor.GetRawAnalogValue( Index);
					CalibrationChanged = true;

					center[ Index].SetValue( Calibration->Setup.Value[ Setup_Calibration::V_Center]);
					center[ Index].Display();
					
					// Skip next.
					RotarySelect = 1;
				}
				break;
					
				case 3 :
				{
					Calibration->Setup.Value[ Setup_Calibration::V_High] =
						GLOBAL.SignalProcessor.GetRawAnalogValue( Index);
					CalibrationChanged = true;

					high[ Index].SetValue( Calibration->Setup.Value[ Setup_Calibration::V_High]);
					high[ Index].Display();
					
					// Skip next.
					RotarySelect = 1;
				}
				break;
			}
		}

		// Set new position.
		if( RotarySelect != 0)
		{
			// Column 0 is the marker.
			if( Column == 0)
			{
				menuMarker.ForegroundColor = LCD_65K_RGB::C_WarmYellow;
				menuMarker.Display();
			}
			else
			{
				// Reset old column.
				selectColumn( Index, Column, false);
			}

			Column += RotarySelect;

			// Bring this back into the positive range.
			while( Column < 0) Column += 4;

			Column = Column % 4;

			// Column 0 is the marker.
			if( Column == 0)
			{
				menuMarker.ForegroundColor = LCD_65K_RGB::C_Red;
				menuMarker.Display();
			}
			else
			{
				// Set selected column.
				selectColumn( Index, Column, true);
			}
		}

		UTILITY::Pause( 5);
	}

	// Unselect.
	currentLabel.Clear();
	currentGauge.Clear();

	menuMarker.ForegroundColor = LCD_65K_RGB::C_WarmYellow;
	menuMarker.Display();

	// Store new values.
	if( CalibrationChanged)
	{
		GLOBAL.SetupService.SetCalibration( Index, &( Calibration->Setup));
	}
}

void Screen_Setup_System_Calibration::selectColumn( uint8_t Index, uint8_t Column, bool Selected)
{
	GUI_Setup_Label* Label = NULL;

	// Set selected column.
	switch( Column)
	{
		case 1 : Label = &( low[ Index]);		break;
		case 2 : Label = &( center[ Index]);	break;
		case 3 : Label = &( high[ Index]);		break;
	}

	if( Label != NULL)
	{
		if( Selected == true)
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
