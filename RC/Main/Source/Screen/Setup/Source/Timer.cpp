// Copyright 2011 Peter Stegemann

#include "Timer.h"

#include "GUI/Setup/Select.h"
#include "Main/Global.h"
#include "Signal/Processor.h"
#include "Text/Text.h"

#include "AVR/Source/Utility.h"

#define TIME_LIMIT					( 5 * 60 * 60)
#define TIME_STEP_WIDTH				5

#define PAUSE_TIME_LIMIT			( 5 * 60 * 60)
#define PAUSE_TIME_STEP_WIDTH		1

Screen_Setup_Source_Timer::Screen_Setup_Source_Timer
(
    Input_Service* InputService,
    Interrupt_Service* InterruptService,
    Screen_Status_Status* StatusScreen,
    uint8_t SignalSourceId
)
    : Screen_Setup_Source_Base
    (
        InputService,
        InterruptService,
        StatusScreen,
        SignalSourceId,
        0b1011010101110101,
        Text::Timer,
        false
    )
    , select( InputService, InterruptService)
    , sourceTimer( NULL)
{
	sourceTimer = &( source->Body.Timer);

	initTimeLabel.SetOptions( GUI_Setup_Label::O_Proportional);

	triggerGauge.SetOptions(( GUI_Setup_Gauge::Options)( GUI_Setup_Gauge::O_Percentage |
														 GUI_Setup_Gauge::O_DualPercentage |
														 GUI_Setup_Gauge::O_CenterLine |
														 GUI_Setup_Gauge::O_Marker));

	triggerHighLimitLabel.SetText_P( Text::FourDigitPercentFormat);
	triggerHighLimitLabel.SetOptions( GUI_Setup_Label::O_Proportional);

	triggerLowLimitLabel.SetText_P( Text::FourDigitPercentFormat);
	triggerLowLimitLabel.SetOptions( GUI_Setup_Label::O_Proportional);

	warnLowTimeLabel.SetOptions( GUI_Setup_Label::O_Proportional);
	warnCriticalTimeLabel.SetOptions( GUI_Setup_Label::O_Proportional);

	warnPauseTimeLabel.SetOptions( GUI_Setup_Label::O_Proportional);
}

void Screen_Setup_Source_Timer::display( void)
{
	Screen_Setup_Source_Base::display();

	// Adjust gauges to frame and set them up.
	const avr::font::Type* Font = avr::font::Font::Get( SCREEN_SETUP_BASE_MAIN_FONT);
	const avr::font::Type* LargeFont = avr::font::Font::Get( avr::font::FI_Large);

	uint16_t ValueLeft = menuLeft + 10 * Font->GetCellWidth();
	uint16_t ContentWidth = frameWidth - ( ValueLeft - frameLeft) - 1;
	uint16_t GaugeWidth = frameWidth - ( ValueLeft - frameLeft) - 1;
	
	uint16_t LowLeft = menuLeft;
	uint16_t LowValueLeft = LowLeft + 14 * Font->GetCellWidth();
	uint16_t HighLeft = LowValueLeft + ( 10 + 1) * Font->GetCellWidth();
	uint16_t HighValueLeft = HighLeft + 7 * Font->GetCellWidth();

	uint16_t TextLeft = frameWidth - ( 8 * LargeFont->GetCellWidth());
	timeLabel.SetDimensions( TextLeft, frameTop + ( 4 * SCREEN_SETUP_BASE_LINE_HEIGHT));
	timeLabel.SetFont( avr::font::FI_Large);

	uint8_t Line = 4;

	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Init);
	initTimeLabel.SetDimensions( ValueLeft,
								 frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT));
	initTimeLabel.SetTime( sourceTimer->Setup.InitTime);

	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Reverse);
	reverseCheckBox.SetDimensions( ValueLeft,
								   frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
								   ContentWidth, SCREEN_SETUP_BASE_GAUGE_THICKNESS);
	reverseCheckBox.Display( sourceTimer->Setup.Reverse);

	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Store);
	storeCheckBox.SetDimensions( ValueLeft,
								 frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 ContentWidth, SCREEN_SETUP_BASE_GAUGE_THICKNESS);
	storeCheckBox.Display( sourceTimer->Setup.Store);

	Line++;

	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Trigger);
	triggerNameLabel.SetDimensions( ValueLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT));

	SetSourceLabel( &triggerNameLabel, triggerName, sourceTimer->TriggerSignalSourceId);

	Line++;

	triggerGauge.SetDimensions( ValueLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								GaugeWidth, SCREEN_SETUP_BASE_GAUGE_THICKNESS);

	Line++;

	GLOBAL.SetupDisplay.Print_P( LowLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::TriggerLowLimit);

	triggerLowLimitLabel.SetDimensions( LowValueLeft,
										frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT));
	triggerLowLimitLabel.SetValue( sourceTimer->Setup.TriggerLowLimit /
								   SIGNAL_SOURCE_FOLLOWER_TRIGGER_SIGNAL_PER_VALUE);
	triggerLowLimitLabel.Display();

	GLOBAL.SetupDisplay.Print_P( HighLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::TriggerHighLimit);

	triggerHighLimitLabel.SetDimensions( HighValueLeft,
										 frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT));
	triggerHighLimitLabel.SetValue( sourceTimer->Setup.TriggerHighLimit /
									SIGNAL_SOURCE_FOLLOWER_TRIGGER_SIGNAL_PER_VALUE);
	triggerHighLimitLabel.Display();

	Line++;

	GLOBAL.SetupDisplay.Print_P( LowLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::WarnLow);

	warnLowTimeLabel.SetDimensions( LowValueLeft + 2 * Font->GetCellWidth(),
								    frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT));
	warnLowTimeLabel.SetTime( sourceTimer->Setup.WarnLowTime);

	GLOBAL.SetupDisplay.Print_P( HighLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::WarnCritical);
	warnCriticalTimeLabel.SetDimensions( HighValueLeft + 2 * Font->GetCellWidth(),
										 frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT));
	warnCriticalTimeLabel.SetTime( sourceTimer->Setup.WarnCriticalTime);

	GLOBAL.SetupDisplay.Print_P( LowLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::WarnPause);
	warnPauseTimeLabel.SetDimensions( LowValueLeft + 2 * Font->GetCellWidth(),
									  frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT));
	warnPauseTimeLabel.SetTime( sourceTimer->Setup.WarnPauseTime);

	Line++;

	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Reset);
}

