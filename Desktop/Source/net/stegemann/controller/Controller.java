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

	public Model addModel( Number TypeId)
	{
		Models models = configuration.getModels();
		Model model = new Model();

		model.setState( Model.State.USED);

		models.insertModel( model);

		try
		{
			model.getTypeId().setValue( TypeId);
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

		Model NewModel = model.clone();

		try
		{
			NewModel.getTypeId().setValue( type);
		}
		catch( ValueOutOfRangeException reason)
		{
			throw new RuntimeException( reason);
		}

		models.insertModel( NewModel);

		// Give the cloned model a new name.
		Text name = NewModel.getName();
		name.setValue( name.getValue() + " " + NewModel.getId().getValue());

		// Also copy the sources for this model.
		Sources Sources = configuration.getSources();

		// Map of all the sources that get cloned for this model. We need this later to change the
		// references.
		HashMap< SourceId, SourceId> sourcesMap = new HashMap<>();

		// Add the type mappings.
		sourcesMap.putAll( typeSourcesMap);

		SourcesView ModelSourcesView = new SourcesView( Sources, PickGlobals.No, null, model.getId(),
														HasEmpty.No, HasFixed.No, HasProxies.Yes);

		for( Source source: ModelSourcesView)
		{
			Source newSource = cloneSource( source, NewModel.getId());

			sourcesMap.put( source.getId(), newSource.getId());
		}

		// Now adjust the source references in the new sources and the model.
		SourcesView NewModelSourcesView = new SourcesView( Sources, PickGlobals.No, null, NewModel.getId(),
														   HasEmpty.No, HasFixed.No, HasProxies.Yes);

		for( Source source: NewModelSourcesView)
		{
			source.replaceSources( sourcesMap);
		}

		NewModel.replaceSources( sourcesMap);

		return NewModel;
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
	
	private void removeModel( Model Model)
	{
		Models Models = configuration.getModels();

		Models.removeModel( Model);

		// Also remove the sources for this model.
		Sources Sources = configuration.getSources();

		SourcesView ModelSourcesView = new SourcesView( Sources, PickGlobals.No, null, Model.getId(),
														HasEmpty.No, HasFixed.No, HasProxies.Yes);

		for( Source Source: ModelSourcesView)
		{
			removeSource( Source);
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

		SourcesView typeSourcesView = new SourcesView( sources, PickGlobals.No, type.getId(), null,
													   HasEmpty.No, HasFixed.No, HasProxies.Yes);

		for( Source source: typeSourcesView)
		{
			Source NewSource = cloneSource( source, newType.getId());

			sourcesMap.put( source.getId(), NewSource.getId());
		}

		// Now adjust the source references in the new sources.
		SourcesView newTypeSourcesView = new SourcesView( sources, PickGlobals.No, newType.getId(), null,
														  HasEmpty.No, HasFixed.No, HasProxies.Yes);

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

	public void removeType( int Index)
	{
		Types Types = configuration.getTypes();
		Type SelectedType = Types.getTypeFromIndex( Index);

		if( SelectedType == null)
		{
			return;
		}

		removeType( SelectedType);
	}

	private void removeType( Type Type)
	{
		Types Types = configuration.getTypes();

		Types.removeType( Type);

		// Also remove the sources for this type.
		Sources Sources = configuration.getSources();

		SourcesView TypeSourcesView = new SourcesView( Sources, PickGlobals.No, Type.getId(), null,
													   HasEmpty.No, HasFixed.No, HasProxies.Yes);

		for( Source Source: TypeSourcesView)
		{
			removeSource( Source);
		}

		// Also remove all the models of this type.
		Models Models = configuration.getModels();

		ModelsView TypeModelsView =	new ModelsView( Models, Type.getId());

		for( Model Model: TypeModelsView.toArray())
		{
			removeModel( Model);
		}
	}

	public Source addSource( Source source, Number modelId)
		throws ValueOutOfRangeException
	{
		if( source == null)
		{
			return null;
		}

		source.setModel( modelId);

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
				newSource.setModel( model);
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

	public void removeSource( int Index)
	{
		Sources sources = configuration.getSources();

		Source selectedSource = sources.getSourceFromIndex( Index);

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
