package net.stegemann.configuration.source;

import net.stegemann.configuration.type.ModelId;import net.stegemann.configuration.type.Number;import net.stegemann.configuration.type.SourceId;import net.stegemann.configuration.type.ValueOutOfRangeException;
import net.stegemann.misc.ChangeListener;
import net.stegemann.misc.ChangeObservable;

import java.util.ArrayList;
import java.util.Comparator;import java.util.Iterator;
import java.util.List;

public class Sources extends ChangeObservable< Sources>
                  implements Iterable< Source>, ChangeListener< Source>
{
	private final List< Source> sources = new ArrayList<>();

	@Override
	public String toString()
	{
		StringBuffer Buffer = new StringBuffer();

		Buffer.append( "Sources = {\n");

		for( Source Source: sources)
		{
			Buffer.append( Source);
		}

		Buffer.append( "}\n");

		return Buffer.toString();
	}

	@Override
	public void hasChanged( Source object)
	{
		notifyChange( this);
	}

	@Override
	public Iterator< Source> iterator()
	{
		return new SourcesIterator(this);
	}

	public void clear()
	{
		sources.clear();		
	}

	public void addSource( Source source)
	{
		sources.add( source);

		source.addChangeListener( this);
		notifyChange( this);
	}

	public void insertSource( Source source)
	{
		int freeId = 0;

		// Find free source index.
		for( Source CurrentSource: sources)
		{
			if( CurrentSource.getId().equals( freeId) == false)
			{
				// Here's a gap.
				break;
			}

			freeId++;
		}

		try
		{
			source.getId().setValue( freeId);
		}
		catch( ValueOutOfRangeException reason)
		{
			throw new RuntimeException( reason);
		}

		sources.add( freeId, source);

		source.addChangeListener( this);
		notifyChange( this);
	}

	public boolean removeSource( Source source)
	{
		if( sources.remove( source) == true)
		{
			source.removeChangeListener( this);
			notifyChange( this);

			return true;
		}

		return false;
	}

    public void switchSources( SourceId sourceIdOne, SourceId sourceIdTwo)
    {
        sources.forEach( source -> source.switchSources( sourceIdOne, sourceIdTwo));

        int sourceIdOneValue = sourceIdOne.getValue();
        int sourceIdTwoValue = sourceIdTwo.getValue();

        try
        {
            sourceIdOne.setValue( sourceIdTwoValue);
            sourceIdTwo.setValue( sourceIdOneValue);
        }
        catch( ValueOutOfRangeException reason)
        {
            throw new RuntimeException( reason);
        }

        sources.sort( Comparator.comparing( Source::getId));
    }

    public void switchModels( ModelId modelIdOne, ModelId modelIdTwo)
    {
        sources.forEach( source -> source.switchModel( modelIdOne, modelIdTwo));
    }

	public Source getSourceFromIndex( int index)
	{
		if( index == -1)
		{
			return null;
		}

		try
		{
			return sources.get( index);
		}
		catch( IndexOutOfBoundsException reason)
		{
			return null;
		}
	}

	public int getIndexFromId( int id)
	{
		int Index = 0;

		for( Source CurrentSource: sources)
		{
			if( CurrentSource.getId().getValue() == id)
			{
				return Index;
			}

			Index++;
		}

		return -1;
	}

	public int getIndexFromSource( Source source)
	{
		return sources.indexOf( source);
	}

	public int getCount()
	{
		return sources.size();
	}

	private static class SourcesIterator implements Iterator< Source>
	{
		private final Iterator< Source> iterator;

		public SourcesIterator( Sources sources)
		{
			iterator = sources.sources.iterator();
		}

		@Override
		public boolean hasNext()
		{
			return iterator.hasNext();
		}

		@Override
		public Source next()
		{
			return iterator.next();
		}

		@Override
		public void remove()
		{
			iterator.remove();
		}
	}
}
