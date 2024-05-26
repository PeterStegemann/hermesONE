// Copyright 2008 Peter Stegemann

#include "Input.h"

#include "GUI/Setup/Select.h"
#include "Main/Global.h"
#include "Signal/Processor.h"
#include "Text/Text.h"

#include "AVR/Source/Utility.h"

static uint16_t getMenuPattern( uint8_t SignalSourceId)
{
	switch( SignalSourceId)
	{
		case Setup_Source_Input::IT_Analog :	return( 0b00000000101101);
		case Setup_Source_Input::IT_Button :	return( 0b01101110101101);
		case Setup_Source_Input::IT_Switch :	return( 0b00001101101101);
		case Setup_Source_Input::IT_Ticker :	return( 0b011011101101101);
		case Setup_Source_Input::IT_Rotary :	return( 0b011011101101101);

		default :	return( 0b1101);
	}

	return( 0);
}

static void setAnalogInput( char Value[ 2 + 1], uint8_t InputId)
{
	Value[ 0] = ( InputId / 2) + 'A';
	Value[ 1] = ( InputId % 2) + '0';
	Value[ 2] = 0;
}

static void setDigitalInput( char Value[ 2 + 1], uint8_t InputId)
{
	Value[ 0] = ( InputId / 4) + 'H';
	Value[ 1] = ( InputId % 4) + '0';
	Value[ 2] = 0;
}

Screen_Setup_Source_Input::Screen_Setup_Source_Input( uint8_t SignalSourceId)
						 : Screen_Setup_Source_Base( SignalSourceId, 0, Text::Input)
						 , sourceInput( NULL)
{
	sourceInput = &( source->Body.Input.Setup);

	previousType = sourceInput->Type;

	SetMenuPattern( getMenuPattern( sourceInput->Type));

	inputAValueLabel.SetText( inputAValue);
	inputBValueLabel.SetText( inputBValue);

	initValueLabel.SetText_P( Text::FourDigitPercentFormat);
	initValueLabel.SetOptions( GUI_Setup_Label::O_Fixed);
	stepValueLabel.SetText_P( Text::FourDigitPercentFormat);
	stepValueLabel.SetOptions( GUI_Setup_Label::O_Fixed);

	topValueLabel.SetText_P( Text::FourDigitPercentFormat);
	topValueLabel.SetOptions( GUI_Setup_Label::O_Fixed);
	bottomValueLabel.SetText_P( Text::FourDigitPercentFormat);
	bottomValueLabel.SetOptions( GUI_Setup_Label::O_Fixed);
}

void Screen_Setup_Source_Input::display( void)
{
	Screen_Setup_Source_Base::display();

	draw();
}

