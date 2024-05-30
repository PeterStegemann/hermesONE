// Copyright 2011 Peter Stegemann

#pragma once

#include "Setup/Battery.h"
#include "Signal/Processor.h"
#include "Status/Service.h"

#include "AVR/Source/Types.h"

#define BATTERY_WARN_CHECK_DELAY		10

#define BATTERY_WARN_LOW_PAUSE			50
#define BATTERY_WARN_CRITICAL_PAUSE		5
#define BATTERY_WARN_PAUSE_RANGE		( BATTERY_WARN_LOW_PAUSE - BATTERY_WARN_CRITICAL_PAUSE)

#define BATTERY_WARN_BEEP_LENGTH		50
#define BATTERY_WARN_BEEP_PAUSE			50
#define BATTERY_WARN_BEEP_REPEAT		2

class Status_Battery
{
  private:
    Signal_Processor* signalProcessor;
    Status_Service* statusService;

    Setup_Battery batterySetup;

    uint8_t batteryVoltage;

    uint16_t warnCount;

    void calculateVoltage( void)
    {
        uint32_t Voltage = GetRawVoltage();

        Voltage *= batterySetup.CalibrationValue;
        Voltage /= 1023;

        Voltage = Voltage + batteryVoltage * 9;
        Voltage /= 10;

        batteryVoltage = Voltage;
    }

  public:
    Status_Battery
    (
        Signal_Processor* SignalProcessor,
        Setup_Service* SetupService,
        Status_Service* StatusService
    )
        : signalProcessor( SignalProcessor)
        , statusService( StatusService)
        // Initialize this with a pause to avoid false alarms on startup.
        , warnCount( BATTERY_WARN_LOW_PAUSE)
    {
    	SetupService->GetBattery( &batterySetup);

    	// Start with a reasonable value.
    	batteryVoltage = batterySetup.WarnLowVoltage;
    }

    // This is for the interrupt, not for you.
    void Process( void)
    {
        calculateVoltage();

        // Count down warning pause.
        if( warnCount != 0)
        {
            warnCount--;

            return;
        }

        // Check for current warning level.
        uint8_t Voltage = GetVoltage();

        if( Voltage > batterySetup.WarnLowVoltage)
        {
            // Check again in a second, no warning needed now.
            warnCount = BATTERY_WARN_CHECK_DELAY;

            return;
        }

        uint8_t WarnVoltageLevel;

        if( Voltage <= batterySetup.WarnCriticalVoltage)
        {
            // Full alarm.
            WarnVoltageLevel = 0;
        }
        else
        {
            WarnVoltageLevel = Voltage - batterySetup.WarnCriticalVoltage;
        }

        uint8_t WarnVoltageRange = batterySetup.WarnLowVoltage - batterySetup.WarnCriticalVoltage;

        uint32_t WarnPause = BATTERY_WARN_PAUSE_RANGE;
        WarnPause *= WarnVoltageLevel;
        WarnPause /= WarnVoltageRange;
        WarnPause += BATTERY_WARN_CRITICAL_PAUSE;

        warnCount = WarnPause;

        statusService->Buzz( BATTERY_WARN_BEEP_LENGTH, BATTERY_WARN_BEEP_PAUSE, BATTERY_WARN_BEEP_REPEAT);
    }

    Setup_Battery* GetBatterySetup( void)
    {
    	return( &batterySetup);
    }

    void UpdateCalibrationValue( uint8_t ExpectedVoltage)
    {
        // RawVoltage == Value
        uint16_t RawVoltage = GetRawVoltage();
        uint32_t NewCalibrationValue = ExpectedVoltage;
        NewCalibrationValue *= 1023;
        NewCalibrationValue /= RawVoltage;
        batterySetup.CalibrationValue = NewCalibrationValue;
    }

    // Raw voltage value.
    uint16_t GetRawVoltage( void)
    {
    	return( signalProcessor->GetRawAnalogValue( INPUT_ANALOG_PORT_VOLTAGE));
    }

    // The current, calibrated voltage value.
    uint8_t GetVoltage( void)
    {
	    return( batteryVoltage);
    }
};
