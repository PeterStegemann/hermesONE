package net.stegemann.configuration.source;

import java.util.HashMap;import java.util.List;
import lombok.Getter;
import net.stegemann.configuration.Configuration;
import net.stegemann.configuration.Model;
import net.stegemann.configuration.Named;
import net.stegemann.configuration.type.*;
import net.stegemann.configuration.util.ConfigurationField;
import net.stegemann.configuration.util.Validatable;
import net.stegemann.io.xml.Names;
import net.stegemann.misc.ChangeListener;
import net.stegemann.misc.ChangeObservable;

import static net.stegemann.misc.Utility.indent;

@Getter
@ConfigurationField( name = Names.SOURCE)
public abstract class Source extends ChangeObservable< Source>
                          implements ChangeListener< Text>, Comparable< Source>, Named, Validatable
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

    public boolean validate( Configuration configuration)
    {
        // Check whether source is properly linked to a model at all.
        if
        (
            Model.isGlobal( modelId) ||
            Model.isNone( modelId) ||
            Model.isType( modelId) && ( configuration.getTypes().getIndexFromId( modelId) != -1) ||
            Model.isModel( modelId) && ( configuration.getModels().getIndexFromId( modelId) != -1)
        )
        {
            return true;
        }
        else
        {
            validationError( configuration, modelId, "unknown model id %s", modelId.getValue());

            return false;
        }
    }

    protected boolean validateReferencedSources
    (
        Configuration configuration, List< SourceWithVolume> sourceWithVolumes, String sourceName
    )
    {
        boolean result = true;

        for( SourceWithVolume sourceWithVolume: sourceWithVolumes)
        {
            if( validateReferencedSource( configuration, sourceWithVolume.getSourceId(), sourceName) == false)
            {
                result = false;
            }
        }

        return result;
    }

    protected boolean validateReferencedSource
    (
        Configuration configuration, SourceWithVolume sourceWithVolume, String sourceName
    )
    {
        return validateReferencedSource( configuration, sourceWithVolume.getSourceId(), sourceName);
    }

    protected boolean validateReferencedSource( Configuration configuration, SourceId sourceId, String sourceName)
    {
        if( Source.isNone( sourceId) || Source.isFixed( sourceId))
        {
            return true;
        }

        int sourceIdValue = sourceId.getValue();

        Source source = configuration.getSources().getSourceFromId( sourceId);

        if( source == null)
        {
            validationError( configuration, modelId, "unknown source " + sourceName + " id %s", sourceIdValue);

            return false;
        }

        ModelId sourceModelId = source.getModelId();

        if( Model.isGlobal( getModelId()))
        {
            return validateReferencedGlobalSource( configuration, sourceIdValue, sourceModelId, sourceName);
        }
        else if( Model.isType( getModelId()))
        {
            return validateReferencedTypeSource( configuration, sourceIdValue, sourceModelId, sourceName);
        }
        else if( Model.isModel( getModelId()))
        {
            return validateReferencedModelSource( configuration, getModelId(), sourceIdValue, sourceModelId, sourceName);
        }

        return true;
    }

    private boolean validateReferencedGlobalSource
    (
        Configuration configuration, int sourceIdValue, ModelId sourceModelId, String sourceName
    )
    {
        if( Model.isGlobal( sourceModelId))
        {
            return true;
        }
        else
        {
            validationError
            (
                configuration,
                modelId,
                "global source references not global source " + sourceName + " id %s",
                sourceIdValue
            );

            return false;
        }
    }

    private boolean validateReferencedTypeSource
    (
        Configuration configuration, int sourceIdValue, ModelId sourceModelId, String sourceName
    )
    {
        if( Model.isModel( sourceModelId))
        {
            validationError
            (
                configuration,
                modelId,
                "type source references model source " + sourceName + " id %s",
                sourceIdValue
            );

            return false;
        }
        else if( Model.isType( sourceModelId) && !getModelId().equals( sourceModelId))
        {
            validationError
            (
                configuration,
                modelId,
                "type id %s does not match type id %s of referenced source " + sourceName + " id %s",
                getModelId().getValue(), sourceModelId.getValue(), sourceIdValue
            );

            return false;
        }
        else
        {
            return true;
        }
    }

    @Override
    public String validationName( Configuration configuration, ModelId modelId)
    {
        return "source %s (%s) of model %s (%s)".formatted
        (
            id.getValue(), name.getValue(), modelId.getValue(), nameFromModelId( configuration, modelId)
        );
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

    public static boolean isNone( SourceId id)
    {
        return id.getValue() == SOURCE_NONE;
    }

    public static boolean isFixed( SourceId id)
    {
        return id.getValue() == SOURCE_FIXED;
    }

    public static boolean isSource( SourceId id)
    {
        return id.getValue() >= SOURCE_START && id.getValue() < SOURCE_FIXED;
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