bool Screen_Setup_Source_Input::processMenu( DoMenuResult Result)
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
					ValueChanged = GUI_Setup_Select::DoSelect(
						( int8_t*) &( sourceInput->Type), 0,
						Setup_Source_Input::IT_InputTypeCount - 1, 1,
						&menuMarker, &typeValueLabel, this, &staticUpdate, &updateType);
				}
				break;

				case 5 :
				{
					switch( sourceInput->Type)
					{
						case Setup_Source_Input::IT_Analog :
						{
							ValueChanged = GUI_Setup_Select::DoSelect(
								( int8_t*) &( sourceInput->InputIdA), 0,
								SIGNAL_PROCESSOR_ANALOG_INPUTS - 1, 1, &menuMarker,
								&inputAValueLabel, this, &staticUpdate, &updateInputAnalog);
						}
						break;

						default :
						{							
							ValueChanged = GUI_Setup_Select::DoSelect(
								( int8_t*) &( sourceInput->InputIdA), 0,
								SIGNAL_PROCESSOR_DIGITAL_INPUTS - 1, 1, &menuMarker,
								&inputAValueLabel, this, &staticUpdate, &updateInputDigitalA);
						}
						break;
					}
				}
				break;

				case 6 :
				{
					ValueChanged = GUI_Setup_Select::DoSelect(
						( int8_t*) &( sourceInput->InputIdB), 0,
						SIGNAL_PROCESSOR_DIGITAL_INPUTS - 1, 1, &menuMarker, &inputBValueLabel,
						this, &staticUpdate, &updateInputDigitalB);
				}
				break;

				case 7 :
				{
					switch( sourceInput->Type)
					{
						case Setup_Source_Input::IT_Analog :
						{
							ValueChanged = GUI_Setup_Select::DoSelect16(
								&( sourceInput->TopVolume), SIGNAL_MINIMUM_VALUE,
								SIGNAL_MAXIMUM_VALUE, SIGNAL_SOURCE_INPUT_VOLUME_SIGNAL_PER_VALUE,
								&menuMarker, &topValueLabel, this, &staticUpdate, &updateVolume);
						}
						break;

						case Setup_Source_Input::IT_Button :
						{
							sourceInput->Store = UTILITY::Invert( sourceInput->Store);
							storeCheckBox.Display( sourceInput->Store);
							ValueChanged = true;
						}
						break;
					}
				}
				break;

				case 8 :
				{
					switch( sourceInput->Type)
					{
						case Setup_Source_Input::IT_Analog :
						{
							ValueChanged = GUI_Setup_Select::DoSelect16(
								&( sourceInput->BottomVolume), SIGNAL_MINIMUM_VALUE,
								SIGNAL_MAXIMUM_VALUE, SIGNAL_SOURCE_INPUT_VOLUME_SIGNAL_PER_VALUE,
								&menuMarker, &bottomValueLabel, this, &staticUpdate, &updateVolume);
						}
						break;

						case Setup_Source_Input::IT_Button :
						{
							ValueChanged = GUI_Setup_Select::DoSelect16(
								&( sourceInput->InitVolume), SIGNAL_MINIMUM_VALUE,
								SIGNAL_MAXIMUM_VALUE, SIGNAL_SOURCE_INPUT_INIT_SIGNAL_PER_VALUE,
								&menuMarker, &initValueLabel, this, &staticUpdate, &updateInit);
						}
						break;

						case Setup_Source_Input::IT_Rotary :
						case Setup_Source_Input::IT_Ticker :
						{
							sourceInput->Store = UTILITY::Invert( sourceInput->Store);
							storeCheckBox.Display( sourceInput->Store);
							ValueChanged = true;
						}
						break;

						case Setup_Source_Input::IT_Switch :
						{
							ValueChanged = GUI_Setup_Select::DoSelect16(
								&( sourceInput->TopVolume), SIGNAL_MINIMUM_VALUE,
								SIGNAL_MAXIMUM_VALUE, SIGNAL_SOURCE_INPUT_VOLUME_SIGNAL_PER_VALUE,
								&menuMarker, &topValueLabel, this, &staticUpdate, &updateVolume);
						}
						break;
					}
				}
				break;

				case 9 :
				{
					switch( sourceInput->Type)
					{
						case Setup_Source_Input::IT_Button :
						{
							sourceInput->Toggle = UTILITY::Invert( sourceInput->Toggle);
							toggleCheckBox.Display( sourceInput->Toggle);
							ValueChanged = true;
						}
						break;

						case Setup_Source_Input::IT_Rotary :
						case Setup_Source_Input::IT_Ticker :
						{
							ValueChanged = GUI_Setup_Select::DoSelect16(
								&( sourceInput->InitVolume), SIGNAL_MINIMUM_VALUE,
								SIGNAL_MAXIMUM_VALUE, SIGNAL_SOURCE_INPUT_INIT_SIGNAL_PER_VALUE,
								&menuMarker, &initValueLabel, this, &staticUpdate, &updateInit);
						}
						break;

						case Setup_Source_Input::IT_Switch :
						{
							ValueChanged = GUI_Setup_Select::DoSelect16(
								&( sourceInput->BottomVolume), SIGNAL_MINIMUM_VALUE,
								SIGNAL_MAXIMUM_VALUE, SIGNAL_SOURCE_INPUT_VOLUME_SIGNAL_PER_VALUE,
								&menuMarker, &bottomValueLabel, this, &staticUpdate, &updateVolume);
						}
						break;
					}
				}
				break;

				case 10 :
				{
					switch( sourceInput->Type)
					{
						case Setup_Source_Input::IT_Rotary :
						case Setup_Source_Input::IT_Ticker :
						{
							ValueChanged = GUI_Setup_Select::DoSelect16(
								&( sourceInput->StepVolume), SIGNAL_MINIMUM_VALUE,
								SIGNAL_MAXIMUM_VALUE, SIGNAL_SOURCE_INPUT_STEP_SIGNAL_PER_VALUE,
								&menuMarker, &stepValueLabel, this, &staticUpdate, &updateStep);
						}
						break;
					}
				}
				break;

				case 11 :
				{
					switch( sourceInput->Type)
					{
						case Setup_Source_Input::IT_Button :
						{
							ValueChanged = GUI_Setup_Select::DoSelect16(
								&( sourceInput->TopVolume), SIGNAL_MINIMUM_VALUE,
								SIGNAL_MAXIMUM_VALUE, SIGNAL_SOURCE_INPUT_VOLUME_SIGNAL_PER_VALUE,
								&menuMarker, &topValueLabel, this, &staticUpdate, &updateVolume);
						}
						break;
					}
				}
				break;

				case 12 :
				{
					switch( sourceInput->Type)
					{
						case Setup_Source_Input::IT_Button :
						{
							ValueChanged = GUI_Setup_Select::DoSelect16(
								&( sourceInput->BottomVolume), SIGNAL_MINIMUM_VALUE,
								SIGNAL_MAXIMUM_VALUE, SIGNAL_SOURCE_INPUT_VOLUME_SIGNAL_PER_VALUE,
								&menuMarker, &bottomValueLabel, this, &staticUpdate, &updateVolume);
						}
						break;

						case Setup_Source_Input::IT_Rotary :
						case Setup_Source_Input::IT_Ticker :
						{
							ValueChanged = GUI_Setup_Select::DoSelect16(
								&( sourceInput->TopVolume), SIGNAL_MINIMUM_VALUE,
								SIGNAL_MAXIMUM_VALUE, SIGNAL_SOURCE_INPUT_VOLUME_SIGNAL_PER_VALUE,
								&menuMarker, &topValueLabel, this, &staticUpdate, &updateVolume);
						}
						break;
					}
				}
				break;

				case 13 :
				{
					switch( sourceInput->Type)
					{
						case Setup_Source_Input::IT_Rotary :
						case Setup_Source_Input::IT_Ticker :
						{
							ValueChanged = GUI_Setup_Select::DoSelect16(
								&( sourceInput->BottomVolume), SIGNAL_MINIMUM_VALUE,
								SIGNAL_MAXIMUM_VALUE, SIGNAL_SOURCE_INPUT_VOLUME_SIGNAL_PER_VALUE,
								&menuMarker, &bottomValueLabel, this, &staticUpdate, &updateVolume);
						}
						break;
					}
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
				GLOBAL.SetupService.SetSourceInput( setupSourceId, sourceInput);
			}
		}
		break;

		default : break;
	}

	return( true);
}

