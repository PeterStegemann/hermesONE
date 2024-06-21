package net.stegemann.controller;

import lombok.Getter;
import net.stegemann.configuration.*;
import net.stegemann.configuration.source.Proxy;
import net.stegemann.configuration.source.Source;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.SourceId;
import net.stegemann.configuration.type.ValueOutOfRangeException;
import net.stegemann.configuration.view.ModelsView;
import net.stegemann.configuration.view.SourcesView;
import net.stegemann.configuration.view.SourcesView.HasEmpty;
import net.stegemann.configuration.view.SourcesView.HasFixed;
import net.stegemann.configuration.view.SourcesView.HasProxies;
import net.stegemann.configuration.view.SourcesView.PickGlobals;

import java.util.Arrays;
import java.util.HashMap;

@Getter
public class Controller
{
    private final Configuration configuration;

    public Controller( Configuration configuration)
    {
        this.configuration = configuration;
    }

    public Model createModel( Number typeId)
        throws ValueOutOfRangeException
    {
        Model model = new Model();
        configuration.getModels().insertModel( model);

        model.setState( Model.State.USED);
        model.getTypeId().setValue( typeId);
        model.getName().setValue( "Modell " + model.getId().getValue());

        return model;
    }

    public Model duplicateModel( int index)
    {
        Model model = configuration.getModels().getModelFromIndex( index);

        if( model == null)
        {
            return null;
        }

        return duplicateModel( model, model.getTypeId(), new HashMap<>());
    }

    private Model duplicateModel( Model model, Number typeId, HashMap< SourceId, SourceId> typeSourcesMap)
    {
        Model newModel = new Model( model, typeId);
        configuration.getModels().insertModel( newModel);

        // Map of all the sources that get cloned for this model. We need this later to change the references.
        HashMap< SourceId, SourceId> sourcesMap = new HashMap<>();

        // Add the type mappings.
        sourcesMap.putAll( typeSourcesMap);

        modelSources( model).forEach
        (
            source -> sourcesMap.put( source.getId(), duplicateSource( source, newModel.getId()).getId())
        );

        // Now adjust the source references in the new sources of the model.
        modelSources( newModel).forEach( source -> source.replaceSources( sourcesMap));

        newModel.replaceSources( sourcesMap);

        return newModel;
    }

    public void removeModel( int index)
    {
        Model model = configuration.getModels().getModelFromIndex( index);

        if( model == null)
        {
            return;
        }

        removeModel( model);
    }

    private void removeModel( Model model)
    {
        configuration.getModels().removeModel( model);

        // Also remove the sources for this model.
        modelSources( model).toList().forEach( this::removeSource);
    }

    public Type createType()
    {
        Type newType = new Type();
        configuration.getTypes().insertType( newType);

        newType.getName().setValue( "Typ " + ( newType.getId().getValue() - Model.TYPE_START));

        return newType;
    }

    public Type duplicateType( int index)
    {
        Type type = configuration.getTypes().getTypeFromIndex( index);

        if( type == null)
        {
            return null;
        }

        return duplicateType( type);
    }

    private Type duplicateType( Type type)
    {
        Type newType = new Type( type);
        configuration.getTypes().insertType( newType);

        // Clone the sources for this type and keep a map from the old source id to the new source id to replace cloned
        // references.
        HashMap< SourceId, SourceId> typeSourcesMap = duplicateTypeSources( type, newType);

        // Also clone all the models of this type, passing them the new type sources to use.
        duplicateTypeModels( type, newType, typeSourcesMap);

        return newType;
    }

    private HashMap< SourceId, SourceId> duplicateTypeSources( Type type, Type newType)
    {
        HashMap< SourceId, SourceId> typeSourcesMap = new HashMap<>();

        typeSources( type).forEach
        (
            source -> typeSourcesMap.put( source.getId(), duplicateSource( source, newType.getId()).getId())
        );

        // Now adjust the source references in the new sources.
        SourcesView newTypeSources = typeSources( newType);

        newTypeSources.forEach( source -> source.replaceSources( typeSourcesMap));

        return typeSourcesMap;
    }

