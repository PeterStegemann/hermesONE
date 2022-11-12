package net.stegemann.configuration;

import net.stegemann.configuration.type.Number;

public class Calibration
{
	public static final int MINIMUM_VALUE = 0;
	public static final int MAXIMUM_VALUE = 1023;

	private final Number high = new Number( MINIMUM_VALUE, MAXIMUM_VALUE);
	private final  Number center = new Number( MINIMUM_VALUE, MAXIMUM_VALUE);
	private final  Number low = new Number( MINIMUM_VALUE, MAXIMUM_VALUE);

	@Override
	public String toString()
	{
		StringBuffer Buffer = new StringBuffer();

		Buffer.append( "Calibration = {\n");
		Buffer.append( " High: " + high + "\n");
		Buffer.append( " Center: " + center + "\n");
		Buffer.append( " Low: " + low + "\n");
		Buffer.append( "}\n");

		return Buffer.toString();
	}

	public Number getHigh()
	{
		return high;
	}

	public Number getCenter()
	{
		return center;
	}

	public Number getLow()
	{
		return low;
	}
}
