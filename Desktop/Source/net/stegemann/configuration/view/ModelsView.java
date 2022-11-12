package net.stegemann.configuration.view;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import net.stegemann.configuration.Model;
import net.stegemann.configuration.Models;
import net.stegemann.configuration.type.Number;
import net.stegemann.misc.ChangeListener;
import net.stegemann.misc.ChangeObservable;

public class ModelsView extends ChangeObservable< ModelsView>
					 implements Iterable< Model>, ChangeListener< Models>
{
	private final Models baseModels;

	private final List< Model> models = new ArrayList< Model>();
	private Number typeId = null;

	/**
	 * This view presents a limited selection of models based on the given criteria.
	 * 
	 * @param models The models to pick from.
	 * @param typeId Selects sources that are valid for this type id. If this is null, no sources
	 * will be selected based on the type id.
	 * @param ModelId Selects sources that are valid for this model id. If this is null, no sources
	 * will be selected based on the model id.
	 */
	public ModelsView( Models models, Number typeId)
	{
		baseModels = models;
		models.addChangeListener( this);

		this.typeId = typeId;

		rescan();
	}

	@Override
	public String toString()
	{
		StringBuffer Buffer = new StringBuffer();

		Buffer.append( "ModelsView = {\n");

		for( Model Model: models)
		{
			Buffer.append( Model);
		}

		Buffer.append( "}\n");

		return Buffer.toString();
	}

	@Override
	public void hasChanged( Models object)
	{
		rescan();

		notifyChange( this);
	}

	@Override
	public Iterator< Model> iterator()
	{
		return new ModelsViewIterator( this);
	}

	private boolean isMatchingModel( Model model)
	{
		Number ModelTypeId = model.getTypeId();

		boolean IsMatching = (( typeId != null) && ( ModelTypeId.equals( typeId)));

		return IsMatching;
	}

	/**
	 * Build the list of sources for the given matching rules.
	 */
	public void rescan()
	{
		models.clear();

		// Copy references to all matching models.
		for( Model CurrentModel: baseModels)
		{
			// Match if the source is global, same type or same model.
			if( isMatchingModel( CurrentModel) == true)
			{
				models.add( CurrentModel);
			}
		}

		notifyChange( this);
	}

	public Model getModelFromIndex( int index)
	{
		if( index == -1)
		{
			return null;
		}

		try
		{
			return models.get( index);
		}
		catch( IndexOutOfBoundsException reason)
		{
			return null;
		}
	}

	public int getFullModelIndex( int index)
	{
		if( index == -1)
		{
			return -1;
		}

		try
		{
			Model Model = models.get( index);

			return baseModels.getIndexFromModel( Model);
		}
		catch( IndexOutOfBoundsException reason)
		{
			return -1;
		}
	}
/*
	public Number getSourceIdFromIndex( int index)
	{
		Source CurrentSource = getSourceFromIndex( index);

		if( CurrentSource == null)
		{
			return null;
		}

		return CurrentSource.getId();
	}
*/
	public int getModelIndexFromId( Number id)
	{
		int Index = 0;

		for( Model CurrentModel: models)
		{
			if( CurrentModel.getId().equals( id))
			{
				return( Index);
			}

			Index++;
		}

		return -1;
	}

	public int getModelCount()
	{
		return models.size();
	}

	private class ModelsViewIterator implements Iterator< Model>
	{
		private Iterator< Model> iterator;

		public ModelsViewIterator( ModelsView modelsView)
		{
			iterator = modelsView.models.iterator();
		}

		@Override
		public boolean hasNext()
		{
			return iterator.hasNext();
		}

		@Override
		public Model next()
		{
			return iterator.next();
		}

		@Override
		public void remove()
		{
			iterator.remove();
		}
	}

	public Model[] toArray()
	{
		Model Array[] = new Model[ getModelCount()];

		int Index = 0;

		for( Model Object: this)
		{
			Array[ Index++] = Object;
		}

		return Array;
	}
}
