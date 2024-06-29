package net.stegemann.configuration;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.Iterator;
import java.util.List;
import net.stegemann.configuration.type.ModelId;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.ValueOutOfRangeException;
import net.stegemann.misc.ChangeListener;
import net.stegemann.misc.ChangeObservable;import net.stegemann.misc.Utility;

import static net.stegemann.misc.Utility.doAll;
import static net.stegemann.misc.Utility.indent;

public class Types extends ChangeObservable< Types>
                implements Iterable< Type>, ChangeListener< Type>
{
	private final List< Type> types = new ArrayList<>();

	@Override
	public void hasChanged( Type object)
	{
		notifyChange( this);
	}

	@Override
	public Iterator< Type> iterator()
	{
		return types.iterator();
	}

	public void clear()
	{
		types.clear();		
	}

	public void addType( Type type)
	{
		types.add( type);

		type.addChangeListener( this);
		notifyChange( this);
	}

	public Type insertType( Type type)
	{
		int freeId = Model.TYPE_START;

		// Find free type index.
		for( Type currentType: types)
		{
			if( currentType.getId().equals( freeId) == false)
			{
				// Here's a gap.
				break;
			}

			freeId++;
		}

		try
		{
			type.getId().setValue( freeId);
		}
		catch( ValueOutOfRangeException reason)
		{
			return null;
		}

		types.add(( freeId - Model.TYPE_START), type);

		type.addChangeListener( this);
		notifyChange( this);

		return type;
	}

	public boolean removeType( Type type)
	{
		if( types.remove( type) == true)
		{
			type.removeChangeListener( this);
			notifyChange( this);

			return true;
		}

		return false;
	}

    public void switchTypes( ModelId modelIdOne, ModelId modelIdTwo)
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

        types.sort( Comparator.comparing( Type::getId));
    }

	public Type getTypeFromIndex( int index)
	{
		if( index == -1)
		{
			return null;
		}

		try
		{
			return types.get( index);
		}
		catch( IndexOutOfBoundsException Reason)
		{
			return null;
		}
	}

	public ModelId getTypeIdFromIndex( int index)
	{
		Type type = getTypeFromIndex( index);

		if( type == null)
		{
			return null;
		}

		return type.getId();
	}

	/** Get the index of a type in this container from its id.
	 * 
	 * @param Id The id of the type.
	 * 
	 * @return The index in this container.
	 */
	public int getIndexFromId( Number Id)
	{
		int index = 0;

		for( Type type: types)
		{
			if( type.getId().equals( Id) == true)
			{
				return index;
			}

			index++;
		}

		return -1;
	}

	public Type getTypeFromId( ModelId Id)
	{
		return getTypeFromIndex( getIndexFromId( Id));
	}

	public int getCount()
	{
		return types.size();
	}

    public boolean validate( Configuration configuration)
    {
        return doAll( types, type -> type.validate( configuration));
    }

	@Override
	public String toString()
	{
		StringBuilder builder = new StringBuilder();

		builder.append
		(
            """
            Types
            {
            """
        );

        for( Type type: types)
        {
            builder.append( indent( type));
        }

		builder.append
		(
            """
            }
            """
        );

		return builder.toString();
	}
}
