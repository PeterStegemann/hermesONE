package net.stegemann.configuration;

import net.stegemann.configuration.type.Bool;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.Text;
import net.stegemann.configuration.type.ValueOutOfRangeException;
import net.stegemann.misc.ChangeListener;
import net.stegemann.misc.ChangeObservable;

public class PPM  extends ChangeObservable< PPM>
					implements ChangeListener< Text>
{
	public static final int PPM_CENTER_MINIMUM = -5;
	public static final int PPM_CENTER_MAXIMUM = 5;

	private final Bool ppmInverted = new Bool();
	private final Number ppmCenter = new Number( PPM_CENTER_MINIMUM, PPM_CENTER_MAXIMUM);
	private final Text name;

	private final ChannelMappings channelMappings = new ChannelMappings();

	public PPM( String name, int outputChannels)
	{
		this.name = new Text( name);
		this.name.addChangeListener( this);

		clear( outputChannels);
	}

	@Override
	public String toString()
	{
		StringBuffer Buffer = new StringBuffer();

		Buffer.append( "PPM = {\n");
		Buffer.append( " Inverted: " + ppmInverted + "\n");
		Buffer.append( " Center: " + ppmCenter + "\n");
		Buffer.append( " Name: " + name + "\n");
		Buffer.append( channelMappings);
		Buffer.append( "}\n");

		return Buffer.toString();
	}

	public void clear( int outputChannels)
	{
		try
		{
			ppmInverted.setValue( false);
			ppmCenter.setValue( 0);

			channelMappings.clear( outputChannels);
		}
		catch( ValueOutOfRangeException reason)
		{
			throw new RuntimeException( reason);
		}
	}

	public void fill( int outputChannels)
	{
		channelMappings.fill( outputChannels);
	}

	public Bool getPPMInverted()
	{
		return ppmInverted;
	}

	public Number getPPMCenter()
	{
		return ppmCenter;
	}

	public Text getName()
	{
		return name;
	}

	public ChannelMappings getChannelMappings()
	{
		return channelMappings;
	}

	@Override
	public void hasChanged( Text Object)
	{
		notifyChange( this);
	}
}
