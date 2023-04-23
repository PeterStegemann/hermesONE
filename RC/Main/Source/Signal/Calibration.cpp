// Copyright 2008 Peter Stegemann

#include "Calibration.h"

#include "Defines.h"

Signal_Calibration::Signal_Calibration( void)
{
	Setup.Value[ Setup_Calibration::V_High] = 0;
	Setup.Value[ Setup_Calibration::V_Center] = 0;
	Setup.Value[ Setup_Calibration::V_Low] = 0;
}

int16_t Signal_Calibration::Calibrate( int16_t Value)
{
	if(( Setup.Value[ Setup_Calibration::V_Center] == Setup.Value[ Setup_Calibration::V_Low]) ||
	   ( Setup.Value[ Setup_Calibration::V_Center] == Setup.Value[ Setup_Calibration::V_High]))
	{
		// Illegal setup, go pass through.
		return( Value);
	}

	int16_t ValueRange;
	int16_t ValueCenter = 0;

	if( Setup.Value[ Setup_Calibration::V_Low] < Setup.Value[ Setup_Calibration::V_High])
	{
		if( Value <= Setup.Value[ Setup_Calibration::V_Center])
		{
			if( Value <= Setup.Value[ Setup_Calibration::V_Low])
			{
				Value = Setup.Value[ Setup_Calibration::V_Low];
			}

			Value -= Setup.Value[ Setup_Calibration::V_Low];
			
			ValueRange = Setup.Value[ Setup_Calibration::V_Center] -
						 Setup.Value[ Setup_Calibration::V_Low];
			ValueCenter = SIGNAL_MINIMUM_VALUE;
		}
		else
		{
			if( Value >= Setup.Value[ Setup_Calibration::V_High])
			{
				Value = Setup.Value[ Setup_Calibration::V_High];
			}
			
			Value -= Setup.Value[ Setup_Calibration::V_Center];
			
			ValueRange = Setup.Value[ Setup_Calibration::V_High] -
						 Setup.Value[ Setup_Calibration::V_Center];
		}
	}
	else
	{
		if( Value <= Setup.Value[ Setup_Calibration::V_Center])
		{
			if( Value <= Setup.Value[ Setup_Calibration::V_High])
			{
				Value = Setup.Value[ Setup_Calibration::V_High];
			}
			
			Value -= Setup.Value[ Setup_Calibration::V_High];
			
			ValueRange = -( Setup.Value[ Setup_Calibration::V_Center] -
						    Setup.Value[ Setup_Calibration::V_High]);
			ValueCenter = SIGNAL_MAXIMUM_VALUE;
		}
		else
		{
			if( Value >= Setup.Value[ Setup_Calibration::V_Low])
			{
				Value = Setup.Value[ Setup_Calibration::V_Low];
			}
			
			Value -= Setup.Value[ Setup_Calibration::V_Center];
			
			ValueRange = -( Setup.Value[ Setup_Calibration::V_Low] -
						    Setup.Value[ Setup_Calibration::V_Center]);
		}
	}

	int32_t CalibratedValue = Value;
	CalibratedValue *= SIGNAL_VALUE_RANGE / 2;
	CalibratedValue /= ValueRange;
	CalibratedValue += ValueCenter;

	return( CalibratedValue);
}
