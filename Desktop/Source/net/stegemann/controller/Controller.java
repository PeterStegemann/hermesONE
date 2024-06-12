package net.stegemann.controller;

import lombok.Getter;
import net.stegemann.configuration.*;
import net.stegemann.configuration.source.Proxy;
import net.stegemann.configuration.source.Source;
import net.stegemann.configuration.source.Sources;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.SourceId;
import net.stegemann.configuration.type.Text;
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

	public Model addModel( Number typeId)
	{
		Models models = configuration.getModels();
		Model model = new Model();

		model.setState( Model.State.USED);

		models.insertModel( model);

		try
		{
			model.getTypeId().setValue( typeId);
		}
		catch( ValueOutOfRangeException reason)
		{
			throw new RuntimeException( reason);
		}

		model.getName().setValue( "Modell " + model.getId().getValue());

		return model;
	}

	public Model cloneModel( int index)
	{
		Models UseModels = configuration.getModels();
		Model SelectedModel = UseModels.getModelFromIndex( index);

		if( SelectedModel == null)
		{
			return null;
		}

		return cloneModel( SelectedModel, SelectedModel.getTypeId(), new HashMap<>());
	}

	private Model cloneModel( Model model, Number type, HashMap< SourceId, SourceId> typeSourcesMap)
	{
		Models models = configuration.getModels();

		Model clonedModel = model.clone();

		try
		{
			clonedModel.getTypeId().setValue( type);
		}
		catch( ValueOutOfRangeException reason)
		{
			throw new RuntimeException( reason);
		}

		models.insertModel( clonedModel);

		// Give the cloned model a new name.
		Text name = clonedModel.getName();
		name.setValue( name.getValue() + " " + clonedModel.getId().getValue());

		// Also copy the sources for this model.
		Sources Sources = configuration.getSources();

		// Map of all the sources that get cloned for this model. We need this later to change the
		// references.
		HashMap< SourceId, SourceId> sourcesMap = new HashMap<>();

		// Add the type mappings.
		sourcesMap.putAll( typeSourcesMap);

		SourcesView ModelSourcesView = new SourcesView
		(
            Sources, PickGlobals.No, null, model.getId(), HasEmpty.No, HasFixed.No, HasProxies.Yes
        );

		for( Source source: ModelSourcesView)
		{
			Source newSource = cloneSource( source, clonedModel.getId());

			sourcesMap.put( source.getId(), newSource.getId());
		}

		// Now adjust the source references in the new sources and the model.
		SourcesView NewModelSourcesView = new SourcesView
		(
            Sources, PickGlobals.No, null, clonedModel.getId(), HasEmpty.No, HasFixed.No, HasProxies.Yes
        );

		for( Source source: NewModelSourcesView)
		{
			source.replaceSources( sourcesMap);
		}

		clonedModel.replaceSources( sourcesMap);

		return clonedModel;
	}

	public void removeModel( int index)
	{
		Models models = configuration.getModels();
		Model selectedModel = models.getModelFromIndex( index);

		if( selectedModel == null)
		{
			return;
		}

		removeModel( selectedModel);
	}
	
	private void removeModel( Model model)
	{
		Models models = configuration.getModels();

		models.removeModel( model);

		// Also remove the sources for this model.
		Sources sources = configuration.getSources();

		SourcesView modelSourcesView = new SourcesView
		(
            sources, PickGlobals.No, null, model.getId(), HasEmpty.No, HasFixed.No, HasProxies.Yes
        );

		for( Source source: modelSourcesView)
		{
			removeSource( source);
		}
	}

	public Type addType()
	{
		Types types = configuration.getTypes();
		Type newType = new Type();

		types.insertType( newType);

		newType.getName().setValue( "Typ " + ( newType.getId().getValue() - Model.TYPE_START));

		return newType;
	}

	public Type cloneType( int index)
	{
		Types types = configuration.getTypes();
		Type selectedType = types.getTypeFromIndex( index);

		if( selectedType == null)
		{
			return null;
		}

		return cloneType( selectedType);
	}

	private Type cloneType( Type type)
	{
		Types types = configuration.getTypes();

		Type newType = type.clone();
		types.insertType( newType);

		Text name = newType.getName();
		name.setValue( name.getValue() + " " + ( newType.getId().getValue() - Model.TYPE_START));

		// Also clone the sources for this type.
		Sources sources = configuration.getSources();

		// Map of all the source ids that get cloned for this type.
		HashMap< SourceId, SourceId> sourcesMap = new HashMap<>();

		SourcesView typeSourcesView = new SourcesView
		(
            sources, PickGlobals.No, type.getId(), null, HasEmpty.No, HasFixed.No, HasProxies.Yes
        );

		for( Source source: typeSourcesView)
		{
			Source NewSource = cloneSource( source, newType.getId());

			sourcesMap.put( source.getId(), NewSource.getId());
		}

		// Now adjust the source references in the new sources.
		SourcesView newTypeSourcesView = new SourcesView
		(
            sources, PickGlobals.No, newType.getId(), null, HasEmpty.No, HasFixed.No, HasProxies.Yes
        );

		for( Source source: newTypeSourcesView)
		{
			source.replaceSources( sourcesMap);
		}

		// Also clone all the models of this type.
		Models models = configuration.getModels();

		ModelsView typeModelsView =	new ModelsView( models, type.getId());

		for( Model model: typeModelsView.toArray())
		{
			cloneModel( model, newType.getId(), sourcesMap);
		}

		return newType;
	}

	public void removeType( int index)
	{
		Types types = configuration.getTypes();
		Type selectedType = types.getTypeFromIndex( index);

		if( selectedType == null)
		{
			return;
		}

		removeType( selectedType);
	}

	private void removeType( Type type)
	{
		Types Types = configuration.getTypes();

		Types.removeType( type);

		// Also remove the sources for this type.
		Sources Sources = configuration.getSources();

		SourcesView typeSourcesView = new SourcesView
		(
            Sources, PickGlobals.No, type.getId(), null, HasEmpty.No, HasFixed.No, HasProxies.Yes
        );

		for( Source source: typeSourcesView)
		{
			removeSource( source);
		}

		// Also remove all the models of this type.
		Models models = configuration.getModels();

		ModelsView typeModelsView =	new ModelsView( models, type.getId());

		for( Model model: typeModelsView.toArray())
		{
			removeModel( model);
		}
	}

	public Source addSource( Source source, Number modelId)
		throws ValueOutOfRangeException
	{
		if( source == null)
		{
			return null;
		}

		source.setModelId( modelId);

		Sources sources = configuration.getSources();

		sources.insertSource( source);

		source.getName().setValue( SourceUtility.getTypeNameForSource( source) + " " + source.getId().getValue());

		return source;
	}

	public Source cloneSource( int index)
	{
		Sources sources = configuration.getSources();

		Source selectedSource = sources.getSourceFromIndex( index);

		if( selectedSource == null)
		{
			return null;
		}

		return cloneSource( selectedSource, null);
	}

	private Source cloneSource( Source source, Number model)
	{
		Sources sources = configuration.getSources();

		Source newSource = source.clone();

		sources.insertSource( newSource);

		if( model != null)
		{
			// Move the source to another model.
			try
			{
				newSource.setModelId( model);
			}
			catch( ValueOutOfRangeException reason)
			{
				throw new RuntimeException( reason);
			}
		}
		else
		{
			// Keep the source in the same model, but give it a new name.
			Text name = newSource.getName();
			name.setValue( name.getValue() + " " + newSource.getId().getValue());
		}

		return newSource;
	}

	public void removeSource( int index)
	{
		Sources sources = configuration.getSources();

		Source selectedSource = sources.getSourceFromIndex( index);

		if( selectedSource == null)
		{
			return;
		}

		removeSource( selectedSource);
	}
	
	private void removeSource( Source source)
	{
		Sources sources = configuration.getSources();

		sources.removeSource( source);
	}

	public Proxy addProxy( Number modelId)
		throws ValueOutOfRangeException
	{
		int newSlot = findEmptySlot();

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

	private int findEmptySlot()
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
}
