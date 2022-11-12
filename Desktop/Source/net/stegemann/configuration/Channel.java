package net.stegemann.configuration;

import java.util.HashMap;

import net.stegemann.configuration.source.Source;
import net.stegemann.configuration.type.Bool;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.SourceId;
import net.stegemann.configuration.type.SourceWithVolume;
import net.stegemann.configuration.type.Text;
import net.stegemann.configuration.type.ValueOutOfRangeException;
import net.stegemann.configuration.type.Volume;
import net.stegemann.misc.ChangeListener;
import net.stegemann.misc.ChangeObservable;

public class Channel extends ChangeObservable< Channel>
				  implements ChangeListener< Text>
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

	private final Text name;

	private final SourceWithVolume input;
	private final SourceWithVolume trim;
	private final SourceWithVolume limit;

	private final Number mode;
	private final Bool reverse;

	private final Volume points[] = new Volume[ POINTS];

	public Channel()
	{
		this( "Kanal");
	}

	public Channel( String name)
	{
		this.name = new Text( name);
		this.name.addChangeListener( this);

		reverse = new Bool();

		try
		{
			input = new SourceWithVolume( Signal.MINIMUM_VALUE, Signal.MAXIMUM_VALUE,
										  INPUT_SIGNAL_PER_VALUE, DEFAULT_INPUT_VOLUME);
			trim = new SourceWithVolume( Signal.MINIMUM_VALUE, Signal.MAXIMUM_VALUE,
										 TRIM_SIGNAL_PER_VALUE, DEFAULT_TRIM_VOLUME);
			limit = new SourceWithVolume( Signal.MINIMUM_VALUE, Signal.MAXIMUM_VALUE,
										  LIMIT_SIGNAL_PER_VALUE, DEFAULT_LIMIT_VOLUME);
			limit.getSourceId().setValue( Source.SOURCE_FIXED);

			mode = new Number( MODE_WARP, MODE_CLIP, MODE_WARP);
		}
		catch( ValueOutOfRangeException reason)
		{
			throw new RuntimeException( reason);
		}

		for( int Index = 0; Index < POINTS; Index++)
		{
			points[ Index] = new Volume( Signal.MINIMUM_VALUE, Signal.MAXIMUM_VALUE,
										 POINT_SIGNAL_PER_VALUE);
		}

		try
		{
			points[ 0].setValue( -100);
			points[ 1].setValue( 0);
			points[ 2].setValue( 100);
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

		for( int Count = 0; Count < points.length; Count++)
		{
			points[ Count] = new Volume( other.points[ Count]);
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
	public void hasChanged( Text Object)
	{
		notifyChange( this);
	}

	public Text getName()
	{
		return name;
	}

	public SourceWithVolume getInput()
	{
		return input;
	}

	public SourceWithVolume getTrim()
	{
		return trim;
	}

	public SourceWithVolume getLimit()
	{
		return limit;
	}

	public Bool getReverse()
	{
		return reverse;
	}

	public Number getMode()
	{
		return mode;
	}

	public Volume getPoint( int Index)
	{
		return points[ Index];
	}
}
