package net.stegemann.configuration.type;

import lombok.Getter;
import net.stegemann.misc.ChangeObservable;

@Getter
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
        return String.format( """
            Text
            {
                value: %s
            }""",
            value
        );
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
