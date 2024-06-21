package net.stegemann.configuration.source;

import net.stegemann.configuration.type.SourceId;
import net.stegemann.configuration.type.ValueOutOfRangeException;
import net.stegemann.configuration.util.ConfigurationField;

import java.util.HashMap;import static net.stegemann.misc.Utility.indent;

@ConfigurationField( ignore = true)
public final class Fixed extends Source
{
    public Fixed()
    {
        try
        {
            getId().setValue( Source.SOURCE_FIXED);
        }
        catch( ValueOutOfRangeException ignored) {}

        getName().setValue( "<Fest>");
    }

    public Fixed( Fixed other)
    {
        super( other);
    }

    @Override
    public Source duplicate()
    {
        return new Fixed( this);
    }

    @Override
    public void replaceSources( HashMap< SourceId, SourceId> sourcesMap) {}

    @Override
    public void switchSources( SourceId sourceIdOne, SourceId sourceIdTwo) {}

    @Override
    public int compareTo( Source other)
    {
        if( other instanceof Empty)
        {
            return 1;
        }

        if( other instanceof Fixed)
        {
            return 0;
        }

        return -1;
    }

    @Override
    public String toString()
    {
        return String.format
        (
            """
            Fixed
            {
                %s
            }
            """,
            indent( super.toString())
        );
    }
}
