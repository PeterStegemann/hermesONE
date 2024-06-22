package net.stegemann.configuration.type;

import lombok.Getter;
import net.stegemann.misc.ChangeObservable;

public class Number extends ChangeObservable< Number>
                 implements Comparable< Number>, ConfigurationValue
{
    private int minimum = Integer.MIN_VALUE;
    private int maximum = Integer.MAX_VALUE;

    @Getter
    private int value = 0;

    public Number() {}

    public Number( int value)
    {
        this.value = value;
    }

    public Number( int minimum, int maximum)
    {
        this.minimum = minimum;
        this.maximum = maximum;

        this.value = minimum;
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

    public void setValue( Number value)
        throws ValueOutOfRangeException
    {
        if( value != null)
        {
            setValue( value.getValue());
        }
    }

    public void setValue( int Value)
        throws ValueOutOfRangeException
    {
        if(( Value > maximum) || ( Value < minimum))
        {
            throw new ValueOutOfRangeException( "Value " + Value + " not in range (" + minimum + " < " + maximum + ")");
        }

        value = Value;

        notifyChange( this);
    }

    @Override
    public void setStringValue( String stringValue)
        throws ValueOutOfRangeException, NumberFormatException
    {
        int intValue;

        if( stringValue == null)
        {
            intValue = 0;
        }
        else
        {
            intValue = Integer.parseInt( stringValue);
        }

        setValue( intValue);
    }

    @Override
    public String getStringValue()
    {
        return Integer.toString( value);
    }

    @Override
    public int compareTo( Number other)
    {
        if( other == null)
        {
            return 1;
        }

        return value - other.value;
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

    @Override
    public String toString()
    {
        return String.format
        (
            """
            Number
            {
                minimum: %s
                maximum: %s
                value: %s
            }
            """,
            minimum, maximum, value
        );
    }
}
