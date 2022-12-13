package net.stegemann.configuration;

import lombok.Getter;
import net.stegemann.configuration.type.Bool;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.Text;
import net.stegemann.configuration.type.ValueOutOfRangeException;
import net.stegemann.configuration.util.ConfigurationField;
import net.stegemann.io.xml.Names;
import net.stegemann.misc.ChangeListener;
import net.stegemann.misc.ChangeObservable;

@Getter
public class PPM extends ChangeObservable< PPM> implements ChangeListener< Text>
{
	public static final int PPM_CENTER_MINIMUM = -5;
	public static final int PPM_CENTER_MAXIMUM = 5;

	@ConfigurationField( name = Names.PPM_INVERTED)
	private final Bool inverted = new Bool();
	@ConfigurationField( name = Names.PPM_CENTER)
	private final Number center = new Number( PPM_CENTER_MINIMUM, PPM_CENTER_MAXIMUM);
	@ConfigurationField( name = Names.PPM_NAME)
	private final Text name;

	@ConfigurationField( name = Names.PPM_CHANNEL_MAPPINGS, itemName = Names.PPM_CHANNEL_MAPPING)
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
		Buffer.append( " Inverted: " + inverted + "\n");
		Buffer.append( " Center: " + center + "\n");
		Buffer.append( " Name: " + name + "\n");
		Buffer.append( channelMappings);
		Buffer.append( "}\n");

		return Buffer.toString();
	}

	public void clear( int outputChannels)
	{
		try
		{
			inverted.setValue( false);
			center.setValue( 0);

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

	@Override
	public void hasChanged( Text object)
	{
		notifyChange( this);
	}
}
