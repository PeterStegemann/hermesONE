package net.stegemann.controller;

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

import java.util.HashMap;

public class Controller
{
	private final Configuration configuration;

	public Controller( Configuration configuration)
	{
		this.configuration = configuration;
	}

	public Configuration getConfiguration()
	{
		return configuration;
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
		Models UseModels = configuration.getModels();

		Model NewModel = model.clone();
		try
		{
			NewModel.getTypeId().setValue( type);
		}
		catch( ValueOutOfRangeException reason)
		{
			throw new RuntimeException( reason);
		}

		UseModels.insertModel( NewModel);

		// Give the cloned model a new name.
		Text Name = NewModel.getName();
		Name.setValue( Name.getValue() + " " + NewModel.getId().getValue());

		// Also copy the sources for this model.
		Sources UseSources = configuration.getSources();

		// Map of all the sources that get cloned for this model. We need this later to change the
		// references.
		HashMap< SourceId, SourceId> sourcesMap = new HashMap<>();

		// Add the type mappings.
		sourcesMap.putAll( typeSourcesMap);

		SourcesView ModelSourcesView =
			new SourcesView( UseSources, PickGlobals.No, null, model.getId(), HasEmpty.No,
							 HasFixed.No, HasProxies.Yes);

		for( Source UseSource: ModelSourcesView)
		{
			Source NewSource = cloneSource( UseSource, NewModel.getId());

			sourcesMap.put( UseSource.getId(), NewSource.getId());
		}

		// Now adjust the source references in the new sources and the model.
		SourcesView NewModelSourcesView =
			new SourcesView( UseSources, PickGlobals.No, null, NewModel.getId(), HasEmpty.No,
							 HasFixed.No, HasProxies.Yes);

		for( Source UseSource: NewModelSourcesView)
		{
			UseSource.replaceSources( sourcesMap);
		}

		NewModel.replaceSources( sourcesMap);

		return NewModel;
	}

	public void removeModel( int index)
	{
		Models UseModels = configuration.getModels();
		Model SelectedModel = UseModels.getModelFromIndex( index);

		if( SelectedModel == null)
		{
			return;
		}

		removeModel( SelectedModel);
	}
	
	private void removeModel( Model model)
	{
		Models UseModels = configuration.getModels();

		UseModels.removeModel( model);

		// Also remove the sources for this model.
		Sources UseSources = configuration.getSources();

		SourcesView ModelSourcesView =
			new SourcesView( UseSources, PickGlobals.No, null, model.getId(), HasEmpty.No,
							 HasFixed.No, HasProxies.Yes);

		for( Source UseSource: ModelSourcesView)
		{
			removeSource( UseSource);
		}
	}

	public Type addType()
	{
		Types UseTypes = configuration.getTypes();
		Type NewType = new Type();

		UseTypes.insertType( NewType);

		NewType.getName().setValue( "Typ " + ( NewType.getId().getValue() - Model.TYPE_START));

		return NewType;
	}

	public Type cloneType( int index)
	{
		Types UseTypes = configuration.getTypes();
		Type SelectedType = UseTypes.getTypeFromIndex( index);

		if( SelectedType == null)
		{
			return null;
		}

		return cloneType( SelectedType);
	}

	private Type cloneType( Type type)
	{
		Types UseTypes = configuration.getTypes();

		Type NewType = type.clone();
		UseTypes.insertType( NewType);

		Text Name = NewType.getName();
		Name.setValue( Name.getValue() + " " + ( NewType.getId().getValue() - Model.TYPE_START));

		// Also clone the sources for this type.
		Sources UseSources = configuration.getSources();

		// Map of all the source ids that get cloned for this type.
		HashMap< SourceId, SourceId> SourcesMap = new HashMap<>();

		SourcesView TypeSourcesView =
			new SourcesView( UseSources, PickGlobals.No, type.getId(), null, HasEmpty.No,
							 HasFixed.No, HasProxies.Yes);

		for( Source UseSource: TypeSourcesView)
		{
			Source NewSource = cloneSource( UseSource, NewType.getId());

			SourcesMap.put( UseSource.getId(), NewSource.getId());
		}

		// Now adjust the source references in the new sources.
		SourcesView NewTypeSourcesView =
			new SourcesView( UseSources, PickGlobals.No, NewType.getId(), null, HasEmpty.No,
							 HasFixed.No, HasProxies.Yes);

		for( Source UseSource: NewTypeSourcesView)
		{
			UseSource.replaceSources( SourcesMap);
		}

		// Also clone all the models of this type.
		Models UseModels = configuration.getModels();

		ModelsView TypeModelsView =	new ModelsView( UseModels, type.getId());

		for( Model UseModel: TypeModelsView.toArray())
		{
			cloneModel( UseModel, NewType.getId(), SourcesMap);
		}

		return NewType;
	}

