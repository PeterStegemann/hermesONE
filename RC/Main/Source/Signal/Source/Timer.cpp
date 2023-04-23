// Copyright 2011 Peter Stegemann

#include "Timer.h"

#include "../Processor.h"
#include "../Utility.h"
#include "Setup/Defines.h"

#define TIMER_WARN_BEEP_LENGTH					25
#define TIMER_WARN_BEEP_PAUSE					25

#define TIMER_WARN_BEEP_BORDER_LENGTH			200
#define TIMER_WARN_BEEP_BORDER_PAUSE			25
#define TIMER_WARN_BEEP_BORDER_START_REPEAT		2
#define TIMER_WARN_BEEP_BORDER_STOP_REPEAT		5

#define TIMER_WARN_CRITICAL_REPEAT				( 5 + 1)
#define TIMER_WARN_LOW_REPEAT					1
#define TIMER_WARN_REPEAT_RANGE					( TIMER_WARN_CRITICAL_REPEAT - TIMER_WARN_LOW_REPEAT)

void Signal_Source_Timer::Initialize( void)
{
	TriggerSignalSourceId = GLOBAL.SignalProcessor.GetSignalSourceId( Setup.TriggerSource);

	if( Setup.Store == true)
	{
		time = Setup.CurrentTime;
	}
	else
	{
		time = Setup.InitTime;
	}

	lastUptime = GLOBAL.StatusTime.GetUptime();
	lastWarnTime = 0;

	modified = false;
}

void Signal_Source_Timer::Reset( void)
{
	TriggerSignalSourceId = SIGNAL_SOURCE_NONE;

	Setup.InitTime = 0;
	Setup.CurrentTime = 0;
	Setup.Reverse = false;
	Setup.Store = false;
	Setup.TriggerLowLimit = SIGNAL_MINIMUM_VALUE;
	Setup.TriggerHighLimit = SIGNAL_MAXIMUM_VALUE;
	Setup.WarnLowTime = 0;
	Setup.WarnCriticalTime = 0;
	Setup.WarnPauseTime = 10;

	time = 0;
	lastUptime = GLOBAL.StatusTime.GetUptime();
	lastWarnTime = 0;

	modified = true;
}

void Signal_Source_Timer::LoadSetup( uint16_t SetupSourceId)
{
	GLOBAL.SetupService.GetSourceTimer( SetupSourceId, &Setup);
}

void Signal_Source_Timer::StoreIfModified( uint16_t SetupSourceId)
{
	if( modified == true)
	{
		modified = false;

		GLOBAL.SetupService.SetSourceTimer( SetupSourceId, &Setup);
	}
}

int16_t Signal_Source_Timer::CalculateValue( Signal_Processor* SignalProcessor)
{
//	int16_t NewValue = SignalProcessor->GetSourceValue( SignalSourceId);

	bool TimerActive = true;

	// If no valid trigger source is set, skip trigger check.
	if( TriggerSignalSourceId < SIGNAL_SOURCES)
	{
		int16_t TriggerValue = SignalProcessor->GetSourceValue( TriggerSignalSourceId);

		if( Setup.TriggerLowLimit <= Setup.TriggerHighLimit)
		{
			// Inner trigger.
			if(( TriggerValue < Setup.TriggerLowLimit) || ( TriggerValue > Setup.TriggerHighLimit))
			{
				// Not active.
				TimerActive = false;
			}
		}
		else
		{
			// Outer trigger.
			if(( TriggerValue < Setup.TriggerLowLimit) && ( TriggerValue > Setup.TriggerHighLimit))
			{
				// Not active.
				TimerActive = false;
			}
		}
	}

	int16_t NewUptime = GLOBAL.StatusTime.GetUptime();

	if( NewUptime != lastUptime)
	{
		if( TimerActive == true)
		{
			if( Setup.Reverse == true)
			{
				time -= NewUptime - lastUptime;
			}
			else
			{
				time += NewUptime - lastUptime;
			}

			if( Setup.Store == true)
			{
				Setup.CurrentTime = time;

				modified = true;
			}
		}

		lastUptime = NewUptime;
	}

	// Only warn when timer active.
	if( TimerActive == false)
	{
		return( SIGNAL_NEUTRAL_VALUE);
	}

	// Wait at least a second after last warn signal.
	if( lastWarnTime == time)
	{
		return( SIGNAL_NEUTRAL_VALUE);
	}

	// Give a special beep when the low level is reached.
	if( time == Setup.WarnLowTime)
	{
		GLOBAL.StatusService.Buzz( TIMER_WARN_BEEP_BORDER_LENGTH, TIMER_WARN_BEEP_BORDER_PAUSE,
								   TIMER_WARN_BEEP_BORDER_START_REPEAT);

		lastWarnTime = time;

		return( SIGNAL_NEUTRAL_VALUE);
	}

	// Give a special beep when the stop level is reached.
	if( time == Setup.WarnCriticalTime)
	{
		GLOBAL.StatusService.Buzz( TIMER_WARN_BEEP_BORDER_LENGTH, TIMER_WARN_BEEP_BORDER_PAUSE,
								   TIMER_WARN_BEEP_BORDER_STOP_REPEAT);
		
		lastWarnTime = time;
		
		return( SIGNAL_NEUTRAL_VALUE);
	}

	// No pause means no warn.
	if( Setup.WarnPauseTime == 0)
	{
		return( SIGNAL_NEUTRAL_VALUE);
	}

	uint16_t WarnTimeLevel;

	if( Setup.Reverse == false)
	{
		// For warnings inside the warn range, wait some seconds after last warn signal.
		if( time < ( lastWarnTime + Setup.WarnPauseTime))
		{
			return( SIGNAL_NEUTRAL_VALUE);
		}

		// Turn off alarm when out of range.
		if(( time >= Setup.WarnCriticalTime) || ( time < Setup.WarnLowTime))
		{
			return( SIGNAL_NEUTRAL_VALUE);
		}

		WarnTimeLevel = time - Setup.WarnLowTime;
	}
	else
	{
		// For warnings inside the warn range, wait some seconds after last warn signal.
		if( time > ( lastWarnTime - Setup.WarnPauseTime))
		{
			return( SIGNAL_NEUTRAL_VALUE);
		}

		// Turn off alarm when out of range.
		if(( time <= Setup.WarnCriticalTime) || ( time > Setup.WarnLowTime))
		{
			return( SIGNAL_NEUTRAL_VALUE);
		}

		WarnTimeLevel = Setup.WarnLowTime - time;
	}

	int16_t WarnTimeRange = Setup.WarnLowTime - Setup.WarnCriticalTime;

	// Make sure the range is positive.
	if( WarnTimeRange < 0)
	{
		WarnTimeRange = -WarnTimeRange;
	}

	uint32_t WarnRepeat = TIMER_WARN_REPEAT_RANGE;
	WarnRepeat *= WarnTimeLevel;
	WarnRepeat /= WarnTimeRange;
	WarnRepeat += TIMER_WARN_LOW_REPEAT;

	GLOBAL.StatusService.Buzz( TIMER_WARN_BEEP_LENGTH, TIMER_WARN_BEEP_PAUSE, WarnRepeat);

	lastWarnTime = time;

	return( SIGNAL_NEUTRAL_VALUE);
}

int16_t Signal_Source_Timer::GetTime( void)
{
	return( time);
}

void Signal_Source_Timer::ResetTime( void)
{
	time = Setup.InitTime;
	Setup.CurrentTime = time;

	modified = true;
}
