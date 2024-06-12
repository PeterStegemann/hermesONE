package net.stegemann.configuration;

import lombok.ToString;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.ValueOutOfRangeException;
import net.stegemann.misc.ChangeListener;
import net.stegemann.misc.ChangeObservable;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

@ToString
public class Models extends ChangeObservable< Models>
                 implements Iterable< Model>, ChangeListener< Model>
{
	private final List< Model> models = new ArrayList<>();

	@Override
	public void hasChanged( Model model)
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
		for( Model currentModel: models)
		{
			currentModel.fill( channels);
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

	public Model getModelFromIndex( int Index)
	{
		if( Index == -1)
		{
			return null;
		}

		try
		{
			return models.get( Index);
		}
		catch( IndexOutOfBoundsException Reason)
		{
			return null;
		}
	}

	public Number getIdFromIndex( int Index)
	{
		Model currentModel = getModelFromIndex( Index);

		if( currentModel == null)
		{
			return null;
		}

		return currentModel.getId();
	}

	public Model getModelFromId( Number Id)
	{
		return getModelFromIndex( getIndexFromId( Id));
	}

	/**
	 * Get the index of a model in this container from its id.
	 * 
	 * @param id The id of the model.
	 * 
	 * @return The index in this container.
	 */
	public int getIndexFromId( Number id)
	{
		int index = 0;

		for( Model currentModel: models)
		{
			if( currentModel.getId().equals( id) == true)
			{
				return index;
			}

			index++;
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
