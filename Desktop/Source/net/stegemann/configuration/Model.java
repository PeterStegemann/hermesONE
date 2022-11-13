package net.stegemann.configuration;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.SourceId;
import net.stegemann.configuration.type.Text;
import net.stegemann.configuration.type.ValueOutOfRangeException;
import net.stegemann.misc.ChangeListener;
import net.stegemann.misc.ChangeObservable;

public class Model extends ChangeObservable< Model> implements ChangeListener< Text>, Named
{
	public enum State
	{
		EMPTY,
		USED
	}

	public enum RFMode
	{
		NONE,
		MODULE_0,
		MODULE_1,
		BOTH
	}

	public enum StatusSource
	{
		LEFT_SIDE,
		LEFT_BOTTOM,
		RIGHT_SIDE,
		RIGHT_BOTTOM
	}

	public enum StatusTime
	{
		TOP,
		BOTTOM
	}

	public static final int MODEL_NONE		= 0xff;
	public static final int MODEL_GLOBAL	= MODEL_NONE - 1;
	public static final int TYPE_START		= 100;
	public static final int TYPE_END		= MODEL_GLOBAL - 1;
	public static final int MODEL_START		= 0;
	public static final int MODEL_END		= TYPE_START - 1;

	public static final int CHANNELS	= 9;
	public static final int PROXIES		= 25;

	public final static Number Global;

	private final Number id;
	private final Text name;
	private State state;
	private final Number rfMode;
	private final Number typeId;
	private final Channels channels;
	private final List< SourceId> statusSourceIds = new ArrayList<>();
	private final List< SourceId> statusTimeIds = new ArrayList<>();
	private final ProxyReferences proxyReferences;

	static
	{
		try
		{
			Global = new Number( MODEL_GLOBAL, MODEL_GLOBAL, MODEL_GLOBAL);
		}
		catch( ValueOutOfRangeException reason)
		{
			throw new RuntimeException( reason);
		}
	}

	public Model()
	{
		id = new Number( MODEL_START, MODEL_END);
		name = new Text();
		state = State.EMPTY;
		channels = new Channels( CHANNELS);
		proxyReferences = new ProxyReferences( PROXIES);

		try
		{
			rfMode = new Number( 0, RFMode.values().length - 1, RFMode.NONE.ordinal());
		}
		catch( ValueOutOfRangeException reason)
		{
			throw new RuntimeException( reason);
		}

		try
		{
			typeId = new Number( TYPE_START, TYPE_END, TYPE_START);
		}
		catch( ValueOutOfRangeException reason)
		{
			throw new RuntimeException( reason);
		}

		for( int Count = 0; Count < StatusSource.values().length; Count++)
		{
			statusSourceIds.add( new SourceId());
		}

		for( int Count = 0; Count < StatusTime.values().length; Count++)
		{
			statusTimeIds.add( new SourceId());
		}

		name.addChangeListener( this);
	}

	public Model( Model other)
	{
		id = new Number( other.id);
		name = new Text( other.name);
		state = other.state;
		rfMode = other.rfMode;
		typeId = new Number( other.typeId);
		channels = new Channels( other.channels);
		proxyReferences = new ProxyReferences( other.proxyReferences);

		for( SourceId otherSourceId: other.statusSourceIds)
		{
			statusSourceIds.add( new SourceId( otherSourceId));
		}

		for( SourceId otherSourceId: other.statusTimeIds)
		{
			statusTimeIds.add( new SourceId( otherSourceId));
		}

		name.addChangeListener( this);
	}

	@Override
	public String toString()
	{
		StringBuilder builder = new StringBuilder();

		builder.append( "Model = {\n");
		builder.append( " Id: " + id + "\n");
		builder.append( " Name: " + name + "\n");
		builder.append( " State: " + state + "\n");
		builder.append( " RFMode: " + rfMode + "\n");
		builder.append( " Type Id: " + typeId + "\n");
		builder.append( channels);
		builder.append( " Status Source Ids = {\n");
		for( SourceId StatusSourceId: statusSourceIds)
		{
			builder.append( "  Status Source Id: " + StatusSourceId + "\n");
		}
		builder.append( "}\n");
		builder.append( " Status Time Ids = {\n");
		for( SourceId StatusTimeId: statusTimeIds)
		{
			builder.append( "  Status Time Id: " + StatusTimeId + "\n");
		}
		builder.append( "}\n");
		builder.append( proxyReferences);
		builder.append( "}\n");

		return builder.toString();
	}

	@SuppressWarnings( "MethodDoesntCallSuperMethod" )
	@Override
	public Model clone()
	{
		return new Model( this);
	}

	@Override
	public void hasChanged( Text Object)
	{
		notifyChange( this);
	}

	public void fill( int size)
	{
		channels.fill( size);
	}

	public Number getId()
	{
		return id;
	}

	@Override
	public Text getName()
	{
		return name;
	}

	public void setState( State state)
	{
		this.state = state;		
	}

	public State getState()
	{
		return state;
	}

	public Number getRFMode()
	{
		return rfMode;
	}

	public Number getTypeId()
	{
		return typeId;
	}

	public Channels getChannels()
	{
		return channels;
	}

	public SourceId getStatusSourceId( StatusSource index)
	{
		return statusSourceIds.get( index.ordinal());
	}

	public SourceId getStatusTimeId( StatusTime index)
	{
		return statusTimeIds.get( index.ordinal());
	}

	public ProxyReferences getProxyReferences()
	{
		return proxyReferences;
	}

	public void replaceSources( HashMap< SourceId, SourceId> sourcesMap)
	{
		channels.replaceSources( sourcesMap);
		proxyReferences.replaceSources( sourcesMap);

		for( SourceId OldSourceId: statusSourceIds)
		{
			SourceId NewSourceId = sourcesMap.get( OldSourceId);

			if( NewSourceId != null)
			{
				try
				{
					OldSourceId.setValue( NewSourceId.getValue());
				}
				catch( ValueOutOfRangeException Reason)
				{
					throw new RuntimeException( Reason);
				}
			}
		}

		for( SourceId OldSourceId: statusTimeIds)
		{
			SourceId NewSourceId = sourcesMap.get( OldSourceId);

			if( NewSourceId != null)
			{
				try
				{
					OldSourceId.setValue( NewSourceId.getValue());
				}
				catch( ValueOutOfRangeException Reason)
				{
					throw new RuntimeException( Reason);
				}
			}
		}
	}
}
