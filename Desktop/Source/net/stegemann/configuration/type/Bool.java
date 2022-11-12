package net.stegemann.configuration.type;

public class Bool
{
	private boolean value;

	public Bool()
	{
		value = false;
	}

	public Bool( Bool other)
	{
		value = other.value;
	}

	@Override
	public String toString()
	{
		StringBuffer Buffer = new StringBuffer();

		Buffer.append( "Bool = {\n");
		Buffer.append(  " Value: " + Boolean.toString( value) + "\n");
		Buffer.append( "}\n");

		return Buffer.toString();
	}

	@Override
	public int hashCode()
	{
		return value ? 1231 : 1237;
	}

	public boolean equals( boolean other)
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

		if(( other instanceof Bool) == false)
		{
			return false;
		}

		return equals(( Bool) other);
	}

	public boolean equals( Bool other)
	{
		return value == other.value;
	}

	public void setValue( boolean value)
	{
		this.value = value;
	}

	public boolean getValue()
	{
		return value;
	}

	public void setConfigurationValue( String stringValue)
	{
		value = Boolean.parseBoolean( stringValue);
	}

	public String getConfigurationValue()
	{
		return Boolean.toString( value);
	}
}
