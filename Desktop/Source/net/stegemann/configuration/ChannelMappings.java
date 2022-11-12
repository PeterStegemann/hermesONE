package net.stegemann.configuration;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import net.stegemann.configuration.type.Number;

public class ChannelMappings implements Iterable< Number>
{
	private final List< Number> channelMappings = new ArrayList< Number>();

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

		// fill up missing channel mappings.
		for( int ChannelMapping = channelMappings.size(); ChannelMapping < outputChannels; ChannelMapping++)
		{
			channelMappings.add( new Number( ChannelMapping));
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

	private class ChannelMappingsIterator implements Iterator< Number>
	{
		private Iterator< Number> iterator;

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