void Screen_Setup_Source_Input::updateType( void* Object, GUI_Setup_Label* Label, int8_t Value)
{
	// Refresh label.
	(( Screen_Setup_Source_Input*) Object)->updateType();
}

void Screen_Setup_Source_Input::updateInputAnalog( void* Object, GUI_Setup_Label* Label,
												   int8_t Value)
{
	// Refresh label.
	Label->Clear();
	setAnalogInput((( Screen_Setup_Source_Input*) Object)->inputAValue, Value);
	Label->Display();
}

void Screen_Setup_Source_Input::updateInputDigitalA( void* Object, GUI_Setup_Label* Label,
													 int8_t Value)
{
	// Refresh label.
	Label->Clear();
	setDigitalInput((( Screen_Setup_Source_Input*) Object)->inputAValue, Value);
	Label->Display();
}

void Screen_Setup_Source_Input::updateInputDigitalB( void* Object, GUI_Setup_Label* Label,
													 int8_t Value)
{
	// Refresh label.
	Label->Clear();
	setDigitalInput((( Screen_Setup_Source_Input*) Object)->inputBValue, Value);
	Label->Display();
}

void Screen_Setup_Source_Input::updateInit( void* Object, GUI_Setup_Label* Label, int16_t Value)
{
	// Refresh label.
	Label->SetValue( Value / SIGNAL_SOURCE_INPUT_INIT_SIGNAL_PER_VALUE);
	Label->Display();
}

void Screen_Setup_Source_Input::updateStep( void* Object, GUI_Setup_Label* Label, int16_t Value)
{
	// Refresh label.
	Label->SetValue( Value / SIGNAL_SOURCE_INPUT_STEP_SIGNAL_PER_VALUE);
	Label->Display();
}