	public void removeType( int index)
	{
		Types UseTypes = configuration.getTypes();
		Type SelectedType = UseTypes.getTypeFromIndex( index);

		if( SelectedType == null)
		{
			return;
		}

		removeType( SelectedType);
	}

	private void removeType( Type type)
	{
		Types UseTypes = configuration.getTypes();

		UseTypes.removeType( type);

		// Also remove the sources for this type.
		Sources UseSources = configuration.getSources();

		SourcesView TypeSourcesView =
			new SourcesView( UseSources, PickGlobals.No, type.getId(), null, HasEmpty.No,
							 HasFixed.No, HasProxies.Yes);

		for( Source UseSource: TypeSourcesView)
		{
			removeSource( UseSource);
		}

		// Also remove all the models of this type.
		Models UseModels = configuration.getModels();

		ModelsView TypeModelsView =	new ModelsView( UseModels, type.getId());

		for( Model UseModel: TypeModelsView.toArray())
		{
			removeModel( UseModel);
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

		Sources UseSources = configuration.getSources();

		UseSources.insertSource( source);

		source.getName().setValue( SourceUtility.getTypeNameForSource( source) + " " +
								   source.getId().getValue());

		return source;
	}

	public Source cloneSource( int index)
	{
		Sources UseSources = configuration.getSources();

		Source SelectedSource = UseSources.getSourceFromIndex( index);

		if( SelectedSource == null)
		{
			return null;
		}

		return cloneSource( SelectedSource, null);
	}

	private Source cloneSource( Source source, Number model)
	{
		Sources UseSources = configuration.getSources();

		Source newSource = source.clone();

		UseSources.insertSource( newSource);

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
		Sources UseSources = configuration.getSources();

		Source SelectedSource = UseSources.getSourceFromIndex( Index);

		if( SelectedSource == null)
		{
			return;
		}

		removeSource( SelectedSource);
	}
	
	private void removeSource( Source source)
	{
		Sources UseSources = configuration.getSources();

		UseSources.removeSource( source);		
	}

	public Proxy addProxy( Number modelId)
		throws ValueOutOfRangeException
	{
		int newSlot = findEmptySlot();

		if( newSlot == -1)
		{
			return null;
		}

		Proxy NewProxy = new Proxy();

		try
		{
			NewProxy.getSlot().setValue( newSlot);
		}
		catch( ValueOutOfRangeException Reason)
		{
			throw new RuntimeException( Reason);
		}

		// TODO: Clean this proxy slot for all models.

		addSource( NewProxy, modelId);

		return NewProxy;
	}

	private int findEmptySlot()
	{
		boolean[] availableProxies = new boolean[ Model.PROXIES];

		for( int CurrentProxyId = 0; CurrentProxyId < Model.PROXIES; CurrentProxyId++)
		{
			availableProxies[ CurrentProxyId] = true;
		}

		// Loop sources.
		for( Source CurrentSource: configuration.getSources())
		{
			if( CurrentSource.getClass() == Proxy.class)
			{
				Proxy CurrentProxy = ( Proxy) CurrentSource;

				int Slot = CurrentProxy.getSlot().getValue();

				// Better ignore invalid slots.
				if( Slot < Model.PROXIES)
				{
					availableProxies[ Slot] = false;
				}
			}
		}

		for( int CurrentProxyId = 0; CurrentProxyId < Model.PROXIES; CurrentProxyId++)
		{
			if( availableProxies[ CurrentProxyId] == true)
			{
				return CurrentProxyId;
			}
		}

		return -1;
	}

	public void removeProxy( int Index)
	{
		removeSource( Index);
	}
}
