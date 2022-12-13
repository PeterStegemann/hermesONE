package net.stegemann.configuration;

import lombok.Getter;
import net.stegemann.configuration.source.Source;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.*;
import net.stegemann.configuration.util.ConfigurationField;
import net.stegemann.io.xml.Names;
import net.stegemann.misc.ChangeListener;
import net.stegemann.misc.ChangeObservable;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

@Getter
public class Channel extends ChangeObservable< Channel> implements ChangeListener< Text>
{
	public static final int INPUT_SIGNAL_PER_VALUE = ( Signal.VALUE_RANGE / 600);
	public static final int TRIM_SIGNAL_PER_VALUE = ( Signal.VALUE_RANGE / 600);
	public static final int LIMIT_SIGNAL_PER_VALUE = ( Signal.VALUE_RANGE / 600);
	public static final int POINT_SIGNAL_PER_VALUE = ( Signal.VALUE_RANGE / 200);

	public static final int POINTS = 3;

	private static final int DEFAULT_INPUT_VOLUME = 100;
	private static final int DEFAULT_TRIM_VOLUME = 25;
	private static final int DEFAULT_LIMIT_VOLUME = 100;

	public static final int MODE_WARP = 0;
	public static final int MODE_CLIP = 1;

	@ConfigurationField( name = Names.CHANNEL_NAME)
	private final Text name;

	@ConfigurationField( name = Names.CHANNEL_REVERSE)
	private final Bool reverse;
	@ConfigurationField( name = Names.CHANNEL_MODE)
	private final Number mode;

	@ConfigurationField( name = Names.CHANNEL_INPUT)
	private final SourceWithVolume input;
	@ConfigurationField( name = Names.CHANNEL_TRIM)
	private final SourceWithVolume trim;
	@ConfigurationField( name = Names.CHANNEL_LIMIT)
	private final SourceWithVolume limit;

	@ConfigurationField( name = Names.CHANNEL_POINTS, itemName = Names.CHANNEL_POINT)
	private final List< Volume> points = new ArrayList<>();

	public Channel( int id)
	{
		name = new Text( "Kanal " + ( id + 1));
		name.addChangeListener( this);

		reverse = new Bool();

		try
		{
			input = new SourceWithVolume( INPUT_SIGNAL_PER_VALUE, DEFAULT_INPUT_VOLUME);
			trim = new SourceWithVolume( TRIM_SIGNAL_PER_VALUE, DEFAULT_TRIM_VOLUME);
			limit = new SourceWithVolume( LIMIT_SIGNAL_PER_VALUE, DEFAULT_LIMIT_VOLUME);
			limit.getSourceId().setValue( Source.SOURCE_FIXED);

			mode = new Number( MODE_WARP, MODE_CLIP, MODE_WARP);
		}
		catch( ValueOutOfRangeException reason)
		{
			throw new RuntimeException( reason);
		}

		for( int Index = 0; Index < POINTS; Index++)
		{
			points.add( new Volume( Signal.MINIMUM_VALUE, Signal.MAXIMUM_VALUE, POINT_SIGNAL_PER_VALUE));
		}

		try
		{
			points.get( 0).setValue( -100);
			points.get( 1).setValue( 0);
			points.get( 2).setValue( 100);
		}
		catch( ValueOutOfRangeException reason)
		{
			throw new RuntimeException( reason);
		}
	}

	public Channel( Channel other)
	{
		name = new Text( other.name);
		name.addChangeListener( this);

		input = new SourceWithVolume( other.input);
		trim = new SourceWithVolume( other.trim);
		limit = new SourceWithVolume( other.limit);

		mode = new Number( other.mode);
		reverse = new Bool( other.reverse);

		for( Volume otherPoint: other.points)
		{
			points.add( new Volume( otherPoint));
		}
	}

	@Override
	public String toString()
	{
		StringBuffer Buffer = new StringBuffer();

		Buffer.append( "Channel = {\n");
		Buffer.append( " Name: " + name + "\n");
		Buffer.append( input);
		Buffer.append( trim);
		Buffer.append( limit);
		Buffer.append( " Mode: " + mode + "\n");
		Buffer.append( " Reverse: " + reverse + "\n");
		Buffer.append( " Points = {\n");

		for( Volume Point: points)
		{
			Buffer.append( "  Point: " + Point + "\n");
		}

		Buffer.append( "}\n");
		Buffer.append( "}\n");

		return Buffer.toString();
	}

	public void replaceSources( HashMap< SourceId, SourceId> sourcesMap)
	{
		input.replaceSource( sourcesMap);
		trim.replaceSource( sourcesMap);
		limit.replaceSource( sourcesMap);
	}

	@Override
	public void hasChanged( Text object)
	{
		notifyChange( this);
	}

	public Volume getPoint( int index)
	{
		return points.get( index);
	}
}
