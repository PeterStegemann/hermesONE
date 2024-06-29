package net.stegemann.configuration.util;

import net.stegemann.configuration.Configuration;
import net.stegemann.configuration.Model;
import net.stegemann.configuration.source.Source;
import net.stegemann.configuration.type.ModelId;
import net.stegemann.configuration.type.SourceId;
import net.stegemann.configuration.type.SourceWithVolume;import static net.stegemann.misc.Utility.doAll;

public interface Validatable
{
    default boolean validateReferencedSourceWithVolumes
    (
        Configuration configuration, ModelId modelId, Iterable< SourceWithVolume> sourceWithVolumes, String sourceName
    )
    {
        return doAll( sourceWithVolumes, sourceWithVolume -> validateReferencedSourceWithVolume
        (
            configuration, modelId, sourceWithVolume, sourceName)
        );
    }

    default boolean validateReferencedSourceWithVolume
    (
        Configuration configuration, ModelId modelId, SourceWithVolume sourceWithVolume, String sourceName
    )
    {
        return validateReferencedSource( configuration, modelId, sourceWithVolume.getSourceId(), sourceName);
    }

    default boolean validateReferencedSources
    (
        Configuration configuration, ModelId modelId, Iterable< SourceId> sourceIds, String sourceName
    )
    {
        return doAll( sourceIds, sourceId -> validateReferencedSource( configuration, modelId, sourceId, sourceName));
    }

    default boolean validateReferencedSource
    (
        Configuration configuration, ModelId modelId, SourceId sourceId, String sourceName
    )
    {
        if( Source.isNone( sourceId) || Source.isFixed( sourceId))
        {
            return true;
        }

        int sourceIdValue = sourceId.getValue();

        Source source = configuration.getSources().getSourceFromId( sourceId);

        if( source == null)
        {
            validationError
            (
                configuration, modelId, "unknown source " + sourceName + " id %s", sourceIdValue
            );

            return false;
        }

        ModelId sourceModelId = source.getModelId();

        return validateReferencedModelSource( configuration, modelId, sourceIdValue, sourceModelId, sourceName);
    }

    default boolean validateReferencedModelSource
    (
        Configuration configuration, ModelId modelId, int sourceIdValue, ModelId sourceModelId, String sourceName
    )
    {
        if( Model.isType( sourceModelId))
        {
            ModelId modelTypeId = configuration.getModels().getModelFromId( modelId).getTypeId();

            if( !modelTypeId.equals( sourceModelId))
            {
                validationError
                (
                    configuration,
                    modelId,
                    "model type id %s does not match type id %s of referenced source " + sourceName + " id %s",
                    modelTypeId.getValue(), sourceModelId.getValue(), sourceIdValue
                );

                return false;
            }
        }
        else if( Model.isModel( sourceModelId))
        {
            if( !modelId.equals( sourceModelId))
            {
                validationError
                (
                    configuration,
                    modelId,
                    "model id %s does not match model id %s of referenced source " + sourceName + " id %s",
                    modelId.getValue(), sourceModelId.getValue(), sourceIdValue
                );

                return false;
            }
        }

        return true;
    }

    default void validationError( Configuration configuration, ModelId modelId, String message, Object... arguments)
    {
        System.err.format( "Validate %s, %s.\n", validationName( configuration, modelId), message.formatted( arguments));
    }

    default String nameFromModelId( Configuration configuration, ModelId modelId)
    {
        if( Model.isGlobal( modelId))
        {
            return "global";
        }
        else if( Model.isNone( modelId))
        {
            return "none";
        }
        else if( Model.isType( modelId))
        {
            return configuration.getTypes().getTypeFromId( modelId).getName().getValue();
        }
        else if( Model.isModel( modelId))
        {
            return configuration.getModels().getModelFromId( modelId).getName().getValue();
        }

        return "unknown";
    }

    String validationName( Configuration configuration, ModelId modelId);
}
