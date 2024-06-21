package net.stegemann.configuration.type;

import lombok.Getter;
import net.stegemann.configuration.Signal;
import net.stegemann.configuration.util.ConfigurationField;
import net.stegemann.io.xml.Names;

import java.util.HashMap;

@Getter
public class SourceWithVolume
{
    @ConfigurationField( name = Names.SOURCE_TUPLE_SOURCE_ID)
    private final SourceId sourceId;
    @ConfigurationField( name = Names.SOURCE_TUPLE_VOLUME)
    private final Volume volume;

    public SourceWithVolume( int signalPerValue)
    {
        this( Signal.MINIMUM_VALUE, Signal.MAXIMUM_VALUE, signalPerValue);
    }

    public SourceWithVolume( int signalPerValue, int value)
        throws ValueOutOfRangeException
    {
        this( Signal.MINIMUM_VALUE, Signal.MAXIMUM_VALUE, signalPerValue, value);
    }

    public SourceWithVolume( int minimum, int maximum, int signalPerValue)
    {
        sourceId = new SourceId();
        volume = new Volume( minimum, maximum, signalPerValue);
    }

    public SourceWithVolume( int minimum, int maximum, int signalPerValue, int value)
        throws ValueOutOfRangeException
    {
        sourceId = new SourceId();
        volume = new Volume( minimum, maximum, signalPerValue, value);
    }

    public SourceWithVolume( SourceWithVolume other)
    {
        sourceId = new SourceId( other.sourceId);
        volume = new Volume( other.volume);
    }

    @Override
    public int hashCode()
    {
        final int prime = 31;

        int result = 1;

        result = prime * result + sourceId.hashCode();
        result = prime * result + volume.hashCode();

        return result;
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

        if(( other instanceof SourceWithVolume) == false)
        {
            return false;
        }

        return equals(( SourceWithVolume) other);
    }

    public boolean equals( SourceWithVolume other)
    {
        if( sourceId.equals( other.sourceId) == false)
        {
            return false;
        }

        if( volume.equals( other.volume) == false)
        {
            return false;
        }

        return true;
    }

    public void replaceSource( HashMap< SourceId, SourceId> sourcesMap)
    {
        sourceId.replaceSource( sourcesMap);
    }

    public void switchSource( SourceId sourceIdOne, SourceId sourceIdTwo)
    {
        sourceId.switchSource( sourceIdOne, sourceIdTwo);
    }

    @Override
    public String toString()
    {
        return String.format
        (
            """
            SourceWithVolume
            {
                SourceId: %s
                Volume: %s
            }
            """,
            sourceId, volume
        );
    }
}
