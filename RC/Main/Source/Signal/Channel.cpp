// Copyright 2008 Peter Stegemann

#include "Channel.h"

#include "Processor.h"
#include "Utility.h"
#include "Main/Global.h"
#include "Setup/Defines.h"

void Signal_Channel::Initialize( void)
{
	cachedValue = SIGNAL_NEUTRAL_VALUE;

	InputSignalSourceId = GLOBAL.SignalProcessor.GetSignalSourceId( Setup.InputSource.Source);
	TrimSignalSourceId = GLOBAL.SignalProcessor.GetSignalSourceId( Setup.TrimSource.Source);
	LimitSignalSourceId = GLOBAL.SignalProcessor.GetSignalSourceId( Setup.LimitSource.Source);
}

void Signal_Channel::Reset( void)
{
	InputSignalSourceId = SIGNAL_SOURCE_NONE;
	TrimSignalSourceId = SIGNAL_SOURCE_NONE;
	LimitSignalSourceId = SIGNAL_SOURCE_FIXED;

	Setup.InputSource.Source = SETUP_SOURCE_NONE;
	Setup.InputSource.Volume = SIGNAL_CHANNEL_INPUT_100_PERCENT_VALUE;
	Setup.TrimSource.Source = SETUP_SOURCE_NONE;
	Setup.TrimSource.Volume = SIGNAL_CHANNEL_INPUT_100_PERCENT_VALUE / 4;
	Setup.LimitSource.Source = SETUP_SOURCE_FIXED;
	Setup.LimitSource.Volume = SIGNAL_CHANNEL_INPUT_100_PERCENT_VALUE;

	Setup.Reverse = false;
	Setup.Mode = Setup_Channel::M_Warp;

	Setup.PointVolume[ Setup_Channel::PV_Low] = - SIGNAL_CHANNEL_100_PERCENT_VALUE;
	Setup.PointVolume[ Setup_Channel::PV_Center] = 0;
	Setup.PointVolume[ Setup_Channel::PV_High] = SIGNAL_CHANNEL_100_PERCENT_VALUE;
}

int16_t Signal_Channel::CalculateValue( Signal_Processor* SignalProcessor)
{
	int16_t SourceValue =
		Signal_Utility::GetVolumizedSourceValue( SignalProcessor, InputSignalSourceId,
												 SIGNAL_CHANNEL_INPUT_100_PERCENT_VALUE,
												 Setup.InputSource.Volume);

	int16_t TrimValue =
		Signal_Utility::GetVolumizedSourceValue( SignalProcessor, TrimSignalSourceId,
												 SIGNAL_CHANNEL_INPUT_100_PERCENT_VALUE,
												 Setup.TrimSource.Volume);

	int16_t LimitValue =
		Signal_Utility::GetVolumizedSourceValue( SignalProcessor, LimitSignalSourceId,
												 SIGNAL_CHANNEL_INPUT_100_PERCENT_VALUE,
												 Setup.LimitSource.Volume);

	int16_t Result;

	if( Setup.Mode == Setup_Channel::M_Clip)
	{
		int16_t Lower, Higher;

		Lower = Signal_Utility::VolumizeValue( Setup.PointVolume[ Setup_Channel::PV_Low],
											   SIGNAL_CHANNEL_100_PERCENT_VALUE,
											   LimitValue);

		Higher = Signal_Utility::VolumizeValue( Setup.PointVolume[ Setup_Channel::PV_High],
											    SIGNAL_CHANNEL_100_PERCENT_VALUE,
											    LimitValue);

		if( Lower > Higher)
		{
			int16_t Temp = Lower;
			Lower = Higher;
			Higher = Temp;
		}

		Result = Signal_Utility::AddValues( Setup.PointVolume[ Setup_Channel::PV_Center],
										    SourceValue);
		Result = Signal_Utility::AddValues( TrimValue, Result);

		if( Result < Lower)
		{
			Result = Lower;
		}
		else if( Result > Higher)
		{
			Result = Higher;
		}
	}
	else
	{
		if( SourceValue == SIGNAL_MAXIMUM_VALUE)
		{
			Result = Signal_Utility::VolumizeValue( Setup.PointVolume[ Setup_Channel::PV_High],
												    SIGNAL_CHANNEL_100_PERCENT_VALUE, LimitValue);
		}
		else
		{
			// Move value into positive realm.
			uint16_t UnsignedValue = ( int32_t) SourceValue - ( int32_t) SIGNAL_MINIMUM_VALUE;

			// Calculate width of one map column.
			uint16_t Width = (( uint16_t) SIGNAL_VALUE_RANGE) / 2;

			// Calculate in which column this value is.
			int32_t Lower, Higher;

			if( SourceValue < 0)
			{
				Lower = Signal_Utility::VolumizeValue( Setup.PointVolume[ Setup_Channel::PV_Low],
													   SIGNAL_CHANNEL_100_PERCENT_VALUE,
													   LimitValue);

				Higher = Signal_Utility::AddValues( Setup.PointVolume[ Setup_Channel::PV_Center],
												    TrimValue);
			}
			else
			{
				Lower = Signal_Utility::AddValues( Setup.PointVolume[ Setup_Channel::PV_Center],
												   TrimValue);

				Higher = Signal_Utility::VolumizeValue( Setup.PointVolume[ Setup_Channel::PV_High],
													    SIGNAL_CHANNEL_100_PERCENT_VALUE,
													    LimitValue);
			}

			// Now calculate where in the column it is.
			int32_t Where = UnsignedValue % Width;

			int32_t Rise = Higher - Lower;
			int32_t Full = Where * Rise;
			Full /= Width;

			Result = Lower + Full;
		}
	}

	// Apply reverse.
	if( Setup.Reverse == true)
	{
		Result = -Result;
	}

	cachedValue = Result;

	return( Result);
}

int16_t Signal_Channel::GetValue( void)
{
	return cachedValue;
}
