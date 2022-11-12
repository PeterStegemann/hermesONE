package net.stegemann.configuration.type;

public class Volume extends Number
{
	private int signalPerValue;

	public Volume( int minimum, int maximum, int signalPerValue)
	{
		super( minimum / signalPerValue, maximum / signalPerValue);

		this.signalPerValue = signalPerValue;
	}

	public Volume( int minimum, int maximum, int signalPerValue, int value)
		throws ValueOutOfRangeException
	{
		super( minimum / signalPerValue, maximum / signalPerValue, value);

		this.signalPerValue = signalPerValue;
	}

	public Volume( Volume other)
	{
		super( other);

		signalPerValue = other.signalPerValue;
	}

	@Override
	public String toString()
	{
		StringBuffer Buffer = new StringBuffer();

		Buffer.append( "Volume = {\n");
		Buffer.append( super.toString());
		Buffer.append(  " SignalPerValue: " + signalPerValue + "\n");
		Buffer.append( "}\n");

		return Buffer.toString();
	}

	@Override
	public int hashCode()
	{
		final int prime = 31;

		int result = super.hashCode();
		result = prime * result + signalPerValue;

		return result;
	}

	@Override
	public boolean equals( Object other)
	{
		if( this == other)
		{
			return true;
		}

		if(( other instanceof Volume) == false)
		{
			return false;
		}

		return equals(( Volume) other);
	}

	boolean equals( Volume other)
	{
		if( super.equals( other) == false)
		{
			return false;
		}

		if( signalPerValue != other.signalPerValue)
		{
			return false;
		}

		return true;
	}

	@Override
	public void setConfigurationValue( String StringValue) throws ValueOutOfRangeException,
																  NumberFormatException
	{
		setValue( Integer.parseInt( StringValue) / signalPerValue);
	}

	@Override
	public String getConfigurationValue()
	{
		return Integer.toString( getValue() * signalPerValue);
	}
}
