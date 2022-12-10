package net.stegemann.configuration;

import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.ValueOutOfRangeException;
import net.stegemann.misc.ChangeListener;
import net.stegemann.misc.ChangeObservable;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

public class Models extends ChangeObservable< Models> implements Iterable< Model>, ChangeListener< Model>
{
	private final List< Model> models = new ArrayList<>();

	@Override
	public String toString()
	{
		StringBuffer Buffer = new StringBuffer();

		Buffer.append( "Models = {\n");

		for( Model Model: models)
		{
			Buffer.append( Model);
		}

		Buffer.append( "}\n");

		return Buffer.toString();
	}

	@Override
	public void hasChanged( Model object)
	{
		notifyChange( this);
	}

	@Override
	public Iterator< Model> iterator()
	{
		return models.iterator();
	}

	public void clear()
	{
		models.clear();		
	}

	public void fillChannels( int channels)
	{
		for( Model CurrentModel: models)
		{
			CurrentModel.fill( channels);
		}
	}

	public void addModel( Model model)
	{
		models.add( model);

		model.addChangeListener( this);
		notifyChange( this);
	}

	/**
	 * Insert is like add, but it will assign a new id to the model.
	 * 
	 * @param model The model to insert.
	 *
	 * @return The model
	 */
	public Model insertModel( Model model)
	{
		int freeId = 0;

		// Find free model index.
		for( Model currentModel: models)
		{
			if( currentModel.getId().equals( freeId) == false)
			{
				// Here's a gap.
				break;
			}

			freeId++;
		}

		try
		{ 
			model.getId().setValue( freeId);
		}
		catch( ValueOutOfRangeException reason)
		{
			return null;
		}

		models.add( freeId, model);

		model.addChangeListener( this);
		notifyChange( this);

		return model;
	}

	public Model removeModel( Model model)
	{
		if( models.remove( model) == true)
		{
			model.removeChangeListener( this);
			notifyChange( this);
		}

		return model;
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

	public Number getIdFromIndex( int index)
	{
		Model CurrentModel = getModelFromIndex( index);

		if( CurrentModel == null)
		{
			return null;
		}

		return CurrentModel.getId();
	}

	public Model getModelFromId( Number id)
	{
		return getModelFromIndex( getIndexFromId( id));
	}

	/** Get the index of a model in this container from its id.
	 * 
	 * @param id The id of the model.
	 * 
	 * @return The index in this container.
	 */
	public int getIndexFromId( Number id)
	{
		int Index = 0;

		for( Model CurrentModel: models)
		{
			if( CurrentModel.getId().equals( id) == true)
			{
				return Index;
			}

			Index++;
		}

		return -1;
	}

	public int getIndexFromModel( Model model)
	{
		return models.indexOf( model);
	}

	public int getCount()
	{
		return models.size();
	}
}