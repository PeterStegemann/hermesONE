// Copyright 2008 Peter Stegemann

#include "Map.h"

#include "Main/Global.h"
#include "Setup/Defines.h"
#include "Signal/Processor.h"
#include "Signal/Utility.h"

int16_t Signal_Source_Map::getPoint( uint8_t PointId, Signal_Processor* SignalProcessor)
{
	uint8_t SignalSourceId = PointSignalSourceId[ PointId];
	int16_t Volume = Setup.PointSource[ PointId].Volume;
/*
	if( SignalSourceId == SIGNAL_SOURCE_NONE)
	{
		// No source, use neutral value.
		return( SIGNAL_NEUTRAL_VALUE);
	}
	else if( SignalSourceId == SIGNAL_SOURCE_FIXED)
	{
		int32_t ResultValue = Volume;
		ResultValue *= SIGNAL_MAXIMUM_VALUE;
		ResultValue /= SIGNAL_SOURCE_MAP_100_PERCENT_VALUE;

		return( ResultValue);
	}
	else*/
	{
		return( Signal_Utility::GetVolumizedSourceValue( SignalProcessor, SignalSourceId,
														 SIGNAL_SOURCE_MAP_100_PERCENT_VALUE,
														 Volume));
	}
}

void Signal_Source_Map::updatePoints( void)
{
	Points = 0;

	// Recalculate number of points.
	for( uint8_t PointId = 0; PointId < SETUP_SOURCE_MAP_POINTS; PointId++)
	{
		if( Setup.PointSource[ PointId].Source != SETUP_SOURCE_NONE)
		{
			Points++;
		}
	}
}

void Signal_Source_Map::Initialize( void)
{
	InputSignalSourceId = GLOBAL.SignalProcessor.GetSignalSourceId( Setup.InputSource.Source);
	
	for( uint8_t Index = 0; Index < SETUP_SOURCE_MAP_POINTS; Index++)
	{
		PointSignalSourceId[ Index] =
			GLOBAL.SignalProcessor.GetSignalSourceId( Setup.PointSource[ Index].Source);
	}

	updatePoints();
}

void Signal_Source_Map::Reset( void)
{
	InputSignalSourceId = SIGNAL_SOURCE_NONE;
	Setup.InputSource.Source = SETUP_SOURCE_NONE;
	Setup.InputSource.Volume = SIGNAL_SOURCE_MAP_INPUT_100_PERCENT_VALUE;

	Points = 3;

	for( uint8_t Index = 0; Index < SETUP_SOURCE_MAP_POINTS; Index++)
	{
		PointSignalSourceId[ Index] = SIGNAL_SOURCE_NONE;
		Setup.PointSource[ Index].Source = SETUP_SOURCE_NONE;
		Setup.PointSource[ Index].Volume = SIGNAL_SOURCE_MAP_INPUT_100_PERCENT_VALUE;
	}

	PointSignalSourceId[ 0] = SIGNAL_SOURCE_FIXED;
	Setup.PointSource[ 0].Source = SETUP_SOURCE_FIXED;
	Setup.PointSource[ 0].Volume = SIGNAL_MINIMUM_VALUE;
	PointSignalSourceId[ 3] = SIGNAL_SOURCE_FIXED;
	Setup.PointSource[ 3].Source = SETUP_SOURCE_FIXED;
	Setup.PointSource[ 3].Volume = SIGNAL_NEUTRAL_VALUE;
	PointSignalSourceId[ 6] = SIGNAL_SOURCE_FIXED;
	Setup.PointSource[ 6].Source = SETUP_SOURCE_FIXED;
	Setup.PointSource[ 6].Volume = SIGNAL_MAXIMUM_VALUE;
}

void Signal_Source_Map::LoadSetup( uint16_t SetupSourceId)
{
	GLOBAL.SetupService.GetSourceMap( SetupSourceId, &Setup);

	updatePoints();
}

int16_t Signal_Source_Map::CalculateValue( Signal_Processor* SignalProcessor)
{
	updatePoints();

	int16_t Value =
		Signal_Utility::GetVolumizedSourceValue( SignalProcessor, InputSignalSourceId,
												 SIGNAL_SOURCE_MAP_INPUT_100_PERCENT_VALUE,
												 Setup.InputSource.Volume);

	// No point? Pass trough.
	if(( Points == 0) || ( Points > SETUP_SOURCE_MAP_POINTS))
	{
		return( Value);
	}

	// One point? Use that one.
	if( Points == 1)
	{
		for( uint8_t PointId = 0; PointId < SETUP_SOURCE_MAP_POINTS; PointId++)
		{
			if( PointSignalSourceId[ PointId] != SIGNAL_SOURCE_NONE)
			{
				return( getPoint( PointId, SignalProcessor));
			}
		}

		// Pass trough.
		return( Value);
	}

	// Multiple points...

	// Move value into positive realm.
	uint16_t UnsignedValue = ( int32_t) Value - ( int32_t) SIGNAL_MINIMUM_VALUE;

	// Calculate width of one map column.
	uint16_t Width = ( uint16_t) SIGNAL_VALUE_RANGE / ( Points - 1);

	// Now calculate in which column this value is and where in the column it is.
	uint8_t Column = UnsignedValue / Width;
	uint16_t Where = UnsignedValue % Width;

	// It's between these two values:
	uint8_t PointId = 0;

	// Get lower value first.
	int16_t Lower = SIGNAL_NEUTRAL_VALUE;

	while( PointId < SETUP_SOURCE_MAP_POINTS)
	{
		// Valid value?
		if( PointSignalSourceId[ PointId] == SIGNAL_SOURCE_NONE)
		{
			// No, try next.
			PointId++;

			continue;
		}

		Lower = getPoint( PointId++, SignalProcessor);

		// Column found?
		if( Column == 0)
		{
			// Yes.
			break;
		}

		// No, try next one.
		Column--;
	}

	// Get higher value.
	int16_t Higher = SIGNAL_NEUTRAL_VALUE;

	while( PointId < SETUP_SOURCE_MAP_POINTS)
	{
		// Valid value?
		if( PointSignalSourceId[ PointId] == SIGNAL_SOURCE_NONE)
		{
			// No, try next.
			PointId++;

			continue;
		}
		else
		{
			// Yes.
			Higher = getPoint( PointId, SignalProcessor);

			break;
		}
	}

	// Get how steep this column is. Remind to divide this by the width of the column later.
	int32_t Rise = ( int32_t) Higher - ( int32_t) Lower;

	if( Points == 2)
	{
		if( Where > SIGNAL_MAXIMUM_VALUE)
		{
			Where = Value;

			Lower += Rise / 2;
		}
	}

	int32_t Full = (( int32_t) Where) * Rise;

//	GLOBAL.Lcd.PrintFormat_P( 0, 40, ( avr::font::FontId) 1, LCD::Red, LCD::Black,
//						  LCD::PO_Fixed, "%u %u %ld       ", Width, Where, Full);

	Full /= Width;

	int16_t Result = ( int32_t) Lower + Full;

//	GLOBAL.Lcd.PrintFormat_P( 0, 50, ( avr::font::FontId) 1, LCD::Red, LCD::Black,
//						  LCD::PO_Fixed, "%ld %ld %d       ", Rise, Full, Result);

	return( Result);
}
