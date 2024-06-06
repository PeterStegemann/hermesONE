package net.stegemann.configuration.type;

import net.stegemann.configuration.source.Source;

import java.util.HashMap;

import static net.stegemann.misc.Utility.indent;

public class SourceId extends Number
{
    public SourceId()
    {
        super( Source.SOURCE_START, Source.SOURCE_NONE);

        try
        {
            setValue( Source.SOURCE_NONE);
        }
        catch( ValueOutOfRangeException reason)
        {
            // No exception possible here.
            throw new RuntimeException( reason);
        }
    }

    public SourceId( int value)
        throws ValueOutOfRangeException
    {
        super( Source.SOURCE_START, Source.SOURCE_NONE, value);
    }

    public SourceId( SourceId other)
    {
        super( other);
    }

    @Override
    public String toString()
    {
        return String.format("""
            SourceId
            {
                %s
            }""",
            indent( super.toString())
        );
    }

    public void replaceSource( HashMap< SourceId, SourceId> sourcesMap)
    {
        SourceId newSourceId = sourcesMap.get( this);

        if( newSourceId == null)
        {
            return;
        }

        try
        {
            setValue( newSourceId.getValue());
        }
        catch( ValueOutOfRangeException reason)
        {
            throw new RuntimeException( reason);
        }
    }
}
