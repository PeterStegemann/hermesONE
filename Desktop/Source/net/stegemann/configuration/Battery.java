package net.stegemann.configuration;

import lombok.Getter;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.ValueOutOfRangeException;
import net.stegemann.configuration.util.ConfigurationField;
import net.stegemann.io.xml.Names;

@Getter
public class Battery
{
	public static final int VOLTAGE_MINIMUM = 0;
	public static final int VOLTAGE_MAXIMUM = 120;
	public static final int CALIBRATION_VOLTAGE_MINIMUM = 0;
	public static final int CALIBRATION_VOLTAGE_MAXIMUM = 180;

	@ConfigurationField( name = Names.BATTERY_WARN_LOW_VOLTAGE)
	private final Number warnLowVoltage = new Number( VOLTAGE_MINIMUM, VOLTAGE_MAXIMUM);
	@ConfigurationField( name = Names.BATTERY_WARN_CRITICAL_VOLTAGE)
	private final Number warnCriticalVoltage = new Number( VOLTAGE_MINIMUM, VOLTAGE_MAXIMUM);
	@ConfigurationField( name = Names.BATTERY_MINIMUM_VOLTAGE)
	private final Number minimumVoltage = new Number( VOLTAGE_MINIMUM, VOLTAGE_MAXIMUM);
	@ConfigurationField( name = Names.BATTERY_MAXIMUM_VOLTAGE)
	private final Number maximumVoltage = new Number( VOLTAGE_MINIMUM, VOLTAGE_MAXIMUM);
	@ConfigurationField( name = Names.BATTERY_CALIBRATION_VOLTAGE)
	private final Number calibrationVoltage = new Number( CALIBRATION_VOLTAGE_MINIMUM, CALIBRATION_VOLTAGE_MAXIMUM);

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
}
