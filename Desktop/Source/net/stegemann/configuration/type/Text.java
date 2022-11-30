package net.stegemann.configuration.type;

import net.stegemann.misc.ChangeObservable;

public class Text extends ChangeObservable< Text> implements ConfigurationValue
{
	private String value;

	public Text()
	{
		value = "";
	}

	public Text( Text other)
	{
		value = other.value;
	}

	public Text( String value)
	{
		this.value = value;
	}

	@Override
	public String toString()
	{
		StringBuffer Buffer = new StringBuffer();

		Buffer.append( "Text = {\n");
		Buffer.append(  " Value: " + value + "\n");
		Buffer.append( "}\n");

		return Buffer.toString();
	}

	@Override
	public int hashCode()
	{
		return value.hashCode();
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

		if(( other instanceof Text) == false)
		{
			return false;
		}

		return equals(( Text) other);
	}

	public boolean equals( Text other)
	{
		if( value == null)
		{
			if( other.value != null)
			{
				return false;
			}
		}
		else
		{
			if( value.equals( other.value) == false)
			{
				return false;
			}
		}

		return true;
	}

	public void setValue( String Value)
	{
		value = Value;

		notifyChange( this);
	}

	public String getValue()
	{
		return value;
	}

	@Override
	public void setConfigurationValue( String value)
	{
		setValue( value);
	}

	@Override
	public String getConfigurationValue()
	{
		return value;
	}
}
