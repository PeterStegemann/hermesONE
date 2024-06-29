package net.stegemann.configuration.source;

import lombok.Getter;
import net.stegemann.configuration.Configuration;import net.stegemann.configuration.Signal;
import net.stegemann.configuration.type.SourceId;
import net.stegemann.configuration.type.ValueOutOfRangeException;
import net.stegemann.configuration.type.Volume;
import net.stegemann.configuration.util.ConfigurationField;
import net.stegemann.io.xml.Names;

import java.util.HashMap;import static net.stegemann.misc.Utility.indent;

@Getter
@ConfigurationField( name = Names.SOURCE_STORE)
public final class Store extends Source
{
    public static final int INIT_SIGNAL_PER_VALUE = Signal.VALUE_RANGE / 200;

    @ConfigurationField( name = Names.SOURCE_STORE_INPUT)
    private final SourceId input;
    @ConfigurationField( name = Names.SOURCE_STORE_INIT)
    private final Volume init;

    public Store()
    {
        input = new SourceId();

        try
        {
            init = new Volume( INIT_SIGNAL_PER_VALUE, Signal.NEUTRAL_VALUE / INIT_SIGNAL_PER_VALUE);
            input.setValue( Source.SOURCE_NONE);
        }
        catch( ValueOutOfRangeException reason)
        {
            throw new RuntimeException( reason);
        }
    }

    public Store( Store other)
    {
        super( other);

        input = new SourceId( other.input);
        init = new Volume( other.init);
    }

    @Override
    public Source duplicate()
    {
        return new Store( this);
    }

    @Override
    public void replaceSources( HashMap< SourceId, SourceId> sourcesMap)
    {
        input.replaceSource( sourcesMap);
    }

    @Override
    public void switchSources( SourceId sourceIdOne, SourceId sourceIdTwo)
    {
        input.switchSource( sourceIdOne, sourceIdTwo);
    }

    @Override
    public boolean validate( Configuration configuration)
    {
        return
            super.validate( configuration) &&
            validateReferencedSource( configuration, input,"input");
    }

    @Override
    public String toString()
    {
        return String.format
        (
            """
            Store
            {
                %s
                input: %s
                init: %s
            }
            """,
            indent( super.toString()), indent( input), indent( init)
        );
    }
}
