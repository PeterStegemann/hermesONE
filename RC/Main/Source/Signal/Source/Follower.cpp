// Copyright 2010 Peter Stegemann

#include "Follower.h"

#include "../Processor.h"
#include "../Utility.h"
#include "Setup/Defines.h"

#include "AVR/Source/Utility.h"

void Signal_Source_Follower::Initialize( void)
{
	value = SIGNAL_MINIMUM_VALUE;

	TargetSignalSourceId = GLOBAL.SignalProcessor.GetSignalSourceId( Setup.TargetSource.Source);
	StepSignalSourceId = GLOBAL.SignalProcessor.GetSignalSourceId( Setup.StepSource.Source);
	TriggerSignalSourceId = GLOBAL.SignalProcessor.GetSignalSourceId( Setup.TriggerSource);
}

void Signal_Source_Follower::Reset( void)
{
	value = SIGNAL_MINIMUM_VALUE;

	TargetSignalSourceId = SIGNAL_SOURCE_NONE;
	StepSignalSourceId = SIGNAL_SOURCE_NONE;
	TriggerSignalSourceId = SIGNAL_SOURCE_NONE;

	Setup.TargetSource.Source = SETUP_SOURCE_NONE;
	Setup.TargetSource.Volume = SIGNAL_SOURCE_FOLLOWER_TARGET_100_PERCENT_VALUE;
	Setup.StepSource.Source = SETUP_SOURCE_NONE;
	Setup.StepSource.Volume = SIGNAL_SOURCE_FOLLOWER_STEP_100_PERCENT_VALUE;
	Setup.TriggerSource = SETUP_SOURCE_NONE;
	Setup.TriggerLowLimit = SIGNAL_MINIMUM_VALUE;
	Setup.TriggerHighLimit = SIGNAL_MAXIMUM_VALUE;
}

void Signal_Source_Follower::LoadSetup( uint16_t SetupSourceId)
{
	GLOBAL.SetupService.GetSourceFollower( SetupSourceId, &Setup);
}

int16_t Signal_Source_Follower::CalculateValue( Signal_Processor* SignalProcessor)
{
	int16_t TargetValue = Signal_Utility::GetVolumizedSourceValue(
		SignalProcessor, TargetSignalSourceId, SIGNAL_SOURCE_FOLLOWER_TARGET_100_PERCENT_VALUE,
		Setup.TargetSource.Volume);

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
				return( value);
			}
		}
		else
		{
			// Outer trigger.
			if(( TriggerValue < Setup.TriggerLowLimit) && ( TriggerValue > Setup.TriggerHighLimit))
			{
				// Not active.
				return( value);
			}
		}
	}

	// If no valid source is set, step source defaults to 100%, meaning you can set a fixed value
	// with the volume.
	int32_t StepValue = Signal_Utility::GetVolumizedSourceValue(
		SignalProcessor, StepSignalSourceId, SIGNAL_SOURCE_FOLLOWER_STEP_100_PERCENT_VALUE,
		Setup.StepSource.Volume);

	StepValue = UTILITY_Absolute( StepValue);
	StepValue /= SIGNAL_SOURCE_FOLLOWER_STEP_SIGNAL_PER_VALUE;

	StepValue = StepValue * StepValue * StepValue;
	StepValue /= 100;
	StepValue *= 3;

	int32_t Difference = ( int32_t) TargetValue - ( int32_t) value;

	if( UTILITY_Absolute( Difference) < StepValue)
	{
		value = TargetValue;
	}
	else if( Difference > 0)
	{
		// Up.
		value += StepValue;
	}
	else if( Difference < 0)
	{
		// Down.
		value -= StepValue;
	}

	return( value);
}
