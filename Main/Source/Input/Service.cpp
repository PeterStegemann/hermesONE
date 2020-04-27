// Copyright 2007 Peter Stegemann

#include "Service.h"

#include "Ports.h"

#include "AVR/Source/Utility.h"

//#include <util/atomic.h>

//#define INPUT_MENU_PINS		( UTILITY_BitValue( INPUT_MENU_ROTARY_A) |
//								  UTILITY_BitValue( INPUT_MENU_ROTARY_B) |
//								  UTILITY_BitValue( INPUT_MENU_C) | UTILITY_BitValue( INPUT_MENU_D))
#define INPUT_MENU_PINS		( UTILITY_BitValue( INPUT_MENU_ROTARY_A) | \
							  UTILITY_BitValue( INPUT_MENU_ROTARY_B) | \
							  UTILITY_BitValue( INPUT_MENU_ROTARY_BUTTON))

void Input_Service::Initialize( void)
{
	rotarySelect = 0;
	lastRotaryButton = false;
	rotaryButton = 0;

	for( uint8_t Count = 0; Count < sizeof( digitalInput); Count++)
	{
		digitalInput[ Count] = 0;
	}

	// Rotary select
	uint8_t RotaryInput = INPUT_MENU_PIN;

	bool LowInput = UTILITY_GetBit( RotaryInput, INPUT_MENU_ROTARY_A);
	bool HighInput = UTILITY_GetBit( RotaryInput, INPUT_MENU_ROTARY_B);

	rotary.Initialize( LowInput, HighInput);

	lastActivityUptime = GLOBAL.StatusTime.GetUptime();

	// All used pins to input and set internal pull up resistors.
	#if defined( INPUT_DIGITAL_H_I_DDR)
		INPUT_DIGITAL_H_I_DDR = 0x00;
		INPUT_DIGITAL_H_I_PORT = 0xff;
	#endif
	#if defined( INPUT_DIGITAL_J_K_DDR)
		INPUT_DIGITAL_J_K_DDR = 0x00;
		INPUT_DIGITAL_J_K_PORT = 0xff;
	#endif
	#if defined( INPUT_DIGITAL_L_DDR)
		INPUT_DIGITAL_L_DDR &= ~0xf0;
		INPUT_DIGITAL_L_PORT |= 0xf0;
	#endif
	#if defined( INPUT_DIGITAL_L_M_DDR)
		INPUT_DIGITAL_L_M_DDR = 0x00;
		INPUT_DIGITAL_L_M_PORT = 0xff;
		INPUT_DIGITAL_N_O_DDR = 0x00;
		INPUT_DIGITAL_N_O_PORT = 0xff;
	#endif

	INPUT_MENU_DDR &= ~INPUT_MENU_PINS;
	INPUT_MENU_PORT |= INPUT_MENU_PINS;
}

bool Input_Service::GetDigitalInput( uint8_t InputId)
{
	if( InputId >= INPUT_DIGITAL_PORTS)
	{
		return( false);
	}

	return( UTILITY_GetBit( digitalInput[ InputId / 8], InputId % 8) == false);
}
/*
uint8_t Processor::GetKeys( uint8_t* Changed)
{
	uint8_t NewKeys = INPUT_MENU_PIN;

	// Evaluate changed keys.
	if( Changed != NULL)
	{
		*Changed = lastKeys ^ NewKeys;
	}

	// Remember for next time.
	lastKeys = NewKeys;

	return( NewKeys);
}
*/
void Input_Service::GetRotary( int8_t* Select, uint8_t* Button, bool* CurrentButton,
							   uint16_t* ButtonTime)
{
	if( Select != NULL)
	{
//		ATOMIC_BLOCK( ATOMIC_RESTORESTATE)
		{
			*Select = rotarySelect;
			rotarySelect = 0;
		}

		/*
		if( rotarySelect != 0)
		{
			GLOBAL.StatusService.Tick();
		}
		*/
	}

	if( Button != NULL)
	{
// 		ATOMIC_BLOCK( ATOMIC_RESTORESTATE)
		{
			*Button = rotaryButton;
			rotaryButton = 0;
		}

		/*
		if( rotaryButton != 0)
		{
			GLOBAL.StatusService.Beep();
		}
		*/
	}

	if( CurrentButton != NULL)
	{
		*CurrentButton = lastRotaryButton;
	}

	if( ButtonTime != NULL)
	{
		*ButtonTime = rotaryButtonTime;
	}
}

uint16_t Input_Service::GetLastActivityUptime( void)
{
	return( lastActivityUptime);
}

void Input_Service::Process( void)
{
	bool HasUserActivity = false;

	uint8_t RotaryInput = INPUT_MENU_PIN;
	uint8_t RotaryDifference =
		rotary.CalculateDifference( UTILITY_GetBit( RotaryInput, INPUT_MENU_ROTARY_A),
									UTILITY_GetBit( RotaryInput, INPUT_MENU_ROTARY_B));

	if( RotaryDifference != 0)
	{
		HasUserActivity = true;
	}

	rotarySelect -= RotaryDifference;

	// Rotary button
	bool RotaryButtonPressed = ( UTILITY_GetBit( INPUT_MENU_PIN, INPUT_MENU_ROTARY_BUTTON) == 0);

	if( RotaryButtonPressed == true)
	{
		rotaryButtonTime++;
	}

	// React on state change.
	if( RotaryButtonPressed != lastRotaryButton)
	{
		HasUserActivity = true;

		if( RotaryButtonPressed == false)
		{
			// React on button beeing released;
			rotaryButton++;
		}
		else
		{
			// Start time count anew.
			rotaryButtonTime = 0;
		}

		// Remind last state.
		lastRotaryButton = RotaryButtonPressed;
	}

	// Get digital inputs (not counted as user activity).
	#if defined( INPUT_DIGITAL_H_I_PIN)
		digitalInput[ 0] = INPUT_DIGITAL_H_I_PIN;
	#endif
	#if defined( INPUT_DIGITAL_J_K_PIN)
		digitalInput[ 1] = INPUT_DIGITAL_J_K_PIN;
	#endif
	#if defined( INPUT_DIGITAL_L_PIN)
		digitalInput[ 2] = INPUT_DIGITAL_L_PIN >> 4;
	#endif
	#if defined( INPUT_DIGITAL_L_M_PIN)
		digitalInput[ 2] = INPUT_DIGITAL_L_M_PIN;
		digitalInput[ 3] = INPUT_DIGITAL_N_O_PIN;
	#endif

	if( HasUserActivity == true)
	{
		// Update activity uptime.
		lastActivityUptime = GLOBAL.StatusTime.GetUptime();
	}
}
