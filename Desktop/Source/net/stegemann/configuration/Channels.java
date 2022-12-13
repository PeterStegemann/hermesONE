package net.stegemann.configuration;

import net.stegemann.configuration.type.SourceId;
import net.stegemann.misc.ChangeListener;
import net.stegemann.misc.ChangeObservable;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;

public class Channels extends ChangeObservable< Channels> implements Iterable< Channel>, ChangeListener< Channel>
{
	private final List< Channel> channels = new ArrayList<>();

	public Channels( int size)
	{
		fill( size);
	}

	public Channels( Channels otherChannels)
	{
		for( Channel otherChannel: otherChannels.channels)
		{
			channels.add( new Channel( otherChannel));
		}
	}

	@Override
	public String toString()
	{
		StringBuffer Buffer = new StringBuffer();

		Buffer.append( "Channels = {\n");

		for( Channel channel: channels)
		{
			Buffer.append( channel);
		}

		Buffer.append( "}\n");

		return Buffer.toString();
	}

	@Override
	public void hasChanged( Channel channel)
	{
		notifyChange( this);
	}

	public void replaceSources( HashMap< SourceId, SourceId> sourcesMap)
	{
		for( Channel channel: channels)
		{
			channel.replaceSources( sourcesMap);
		}
	}

	@Override
	public Iterator< Channel> iterator()
	{
		return channels.iterator();
	}

	public void fill( int size)
	{
		// Fill up missing channels.
		while( size > channels.size())
		{
			addChannel();
		}
	}

	public Channel addChannel()
	{
		Channel channel = new Channel( channels.size());

		channels.add( channel);

		channel.addChangeListener( this);
		notifyChange( this);

		return channel;
	}

	public Channel addChannel( int index)
	{
		fill( index);
	
		Channel oldChannel = channels.get( index);

		if( oldChannel != null)
		{
			oldChannel.removeChangeListener( this);
		}

		Channel newChannel = new Channel( index);

		channels.set( index, newChannel);

		newChannel.addChangeListener( this);
		notifyChange( this);

		return newChannel;
	}

	public Channel getChannelFromIndex( int index)
	{
		if( index == -1)
		{
			return null;
		}

		try
		{
			return channels.get( index);
		}
		catch( IndexOutOfBoundsException ignored)
		{
			return null;
		}
	}

	public int getChannelIndexFromChannel( Channel channel)
	{
		return( channels.indexOf( channel));
	}

	public int getChannelCount()
	{
		return channels.size();
	}
}