    private void duplicateTypeModels( Type type, Type newType, HashMap< SourceId, SourceId> typeSourcesMap)
    {
        typeModels( type).forEach( model -> duplicateModel( model, newType.getId(), typeSourcesMap));
    }

    public void removeType( int index)
    {
        Type type = configuration.getTypes().getTypeFromIndex( index);

        if( type == null)
        {
            return;
        }

        removeType( type);
    }

    private void removeType( Type type)
    {
        configuration.getTypes().removeType( type);

        // Remove the models of this type.
        typeModels( type).toList().forEach( this::removeModel);
        // Remove the sources of this type.
        typeSources( type).toList().forEach( this::removeSource);
    }

    public Source addSource( Source source, Number modelId)
    {
        if( source == null)
        {
            return null;
        }

        configuration.getSources().insertSource( source);

        try
        {
            source.setModelId( modelId);
        }
        catch( ValueOutOfRangeException reason)
        {
            throw new RuntimeException( reason);
        }

        source.getName().setValue( SourceUtility.getTypeNameForSource( source) + " " + source.getId().getValue());

        return source;
    }

    public Source duplicateSource( int index)
    {
        Source selectedSource = configuration.getSources().getSourceFromIndex( index);

        if( selectedSource == null)
        {
            return null;
        }

        return duplicateSource( selectedSource, null);
    }

    private Source duplicateSource( Source source, Number modelId)
    {
        Source newSource = source.duplicate();
        configuration.getSources().insertSource( newSource);

        if( modelId != null)
        {
            // Move the source to another model.
            try
            {
                newSource.setModelId( modelId);
            }
            catch( ValueOutOfRangeException reason)
            {
                throw new RuntimeException( reason);
            }
        }

        return newSource;
    }

    public void removeSource( int index)
    {
        Source source = configuration.getSources().getSourceFromIndex( index);

        if( source == null)
        {
            return;
        }

        removeSource( source);
    }

    private void removeSource( Source source)
    {
        configuration.getSources().removeSource( source);
    }

    public Proxy addProxy( Number modelId)
    {
        int newSlot = findEmptyProxySlot();

        if( newSlot == -1)
        {
            return null;
        }

        Proxy proxy = new Proxy();

        try
        {
            proxy.getSlot().setValue( newSlot);
        }
        catch( ValueOutOfRangeException reason)
        {
            throw new RuntimeException( reason);
        }

        // TODO: Clean this proxy slot for all models.

        addSource( proxy, modelId);

        return proxy;
    }

    private int findEmptyProxySlot()
    {
        boolean[] availableProxies = new boolean[ Model.PROXIES];

        Arrays.fill( availableProxies, true);

        // Loop sources.
        for( Source currentSource: configuration.getSources())
        {
            if( currentSource.getClass() == Proxy.class)
            {
                Proxy currentProxy = ( Proxy) currentSource;

                int slot = currentProxy.getSlot().getValue();

                // Better ignore invalid slots.
                if( slot < Model.PROXIES)
                {
                    availableProxies[ slot] = false;
                }
            }
        }

        for( int currentProxyId = 0; currentProxyId < Model.PROXIES; currentProxyId++)
        {
            if( availableProxies[ currentProxyId] == true)
            {
                return currentProxyId;
            }
        }

        return -1;
    }

    public void removeProxy( int index)
    {
        removeSource( index);
    }

    public void switchSources( Source sourceOne, Source sourceTwo)
    {
        configuration.getSources().switchSources( sourceOne.getId(), sourceTwo.getId());

        configuration.getModels().forEach( model -> model.switchSources( sourceOne.getId(), sourceTwo.getId()));
    }

    private ModelsView typeModels( Type type)
    {
        return new ModelsView( configuration.getModels(), type.getId());
    }

    private SourcesView typeSources( Type type)
    {
        return new SourcesView
        (
            configuration.getSources(), PickGlobals.No, type.getId(), null,
            HasEmpty.No, HasFixed.No, HasProxies.Yes
        );
    }

    private SourcesView modelSources( Model model)
    {
        return new SourcesView
        (
            configuration.getSources(), PickGlobals.No, null, model.getId(),
            HasEmpty.No, HasFixed.No, HasProxies.Yes
        );
    }
}
