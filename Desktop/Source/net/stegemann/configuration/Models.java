package net.stegemann.configuration;

import java.util.ArrayList;
import java.util.Comparator;import java.util.Iterator;
import java.util.List;import java.util.function.Function;
import lombok.ToString;
import net.stegemann.configuration.type.ModelId;
import net.stegemann.configuration.type.SourceId;
import net.stegemann.configuration.type.ValueOutOfRangeException;
import net.stegemann.misc.ChangeListener;
import net.stegemann.misc.ChangeObservable;import static net.stegemann.misc.Utility.doAll;

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
		int freeId = findFreeModelId();

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

    private int findFreeModelId()
    {
		int freeId = 0;

		for( Model model: models)
		{
			if( model.getId().equals( freeId) == false)
			{
				// Here's a gap.
				break;
			}

			freeId++;
		}

        return freeId;
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

    public void switchTypes( ModelId typeIdOne, ModelId typeIdTwo)
    {
        models.forEach( model -> model.switchType( typeIdOne, typeIdTwo));
    }

    public void switchModels( ModelId modelIdOne, ModelId modelIdTwo)
    {
        int modelIdOneValue = modelIdOne.getValue();
        int modelIdTwoValue = modelIdTwo.getValue();

        try
        {
            modelIdOne.setValue( modelIdTwoValue);
            modelIdTwo.setValue( modelIdOneValue);
        }
        catch( ValueOutOfRangeException reason)
        {
            throw new RuntimeException( reason);
        }

        models.sort( Comparator.comparing( Model::getId));
    }

    public void switchSources( SourceId sourceIdOne, SourceId sourceIdTwo)
    {
        models.forEach( model -> model.switchSources( sourceIdOne, sourceIdTwo));
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
		catch( IndexOutOfBoundsException Reason)
		{
			return null;
		}
	}

	public ModelId getIdFromIndex( int index)
	{
		Model model = getModelFromIndex( index);

		if( model == null)
		{
			return null;
		}

		return model.getId();
	}

	public Model getModelFromId( ModelId Id)
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
	public int getIndexFromId( ModelId id)
	{
		int index = 0;

		for( Model model: models)
		{
			if( model.getId().equals( id) == true)
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

    public boolean validate( Configuration configuration)
    {
        return doAll( models, model -> model.validate( configuration));
    }
}