void Screen_Setup_Source_Input::updateVolume( void* Object, GUI_Setup_Label* Label, int16_t Value)
{
	// Refresh label.
	Label->SetValue( Value / SIGNAL_SOURCE_INPUT_VOLUME_SIGNAL_PER_VALUE);
	Label->Display();
}

void Screen_Setup_Source_Input::updateType( void)
{
	clear();
	draw();

	// Refresh menu pattern.
	SetMenuPattern( getMenuPattern( sourceInput->Type));
}

void Screen_Setup_Source_Input::clear( void)
{
	typeValueLabel.Clear();

	// Inputs.
	switch( previousType)
	{
		case Setup_Source_Input::IT_Rotary :
		case Setup_Source_Input::IT_Switch :
		case Setup_Source_Input::IT_Ticker :
		{
			inputALabel.Clear();
			inputAValueLabel.Clear();
			inputBLabel.Clear();
			inputBValueLabel.Clear();
		}
		break;

		default :
		{
			inputALabel.Clear();
			inputAValueLabel.Clear();
		}
		break;
	}

	// Store & Init
	switch( previousType)
	{
		case Setup_Source_Input::IT_Button :
		case Setup_Source_Input::IT_Rotary :
		case Setup_Source_Input::IT_Ticker :
		{
			storeLabel.Clear();
			storeCheckBox.Clear();
			initLabel.Clear();
			initValueLabel.Clear();
		}
		break;
	}
	
	// Step
	switch( previousType)
	{
		case Setup_Source_Input::IT_Rotary :
		case Setup_Source_Input::IT_Ticker :
		{
			stepLabel.Clear();
			stepValueLabel.Clear();
		}
		break;
	}

	// Toggle
	switch( previousType)
	{
		case Setup_Source_Input::IT_Button :
		{
			toggleLabel.Clear();
			toggleCheckBox.Clear();
		}
		break;
	}

	topLabel.Clear();
	topValueLabel.Clear();
	bottomLabel.Clear();
	bottomValueLabel.Clear();
	
	previousType = sourceInput->Type;
}

