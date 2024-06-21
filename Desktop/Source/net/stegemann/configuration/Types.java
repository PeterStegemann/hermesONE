package net.stegemann.configuration;

import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.ValueOutOfRangeException;
import net.stegemann.misc.ChangeListener;
import net.stegemann.misc.ChangeObservable;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

public class Types extends ChangeObservable< Types>
                implements Iterable< Type>, ChangeListener< Type>
{
	private final List< Type> types = new ArrayList<>();

	@Override
	public String toString()
	{
		StringBuffer Buffer = new StringBuffer();

		Buffer.append( "Types = {\n");

		for( Type CurrentType: types)
		{
			Buffer.append( CurrentType);
		}

		Buffer.append( "}\n");

		return Buffer.toString();
	}

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

	public Number getTypeIdFromIndex( int index)
	{
		Type CurrentType = getTypeFromIndex( index);

		if( CurrentType == null)
		{
			return null;
		}

		return CurrentType.getId();
	}

	public Type getTypeFromId( Number id)
	{
		return getTypeFromIndex( getIndexFromId( id));
	}

	/** Get the index of a type in this container from its id.
	 * 
	 * @param Id The id of the type.
	 * 
	 * @return The index in this container.
	 */
	public int getIndexFromId( Number Id)
	{
		int Index = 0;

		for( Type CurrentType: types)
		{
			if( CurrentType.getId().equals( Id) == true)
			{
				return Index;
			}

			Index++;
		}

		return -1;
	}

	public int getCount()
	{
		return types.size();
	}
}
