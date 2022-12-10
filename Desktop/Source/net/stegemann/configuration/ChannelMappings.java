package net.stegemann.configuration;

import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.ValueOutOfRangeException;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

public class ChannelMappings implements Iterable< Number>
{
	public static final int MINIMUM_VALUE = 0;
	public static final int MAXIMUM_VALUE = 15;

	private final List< Number> channelMappings = new ArrayList<>();

	@Override
	public String toString()
	{
		StringBuffer Buffer = new StringBuffer();

		Buffer.append( "ChannelMappings = {\n");

		for( Number ChannelMapping: channelMappings)
		{
			Buffer.append( ChannelMapping);
		}

		Buffer.append( "}\n");

		return Buffer.toString();
	}

	public void clear( int outputChannels)
	{
		channelMappings.clear();

		fill( outputChannels);
	}

	public void fill( int outputChannels)
	{
		if( outputChannels <= channelMappings.size())
		{
			return;
		}

		// Fill up missing channel mappings.
		for( int channelMapping = channelMappings.size(); channelMapping < outputChannels; channelMapping++)
		{
			try
			{
				channelMappings.add(
						new Number( ChannelMappings.MINIMUM_VALUE, ChannelMappings.MAXIMUM_VALUE, channelMapping));
			}
			catch( ValueOutOfRangeException ignored) {}
		}
	}

	public void addChannelMapping( Number channelMapping)
	{
		channelMappings.add( channelMapping);
	}

	public void setChannelMapping( int index, Number channelMapping)
	{
		fill( index + 1);

		channelMappings.set( index, channelMapping);
	}

	public Number getChannelMapping( int index)
	{
		if( index == -1)
		{
			return null;
		}

		return channelMappings.get( index);
	}

	public int getChannelMappingCount()
	{
		return channelMappings.size();
	}

	private static class ChannelMappingsIterator implements Iterator< Number>
	{
		private final Iterator< Number> iterator;

		public ChannelMappingsIterator( ChannelMappings channelMappings)
		{
			iterator = channelMappings.channelMappings.iterator();
		}

		@Override
		public boolean hasNext()
		{
			return iterator.hasNext();
		}

		@Override
		public Number next()
		{
			return iterator.next();
		}

		@Override
		public void remove()
		{
			iterator.remove();
		}
	}

	@Override
	public Iterator< Number> iterator()
	{
		return new ChannelMappingsIterator( this);
	}
}
