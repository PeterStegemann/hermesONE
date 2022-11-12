package net.stegemann.configuration.type;

import net.stegemann.misc.ChangeObservable;

public class Number extends ChangeObservable< Number>
{
	private int minimum = Integer.MIN_VALUE;
	private int maximum = Integer.MAX_VALUE;

	private int value = 0;

	public Number()
	{
	}

	public Number( int value)
	{
		try
		{
			setValue( value);
		}
		catch( ValueOutOfRangeException reason)
		{
			// No exception possible here.
			throw new RuntimeException( reason);
		}
	}

	public Number( int minimum, int maximum)
	{
		this.minimum = minimum;
		this.maximum = maximum;
	}

	public Number( int minimum, int maximum, int value) 
			throws ValueOutOfRangeException
	{
		this.minimum = minimum;
		this.maximum = maximum;

		setValue( value);
	}

	public Number( Number other)
	{
		minimum = other.minimum;
		maximum = other.maximum;

		value = other.value;
	}

	@Override
	public String toString()
	{
		StringBuffer Buffer = new StringBuffer();

		Buffer.append( "Number = {\n");
		Buffer.append(  " Minimum: " + Integer.toString( minimum) + "\n");
		Buffer.append(  " Maximum: " + Integer.toString( maximum) + "\n");
		Buffer.append(  " Value: " + Integer.toString( value) + "\n");
		Buffer.append( "}\n");

		return Buffer.toString();
	}

	@Override
	public int hashCode()
	{
		return value;
	}

	public boolean equals( int other)
	{
		return value == other;
	}

	@Override
	public boolean equals( Object other)
	{
		if( this == other)
		{
			return true;
		}

		if( other == null)
		{
			return false;
		}

		if(( other instanceof Number) == false)
		{
			return false;
		}

		return equals(( Number) other);
	}

	public boolean equals( Number other)
	{
		return value == other.value;
	}

	public void setValue( Number value) throws ValueOutOfRangeException
	{
		if( value != null)
		{
			setValue( value.getValue());
		}
	}

	public void setValue( int Value) throws ValueOutOfRangeException
	{
		if(( Value > maximum) || ( Value < minimum))
		{
			throw new ValueOutOfRangeException( "Value " + Value + " not in range (" + minimum +
														   " < " + maximum + ")");
		}

		value = Value;

		notifyChange( this);
	}

	public int getValue()
	{
		return value;
	}

	public void setConfigurationValue( String stringValue)
			throws ValueOutOfRangeException, NumberFormatException
	{
		setValue( Integer.parseInt( stringValue));
	}

	public String getConfigurationValue()
	{
		return Integer.toString( value);
	}
}
