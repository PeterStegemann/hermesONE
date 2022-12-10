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

	public Channels( Channels UseChannels)
	{
		for( Channel otherChannel: UseChannels.channels)
		{
			channels.add( new Channel( otherChannel));
		}
	}

	@Override
	public String toString()
	{
		StringBuffer Buffer = new StringBuffer();

		Buffer.append( "Channels = {\n");

		for( Channel Channel: channels)
		{
			Buffer.append( Channel);
		}

		Buffer.append( "}\n");

		return Buffer.toString();
	}

	@Override
	public void hasChanged( Channel UseChannel)
	{
		notifyChange( this);
	}

	public void replaceSources( HashMap< SourceId, SourceId> SourcesMap)
	{
		for( Channel Channel: channels)
		{
			Channel.replaceSources( SourcesMap);
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

	public Channel getChannelFromIndex( int Index)
	{
		if( Index == -1)
		{
			return null;
		}

		try
		{
			return channels.get( Index);
		}
		catch( IndexOutOfBoundsException Reason)
		{
			return null;
		}
	}

	public int getChannelIndexFromChannel( Channel UseChannel)
	{
		return( channels.indexOf( UseChannel));
	}

	public int getChannelCount()
	{
		return channels.size();
	}
}
