// Copyright 2008 Peter Stegemann

#include "Service.h"

#include "Ports.h"

#include "AVR/Source/Utility.h"

#define STATUS_SERVICE_BLINK_OFF		2000
#define STATUS_SERVICE_BLINK_ON			STATUS_SERVICE_BLINK_OFF + 100

#define STATUS_SERVICE_BEEP_LENGTH		20
#define STATUS_SERVICE_BEEP_PAUSE		0
#define STATUS_SERVICE_BEEP_REPEAT		1

#define STATUS_SERVICE_GREEN_LED_ON		UTILITY_SetBit( STATUS_SERVICE_PORT, STATUS_SERVICE_GREEN);
#define STATUS_SERVICE_GREEN_LED_OFF	UTILITY_ClearBit( STATUS_SERVICE_PORT, STATUS_SERVICE_GREEN);
#define STATUS_SERVICE_RED_LED_ON		UTILITY_SetBit( STATUS_SERVICE_PORT, STATUS_SERVICE_RED);
#define STATUS_SERVICE_RED_LED_OFF		UTILITY_ClearBit( STATUS_SERVICE_PORT, STATUS_SERVICE_RED);

Status_Service::Status_Service( void)
			  : blinkCount( 0)
			  , beepCount( 0)
			  , beepLength( 0)
			  , beepPause( 0)
			  , beepRepeat( 0)
{
}

void Status_Service::Initialize( void)
{
	UTILITY_SetBit( STATUS_SERVICE_DDR, STATUS_SERVICE_GREEN);
	UTILITY_SetBit( STATUS_SERVICE_DDR, STATUS_SERVICE_RED);
	STATUS_SERVICE_GREEN_LED_ON
	STATUS_SERVICE_RED_LED_ON

	UTILITY_SetBit( BUZZER_DDR, BUZZER_NOISE);
	UTILITY_ClearBit( BUZZER_PORT, BUZZER_NOISE);
}

void Status_Service::Process( void)
{
	// Set status led.
	bool RFEnable0 = GLOBAL.SignalService.GetPPM( 0)->GetRFEnabled();
	bool RFEnable1 = GLOBAL.SignalService.GetPPM( 1)->GetRFEnabled();

	blinkCount++;

	if( blinkCount == STATUS_SERVICE_BLINK_OFF)
	{
		if( RFEnable0 == false)
		{
			STATUS_SERVICE_RED_LED_ON
		}
		else
		{
			STATUS_SERVICE_RED_LED_OFF
		}

		if( RFEnable1 == false)
		{
			STATUS_SERVICE_GREEN_LED_ON
		}
		else
		{
			STATUS_SERVICE_GREEN_LED_OFF
		}
	}
	else if( blinkCount == STATUS_SERVICE_BLINK_ON)
	{
		// Blink green only if rf ok.
		STATUS_SERVICE_RED_LED_ON
		STATUS_SERVICE_GREEN_LED_ON

		blinkCount = 0;
	}

	// Count down beep.
	if( beepCount == 0)
	{
		if( beepRepeat != 0)
		{
			beepRepeat--;

			beepCount = beepLength + beepPause;

			UTILITY_SetBit( BUZZER_PORT, BUZZER_NOISE);
		}
	}
	else
	{
		beepCount--;

		if( beepCount == beepPause)
		{
			UTILITY_ClearBit( BUZZER_PORT, BUZZER_NOISE);
		}
	}
}

void Status_Service::Buzz( uint16_t Length, uint16_t Pause, uint8_t Repeat)
{
	beepLength = Length;
	beepPause = Pause;
	beepRepeat = Repeat;
}

void Status_Service::Beep( void)
{
	Buzz( STATUS_SERVICE_BEEP_LENGTH, STATUS_SERVICE_BEEP_PAUSE, STATUS_SERVICE_BEEP_REPEAT);
}
