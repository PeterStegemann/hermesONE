// Copyright 2008 Peter Stegemann

#include "Input.h"

#include "Main/Global.h"
#include "Signal/Processor.h"

#define BUZZ_LIMIT_LENGTH		10
#define BUZZ_LIMIT_PAUSE		20
#define BUZZ_LIMIT_REPEAT		2

#define BUZZ_CENTER_LENGTH		10
#define BUZZ_CENTER_PAUSE		5
#define BUZZ_CENTER_REPEAT		4

#define BUZZ_TICK_LENGTH		5
#define BUZZ_TICK_PAUSE			0
#define BUZZ_TICK_REPEAT		1

void Signal_Source_Input::Initialize( Input_Service* InputService, Status_Service* StatusService)
{
    inputService = InputService;
    statusService = StatusService;

	value = Setup.InitVolume;
	lastLowInput = inputService->GetDigitalInput( Setup.InputIdA);
	lastHighInput = inputService->GetDigitalInput( Setup.InputIdB);
	modified = false;

	rotary.Initialize( lastLowInput, lastHighInput);
}

void Signal_Source_Input::Reset( void)
{
	value = SIGNAL_MINIMUM_VALUE;
	lastLowInput = inputService->GetDigitalInput( Setup.InputIdA);
	lastHighInput = inputService->GetDigitalInput( Setup.InputIdB);

	Setup.Type = Setup_Source_Input::IT_Button;
	Setup.InputIdA = 0;
	Setup.InputIdB = 0;

	Setup.Store = false;
	Setup.InitVolume = SIGNAL_MINIMUM_VALUE;
	Setup.StepVolume = SIGNAL_MAXIMUM_VALUE / 100;
	Setup.Toggle = false;

	Setup.TopVolume = SIGNAL_MAXIMUM_VALUE;
	Setup.BottomVolume = SIGNAL_MINIMUM_VALUE;

	modified = true;
}

void Signal_Source_Input::LoadSetup( uint16_t SetupSourceId)
{
	GLOBAL.SetupService.GetSourceInput( SetupSourceId, &Setup);
}

void Signal_Source_Input::StoreIfModified( uint16_t SetupSourceId)
{
	if( modified == true)
	{
		modified = false;

		GLOBAL.SetupService.SetSourceInput( SetupSourceId, &Setup);
	}
}