void Screen_Setup_Source_Timer::update( void)
{
	Screen_Setup_Source_Base::update();

	triggerGauge.Display( SIGNAL_MINIMUM_VALUE, SIGNAL_MAXIMUM_VALUE,
						  GLOBAL.SignalProcessor.GetSourceValue(
								sourceTimer->TriggerSignalSourceId));

	timeLabel.SetTime( sourceTimer->GetTime());
/*
	GLOBAL.SetupDisplay.PrintFormat( 10, 10, avr::font::FI_Medium, LCD_65K_RGB::C_Black,
									 LCD_65K_RGB::C_White, LCD::PO_Fixed, "%d %d",
									 sourceTimer->Setup.Reverse, !sourceTimer->Setup.Reverse);
*/
}

bool Screen_Setup_Source_Timer::processMenu( DoMenuResult Result)
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
					ValueChanged = select.DoSelectTime
					(
						&( sourceTimer->Setup.InitTime), 0, TIME_LIMIT, TIME_STEP_WIDTH,
						&menuMarker, &initTimeLabel, this, &updateTime
                    );
				}
				break;

				case 5 :
				{
					sourceTimer->Setup.Reverse = avr::Utility::Invert( sourceTimer->Setup.Reverse);
					reverseCheckBox.Display( sourceTimer->Setup.Reverse);
					ValueChanged = true;
				}
				break;

				case 6 :
				{
					sourceTimer->Setup.Store = avr::Utility::Invert( sourceTimer->Setup.Store);
					storeCheckBox.Display( sourceTimer->Setup.Store);
					ValueChanged = true;
				}
				break;

				case 8 :
				{
					ValueChanged = select.DoSourceSelect
					(
						&( sourceTimer->TriggerSignalSourceId),
						&( sourceTimer->Setup.TriggerSource), &menuMarker,
						&triggerNameLabel, NULL, triggerName, this, &staticUpdate, false,
						source->GetLevel()
                    );
				}
				break;

				case 10 :
				{
					bool LowChanged = select.DoSelect16
					(
						&( sourceTimer->Setup.TriggerLowLimit), SIGNAL_MINIMUM_VALUE,
						SIGNAL_MAXIMUM_VALUE, SIGNAL_SOURCE_FOLLOWER_TRIGGER_SIGNAL_PER_VALUE,
						&menuMarker, &triggerLowLimitLabel, this, &staticUpdate,
						&updateTriggerLimit
                    );

					bool HighChanged = select.DoSelect16
					(
						&( sourceTimer->Setup.TriggerHighLimit), SIGNAL_MINIMUM_VALUE,
						SIGNAL_MAXIMUM_VALUE, SIGNAL_SOURCE_FOLLOWER_TRIGGER_SIGNAL_PER_VALUE,
						&menuMarker, &triggerHighLimitLabel, this, &staticUpdate,
						&updateTriggerLimit
                    );

					ValueChanged = HighChanged || LowChanged;
				}
				break;

				case 12 :
				{
					bool LowChanged = select.DoSelectTime
					(
						&( sourceTimer->Setup.WarnLowTime), 0, TIME_LIMIT, TIME_STEP_WIDTH,
						&menuMarker, &warnLowTimeLabel, this, &updateTime
                    );

					bool HighChanged = select.DoSelectTime
					(
						&( sourceTimer->Setup.WarnCriticalTime), 0, TIME_LIMIT, TIME_STEP_WIDTH,
						&menuMarker, &warnCriticalTimeLabel, this, &updateTime
                    );

					ValueChanged = HighChanged || LowChanged;
				}
				break;

				case 13 :
				{
					ValueChanged = select.DoSelectTime
					(
						&( sourceTimer->Setup.WarnPauseTime), 0, PAUSE_TIME_LIMIT,
						PAUSE_TIME_STEP_WIDTH, &menuMarker, &warnPauseTimeLabel, this, &updateTime
                    );
				}
				break;

				case 15 :
				{
					sourceTimer->ResetTime();
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
				GLOBAL.SetupService.SetSourceTimer( setupSourceId, &( sourceTimer->Setup));
			}
		}
		break;

		case DMR_Changed :	break;

		default : break;
	}

	return( true);
}

void Screen_Setup_Source_Timer::updateTime( void* Object, GUI_Setup_Label* Label, int16_t Value)
{
	(( GUI_Setup_TimeLabel*) Label)->SetTime( Value);
}

void Screen_Setup_Source_Timer::updateTriggerLimit
(
    void* Object, GUI_Setup_Label* Label, int16_t Value
)
{
	// Refresh label.
	(( Screen_Setup_Source_Timer*) Object)->updateVolume
	(
	    Label, Value, SIGNAL_SOURCE_FOLLOWER_TRIGGER_SIGNAL_PER_VALUE
    );
}

void Screen_Setup_Source_Timer::updateVolume
(
    GUI_Setup_Label* Label, int16_t Value, int16_t SignalPerValue
)
{
	// Refresh label.		
	Label->SetValue( Value / SignalPerValue);
	Label->Display();
}