void Screen_Setup_Source_Input::draw( void)
{
	// Adjust gauges to frame and set them up.
	const FONT_Type* Font = FONT::GetFont( SCREEN_SETUP_BASE_MAIN_FONT);

	uint16_t ValueLeft = menuLeft + 7 * Font->CellWidth;
	uint16_t InputLeft = ValueLeft + 6 * Font->CellWidth;
	uint16_t ContentWidth = frameWidth - ( ValueLeft - frameLeft) - 1;

	uint8_t Line = 3;

	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Type);
	typeValueLabel.SetDimensions( ValueLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT));

	Line += 2;

	// Input A
	inputALabel.SetDimensions( menuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT));
	inputAValueLabel.SetDimensions( InputLeft,
								    frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT));

	// Input B
	switch( sourceInput->Type)
	{
		case Setup_Source_Input::IT_Switch :
		case Setup_Source_Input::IT_Rotary :
		case Setup_Source_Input::IT_Ticker :
		{
			inputBLabel.SetDimensions( menuLeft,
									   frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT));
			inputBValueLabel.SetDimensions( InputLeft,
										    frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT));
		}
		break;
	}

	// Store & Init
	switch( sourceInput->Type)
	{
		case Setup_Source_Input::IT_Button :
		case Setup_Source_Input::IT_Rotary :
		case Setup_Source_Input::IT_Ticker :
		{
			Line++;

			storeLabel.SetDimensions( menuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT));
			storeCheckBox.SetDimensions( InputLeft,
										 frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
										 ContentWidth, SCREEN_SETUP_BASE_GAUGE_THICKNESS);

			initLabel.SetDimensions( menuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT));
			initValueLabel.SetDimensions( InputLeft, frameTop +
										             ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT));
		}
		break;
	}
	
	// Step
	switch( sourceInput->Type)
	{
		case Setup_Source_Input::IT_Rotary :
		case Setup_Source_Input::IT_Ticker :
		{
			stepLabel.SetDimensions( menuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT));
			stepValueLabel.SetDimensions( InputLeft,
										  frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT));
		}
		break;
	}
	
	// Toggle
	switch( sourceInput->Type)
	{
		case Setup_Source_Input::IT_Button :
		{
			toggleLabel.SetDimensions( menuLeft,
									   frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT));
			toggleCheckBox.SetDimensions( InputLeft,
										  frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
										  ContentWidth, SCREEN_SETUP_BASE_GAUGE_THICKNESS);
		}
		break;
	}

	Line++;

	// Volume
	if( sourceInput->Type != Setup_Source_Input::IT_Analog)
	{
		topLabel.SetDimensions( menuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT));
		topValueLabel.SetDimensions( InputLeft,
									 frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT));
		bottomLabel.SetDimensions( menuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT));
		bottomValueLabel.SetDimensions( InputLeft,
									    frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT));
	}

	// Refresh label.
	switch( sourceInput->Type)
	{
		case Setup_Source_Input::IT_Analog : typeValueLabel.Display_P( Text::Analog);	break;
		case Setup_Source_Input::IT_Button : typeValueLabel.Display_P( Text::Button);	break;
		case Setup_Source_Input::IT_Switch : typeValueLabel.Display_P( Text::Switch);	break;
		case Setup_Source_Input::IT_Ticker : typeValueLabel.Display_P( Text::Ticker);	break;
		case Setup_Source_Input::IT_Rotary : typeValueLabel.Display_P( Text::Rotary);	break;
	}
	
	switch( sourceInput->Type)
	{
		case Setup_Source_Input::IT_Analog :
		{
			setAnalogInput( inputAValue, sourceInput->InputIdA);
		}
		break;

		case Setup_Source_Input::IT_Button :
		case Setup_Source_Input::IT_Rotary :
		case Setup_Source_Input::IT_Switch :
		case Setup_Source_Input::IT_Ticker :
		{
			setDigitalInput( inputAValue, sourceInput->InputIdA);
			setDigitalInput( inputBValue, sourceInput->InputIdB);
		}
		break;
	}
	
	// Refresh inputs.
	switch( sourceInput->Type)
	{
		case Setup_Source_Input::IT_Analog :
		case Setup_Source_Input::IT_Button :
		{
			inputALabel.Display_P( Text::Input);
			inputAValueLabel.Display();
		}
		break;

		case Setup_Source_Input::IT_Rotary :
		case Setup_Source_Input::IT_Switch :
		case Setup_Source_Input::IT_Ticker :
		{
			inputALabel.Display_P( Text::InputA);
			inputAValueLabel.Display();
			inputBLabel.Display_P( Text::InputB);
			inputBValueLabel.Display();
		}
		break;
	}
	
	// Store & Init
	switch( sourceInput->Type)
	{
		case Setup_Source_Input::IT_Button :
		case Setup_Source_Input::IT_Rotary :
		case Setup_Source_Input::IT_Ticker :
		{
			storeCheckBox.Display( sourceInput->Store);

			storeLabel.Display_P( Text::Store);
			initLabel.Display_P( Text::Init);
			initValueLabel.SetValue( sourceInput->InitVolume /
									 SIGNAL_SOURCE_INPUT_INIT_SIGNAL_PER_VALUE);
			initValueLabel.Display();		
		}
		break;
	}
	
	// Step
	switch( sourceInput->Type)
	{
		case Setup_Source_Input::IT_Rotary :
		case Setup_Source_Input::IT_Ticker :
		{
			stepLabel.Display_P( Text::Step);
			stepValueLabel.SetValue( sourceInput->StepVolume /
									 SIGNAL_SOURCE_INPUT_INIT_SIGNAL_PER_VALUE);
			stepValueLabel.Display();
		}
		break;
	}
	
	// Toggle
	switch( sourceInput->Type)
	{
		case Setup_Source_Input::IT_Button :
		{
			toggleCheckBox.Display( sourceInput->Toggle);
			toggleLabel.Display_P( Text::Toggle);
		}
		break;
	}

	// Volume
	if( sourceInput->Type != Setup_Source_Input::IT_Analog)
	{
		topLabel.Display_P( Text::Top);
		topValueLabel.SetValue( sourceInput->TopVolume /
							    SIGNAL_SOURCE_INPUT_VOLUME_SIGNAL_PER_VALUE);
		topValueLabel.Display();
		bottomLabel.Display_P( Text::Bottom);
		bottomValueLabel.SetValue( sourceInput->BottomVolume /
								   SIGNAL_SOURCE_INPUT_VOLUME_SIGNAL_PER_VALUE);
		bottomValueLabel.Display();
	}
}
