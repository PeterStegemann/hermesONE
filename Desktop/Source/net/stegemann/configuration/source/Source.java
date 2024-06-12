package net.stegemann.configuration.source;

import lombok.Getter;
import net.stegemann.configuration.Model;
import net.stegemann.configuration.Named;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.SourceId;
import net.stegemann.configuration.type.Text;
import net.stegemann.configuration.type.ValueOutOfRangeException;
import net.stegemann.configuration.util.ConfigurationField;
import net.stegemann.io.xml.Names;
import net.stegemann.misc.ChangeListener;
import net.stegemann.misc.ChangeObservable;

import java.util.HashMap;

import static net.stegemann.misc.Utility.indent;

@Getter
@ConfigurationField( name = Names.SOURCE)
public abstract class Source extends ChangeObservable< Source>
                          implements ChangeListener< Text>, Comparable< Source>, Named
{
    public static final int SOURCE_FIXED = 0xfffe;
    public static final int SOURCE_NONE = 0xffff;

    public static final int SOURCE_START = 0;
    public static final int SOURCE_END = SOURCE_NONE;

    @ConfigurationField( name = Names.SOURCE_ID)
    private final SourceId id = new SourceId();
    @ConfigurationField( name = Names.SOURCE_NAME)
    private final Text name;
    @ConfigurationField( name = Names.SOURCE_MODEL)
    private final Number modelId = new Number( Model.MODEL_START, Model.MODEL_GLOBAL);

    protected Source()
    {
        name = new Text();

        name.addChangeListener( this);
    }

    protected Source( Source other)
    {
        name = new Text( other.name);

        try
        {
            modelId.setValue( other.modelId);
        }
        catch( ValueOutOfRangeException reason)
        {
            throw new RuntimeException( reason);
        }

        name.addChangeListener( this);
    }

    @Override
    public String toString()
    {
        return String.format( """
            Source
            {
                id: %s
                name: %s
                modelId: %s
            }""",
			indent( id),
            indent( name),
            indent( modelId)
		);
    }

	@Override
    public abstract Source clone();

    @Override
    public void hasChanged( Text object)
    {
        notifyChange( this);
    }

    public void setModelId( Number modelId)
        throws ValueOutOfRangeException
    {
        this.modelId.setValue( modelId);
    }

    public abstract void replaceSources( HashMap< SourceId, SourceId> sourcesMap);

    @Override
    public int compareTo( Source other)
    {
        if(( other instanceof Empty) || ( other instanceof Fixed))
        {
            return 1;
        }

        return name.getValue().compareToIgnoreCase( other.name.getValue());
    }
}
