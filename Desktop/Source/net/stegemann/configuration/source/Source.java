package net.stegemann.configuration.source;

import java.util.HashMap;
import lombok.Getter;
import net.stegemann.configuration.Model;
import net.stegemann.configuration.Named;
import net.stegemann.configuration.type.*;
import net.stegemann.configuration.util.ConfigurationField;
import net.stegemann.io.xml.Names;
import net.stegemann.misc.ChangeListener;
import net.stegemann.misc.ChangeObservable;

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
    private final ModelId modelId = new ModelId( Model.MODEL_START, Model.MODEL_GLOBAL);

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

    public void setModelId( ModelId modelId)
        throws ValueOutOfRangeException
    {
        this.modelId.setValue( modelId);
    }

	public abstract Source duplicate();

    public abstract void replaceSources( HashMap< SourceId, SourceId> sourcesMap);

    /**
     * Switch two source ids. This is used to change the visible order of sources.
     *
     * @param sourceIdOne One source id
     * @param sourceIdTwo Another source id
     */
    public abstract void switchSources( SourceId sourceIdOne, SourceId sourceIdTwo);

    public void switchModel( ModelId modelIdOne, ModelId modelIdTwo)
    {
        modelId.switchModel( modelIdOne, modelIdTwo);
    }

    @Override
    public void hasChanged( Text object)
    {
        notifyChange( this);
    }

    @Override
    public int compareTo( Source other)
    {
        if( other == null)
        {
            return 1;
        }

        return id.compareTo( other.id);
    }

    @Override
    public String toString()
    {
        return String.format
        (
            """
            Source
            {
                id: %s
                name: %s
                modelId: %s
            }
            """,
			indent( id),
            indent( name),
            indent( modelId)
		);
    }
}
