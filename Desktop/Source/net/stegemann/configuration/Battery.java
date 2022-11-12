package net.stegemann.configuration;

import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.ValueOutOfRangeException;

public class Battery
{
	public static final int VOLTAGE_MINIMUM = 0;
	public static final int VOLTAGE_MAXIMUM = 120;
	public static final int CALIBRATION_VOLTAGE_MINIMUM = 0;
	public static final int CALIBRATION_VOLTAGE_MAXIMUM = 180;

	private final Number warnLowVoltage = new Number( VOLTAGE_MINIMUM, VOLTAGE_MAXIMUM);
	private final Number warnCriticalVoltage = new Number( VOLTAGE_MINIMUM, VOLTAGE_MAXIMUM);
	private final Number minimumVoltage = new Number( VOLTAGE_MINIMUM, VOLTAGE_MAXIMUM);
	private final Number maximumVoltage = new Number( VOLTAGE_MINIMUM, VOLTAGE_MAXIMUM);
	private final Number calibrationVoltage = new Number( CALIBRATION_VOLTAGE_MINIMUM,
														  CALIBRATION_VOLTAGE_MAXIMUM);

	@Override
	public String toString()
	{
		StringBuffer Buffer = new StringBuffer();

		Buffer.append( "Battery = {\n");
		Buffer.append( " WarnLowVoltage: " + warnLowVoltage + "\n");
		Buffer.append( " WarnCriticalVoltage: " + warnCriticalVoltage + "\n");
		Buffer.append( " MinimumVoltage: " + minimumVoltage + "\n");
		Buffer.append( " MaximumVoltage: " + maximumVoltage + "\n");
		Buffer.append( " CalibrationVoltage: " + calibrationVoltage + "\n");
		Buffer.append( "}\n");

		return Buffer.toString();
	}

	public void clear()
	{
		try
		{ 
			warnLowVoltage.setValue( VOLTAGE_MAXIMUM / 2);
			warnCriticalVoltage.setValue( VOLTAGE_MAXIMUM / 2);
			minimumVoltage.setValue( VOLTAGE_MAXIMUM / 2);
			maximumVoltage.setValue( VOLTAGE_MAXIMUM / 2);
			calibrationVoltage.setValue( VOLTAGE_MAXIMUM / 2);
		}
		catch( ValueOutOfRangeException reason)
		{
			throw new RuntimeException( reason);
		}
	}

	public Number getWarnLowVoltage()
	{
		return warnLowVoltage;
	}

	public Number getWarnCriticalVoltage()
	{
		return warnCriticalVoltage;
	}

	public Number getMinimumVoltage()
	{
		return minimumVoltage;
	}

	public Number getMaximumVoltage()
	{
		return maximumVoltage;
	}

	public Number getCalibrationVoltage()
	{
		return calibrationVoltage;
	}
}
