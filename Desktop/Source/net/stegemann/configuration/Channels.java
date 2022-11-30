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

	public void fill( int Size)
	{
		if( Size <= channels.size())
		{
			return;
		}

		// fill up missing channels.
		int Fill = Size - channels.size();

		while( Fill > 0)
		{
			channels.add( new Channel());

			Fill--;
		}
	}

	public Channel addChannel( Channel UseChannel)
	{
		channels.add( UseChannel);

		UseChannel.addChangeListener( this);
		notifyChange( this);

		return UseChannel;
	}

	public void setChannel( int Index, Channel UseChannel)
	{
		fill( Index + 1);
	
		Channel OldChannel = channels.get( Index);

		if( OldChannel != null)
		{
			OldChannel.removeChangeListener( this);
		}

		channels.set( Index, UseChannel);	

		UseChannel.addChangeListener( this);
		notifyChange( this);
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
