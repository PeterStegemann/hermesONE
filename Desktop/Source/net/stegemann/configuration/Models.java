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
public class Models
    extends ChangeObservable< Models>
    implements Iterable< Model>, ChangeListener< Model>
{
	private final List< Model> models = new ArrayList<>();

	@Override
	public void hasChanged( Model Model)
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
		int FreeId = 0;

		// Find free model index.
		for( Model CurrentModel: models)
		{
			if( CurrentModel.getId().equals( FreeId) == false)
			{
				// Here's a gap.
				break;
			}

			FreeId++;
		}

		try
		{ 
			model.getId().setValue( FreeId);
		}
		catch( ValueOutOfRangeException reason)
		{
			return null;
		}

		models.add( FreeId, model);

		model.addChangeListener( this);
		notifyChange( this);

		return model;
	}

	public Model removeModel( Model Model)
	{
		if( models.remove( Model) == true)
		{
			Model.removeChangeListener( this);
			notifyChange( this);
		}

		return Model;
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
		Model CurrentModel = getModelFromIndex( Index);

		if( CurrentModel == null)
		{
			return null;
		}

		return CurrentModel.getId();
	}

	public Model getModelFromId( Number Id)
	{
		return getModelFromIndex( getIndexFromId( Id));
	}

	/** Get the index of a model in this container from its id.
	 * 
	 * @param Id The id of the model.
	 * 
	 * @return The index in this container.
	 */
	public int getIndexFromId( Number Id)
	{
		int Index = 0;

		for( Model CurrentModel: models)
		{
			if( CurrentModel.getId().equals( Id) == true)
			{
				return Index;
			}

			Index++;
		}

		return -1;
	}

	public int getIndexFromModel( Model Model)
	{
		return models.indexOf( Model);
	}

	public int getCount()
	{
		return models.size();
	}
}