int16_t Signal_Source_Input::CalculateValue( const Signal_Processor* SignalProcessor)
{
	switch( Setup.Type)
	{
		case Setup_Source_Input::IT_Analog :
		{
			value = SignalProcessor->GetCalibratedAnalogValue( Setup.InputIdA);
		}
		break;

		case Setup_Source_Input::IT_Button :
		{
			if( Setup.Toggle == false)
			{
				if( inputService->GetDigitalInput( Setup.InputIdA) == true)
				{
					value = Setup.TopVolume;
				}
				else
				{
					value = Setup.BottomVolume;
				}
			}
			else
			{
				// Button was pressed and released?
				if(( lastLowInput == false) &&
				   ( inputService->GetDigitalInput( Setup.InputIdA) == true))
				{
					if( value == Setup.TopVolume)
					{
						value = Setup.BottomVolume;
					}
					else
					{
						value = Setup.TopVolume;
					}

					if( Setup.Store == true)
					{
						Setup.InitVolume = value;
						modified = true;
					}
				}

				lastLowInput = inputService->GetDigitalInput( Setup.InputIdA);
			}
		}
		break;

		case Setup_Source_Input::IT_Switch :
		{
			// This is undefined for low & high == true and ends up low.
			if( inputService->GetDigitalInput( Setup.InputIdA) == true)
			{
				value = Setup.TopVolume;
			}
			else if( inputService->GetDigitalInput( Setup.InputIdB) == true)
			{
				value = Setup.BottomVolume;
			}
			else
			{
				int32_t Center = Setup.BottomVolume;
				Center += Setup.TopVolume;
				Center >>= 1;

				value = Center;
			}
		}
		break;

		case Setup_Source_Input::IT_Ticker :
		{
			bool Tick = false;

			int16_t Step = 0;

			// Check if there was a tick and what was its direction. This is undefined for low &
			// high == true and ends up low.
			if(( lastLowInput == false) &&
			   ( inputService->GetDigitalInput( Setup.InputIdA) == true))
			{
				Step = Setup.StepVolume;
				Tick = true;
			}

			if(( lastHighInput == false) &&
			   ( inputService->GetDigitalInput( Setup.InputIdB) == true))
			{
				Step = -Setup.StepVolume;
				Tick = true;
			}

			// If there was a tick, calculate the new value.
			if( Tick == true)
			{
				// If the limits are flipped, correct them for the next step and turn around the
				// step direction.
				int16_t BottomLimit, TopLimit;

				if( Setup.BottomVolume < Setup.TopVolume)
				{
					BottomLimit = Setup.BottomVolume;
					TopLimit = Setup.TopVolume;
				}
				else
				{
					BottomLimit = Setup.TopVolume;
					TopLimit = Setup.BottomVolume;
					Step = -Step;
				}

				int32_t NewValue = value;
				NewValue += Step;
				
				// Check if the new value is in bounds. If not, set it to the bounds. If it is, use
				// the new value.
				if( NewValue < BottomLimit)
				{
					NewValue = BottomLimit;

					statusService->Buzz( BUZZ_LIMIT_LENGTH, BUZZ_LIMIT_PAUSE, BUZZ_LIMIT_REPEAT);
				}
				else if( NewValue > TopLimit)
				{
					NewValue = TopLimit;

					statusService->Buzz( BUZZ_LIMIT_LENGTH, BUZZ_LIMIT_PAUSE, BUZZ_LIMIT_REPEAT);
				}
				else
				{
					int16_t Align = 0;

					if( Step > 0)
					{
						Align = NewValue % Step;
					}
					else if( Step < 0)
					{
						Align = NewValue % -Step;
					}

					if( Align != 0)
					{
						NewValue -= Step + Align;
					}

					if( NewValue == 0)
					{				
						statusService->Buzz( BUZZ_CENTER_LENGTH, BUZZ_CENTER_PAUSE,
                                             BUZZ_CENTER_REPEAT);
					}
					else
					{
						statusService->Buzz( BUZZ_TICK_LENGTH, BUZZ_TICK_PAUSE, BUZZ_TICK_REPEAT);
					}
				}

				if(( NewValue != value) && ( Setup.Store == true))
				{
					Setup.InitVolume = NewValue;
					modified = true;
				}

				value = NewValue;
			}

			lastLowInput = inputService->GetDigitalInput( Setup.InputIdA);
			lastHighInput = inputService->GetDigitalInput( Setup.InputIdB);
		}
		break;

		case Setup_Source_Input::IT_Rotary :
		{
			int8_t Difference =
				rotary.CalculateDifference
				(
					inputService->GetDigitalInput( Setup.InputIdA),
					inputService->GetDigitalInput( Setup.InputIdB)
                );
			
			if( Difference != 0)
			{
				int16_t Step;

				if( Difference > 0)
				{
					Step = Setup.StepVolume;
				}
				else
				{
					Step = -Setup.StepVolume;
				}

				int32_t NewValue = value;
				NewValue += ( int32_t) Difference * Setup.StepVolume;

				if( NewValue < Setup.BottomVolume)
				{
					NewValue = Setup.BottomVolume;

					statusService->Buzz( BUZZ_LIMIT_LENGTH, BUZZ_LIMIT_PAUSE, BUZZ_LIMIT_REPEAT);
				}
				else if( NewValue > Setup.TopVolume)
				{
					NewValue = Setup.TopVolume;

					statusService->Buzz( BUZZ_LIMIT_LENGTH, BUZZ_LIMIT_PAUSE, BUZZ_LIMIT_REPEAT);
				}
				else
				{
					int16_t Align = 0;

					if( Step > 0)
					{
						Align = NewValue % Step;
					}
					else if( Step < 0)
					{
						Align = NewValue % -Step;
					}

					if( Align != 0)
					{
						NewValue -= Step + Align;
					}

					if( NewValue == 0)
					{				
						statusService->Buzz( BUZZ_CENTER_LENGTH, BUZZ_CENTER_PAUSE,
                                             BUZZ_CENTER_REPEAT);
					}
					else
					{
						statusService->Buzz( BUZZ_TICK_LENGTH, BUZZ_TICK_PAUSE, BUZZ_TICK_REPEAT);
					}
				}

				if(( NewValue != value) && ( Setup.Store == true))
				{
					Setup.InitVolume = NewValue;
					modified = true;
				}

				value = NewValue;
			}
/*			
			if( Difference != 0)
			{
				int32_t NewValue = value;
				NewValue += ( int32_t) Difference * Setup.Step;

				if( NewValue < SIGNAL_MINIMUM_VALUE)
				{
					NewValue = SIGNAL_MINIMUM_VALUE;

					statusService->Beep();
				}
				else if( NewValue > SIGNAL_MAXIMUM_VALUE)
				{
					NewValue = SIGNAL_MAXIMUM_VALUE;

					statusService->Beep();
				}
				else if( NewValue == 0)
				{				
					statusService->Beep();
				}
				else
				{
					statusService->Tick();
				}

				value = NewValue;
			}
 */
		}
		break;
	}

	// Ups?
	return( value);
}
