package net.stegemann.configuration.type;

import net.stegemann.configuration.Signal;

public class Volume extends Number
{
    private final int signalPerValue;

    public Volume( int signalPerValue)
    {
        this( Signal.MINIMUM_VALUE, Signal.MAXIMUM_VALUE, signalPerValue);
    }

    public Volume( int signalPerValue, int value)
        throws ValueOutOfRangeException
    {
        super( Signal.MINIMUM_VALUE / signalPerValue, Signal.MAXIMUM_VALUE / signalPerValue, value);

        this.signalPerValue = signalPerValue;
    }

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
    public void setConfigurationValue( String stringValue)
        throws ValueOutOfRangeException, NumberFormatException
    {
        setValue( Integer.parseInt( stringValue) / signalPerValue);
    }

    @Override
    public String getConfigurationValue()
    {
        return Integer.toString( getValue() * signalPerValue);
    }

    @Override
    public String toString()
    {
        return String.format
        (
            """
            Volume
            {
                %s SignalPerValue: %s
            }
            """,
            super.toString(), signalPerValue
        );
    }
}
