package net.stegemann.controller;

import java.util.HashMap;

import net.stegemann.configuration.Configuration;
import net.stegemann.configuration.Model;
import net.stegemann.configuration.Models;
import net.stegemann.configuration.Type;
import net.stegemann.configuration.Types;
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
import net.stegemann.gui.misc.SourceUtility;

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
		Models UseModels = configuration.getModels();
		Model NewModel = new Model();

		NewModel.setState( Model.State.USED);

		UseModels.insertModel( NewModel);

		try
		{
			NewModel.getTypeId().setValue( TypeId);
		}
		catch( ValueOutOfRangeException reason)
		{
			throw new RuntimeException( reason);
		}

		NewModel.getName().setValue( "Modell " + NewModel.getId().getValue());

		return NewModel;
	}

	public Model cloneModel( int index)
	{
		Models UseModels = configuration.getModels();
		Model SelectedModel = UseModels.getModelFromIndex( index);

		if( SelectedModel == null)
		{
			return null;
		}

		return cloneModel( SelectedModel, SelectedModel.getTypeId(),
						   new HashMap< SourceId, SourceId>());
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
		HashMap< SourceId, SourceId> SourcesMap = new HashMap< SourceId, SourceId>();

		// Add the type mappings.
		SourcesMap.putAll( typeSourcesMap);

		SourcesView ModelSourcesView =
			new SourcesView( UseSources, PickGlobals.No, null, model.getId(), HasEmpty.No,
							 HasFixed.No, HasProxies.Yes);

		for( Source UseSource: ModelSourcesView)
		{
			Source NewSource = cloneSource( UseSource, NewModel.getId());

			SourcesMap.put( UseSource.getId(), NewSource.getId());
		}

		// Now adjust the source references in the new sources and the model.
		SourcesView NewModelSourcesView =
			new SourcesView( UseSources, PickGlobals.No, null, NewModel.getId(), HasEmpty.No,
							 HasFixed.No, HasProxies.Yes);

		for( Source UseSource: NewModelSourcesView)
		{
			UseSource.replaceSources( SourcesMap);
		}

		NewModel.replaceSources( SourcesMap);

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
		HashMap< SourceId, SourceId> SourcesMap = new HashMap< SourceId, SourceId>();

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

		Source NewSource = source.clone();

		UseSources.insertSource( NewSource);

		if( model != null)
		{
			// Move the source to another model.
			NewSource.setModel( model);
		}
		else
		{
			// Keep the source in the same model, but give it a new name.
			Text Name = NewSource.getName();
			Name.setValue( Name.getValue() + " " + NewSource.getId().getValue());
		}

		return NewSource;
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
	{
		int NewSlot = findEmptySlot();

		if( NewSlot == -1)
		{
			return null;
		}

		Proxy NewProxy = new Proxy();

		try
		{
			NewProxy.getSlot().setValue( NewSlot);
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
		boolean AvailableProxies[] = new boolean[ Model.PROXIES];

		for( int CurrentProxyId = 0; CurrentProxyId < Model.PROXIES; CurrentProxyId++)
		{
			AvailableProxies[ CurrentProxyId] = true;
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
					AvailableProxies[ Slot] = false;
				}
			}
		}

		for( int CurrentProxyId = 0; CurrentProxyId < Model.PROXIES; CurrentProxyId++)
		{
			if( AvailableProxies[ CurrentProxyId] == true)
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
