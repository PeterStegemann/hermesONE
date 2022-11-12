package net.stegemann.configuration.source;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import net.stegemann.configuration.Signal;
import net.stegemann.configuration.type.SourceId;
import net.stegemann.configuration.type.SourceWithVolume;
import net.stegemann.configuration.type.ValueOutOfRangeException;

public final class Map extends Source
{
	public static final int INPUT_SIGNAL_PER_VALUE = ( Signal.VALUE_RANGE / 600);
	public static final int POINT_SIGNAL_PER_VALUE = ( Signal.VALUE_RANGE / 200);

	public static final int POINTS = 7;

	private static final int DEFAULT_INPUT_VOLUME = 100;
	private static final int DEFAULT_POINT_VOLUME = 0;

	private final SourceWithVolume input;
	private final List< SourceWithVolume> points = new ArrayList< SourceWithVolume>();

	public Map()
	{
		try
		{
			input = new SourceWithVolume( Signal.MINIMUM_VALUE, Signal.MAXIMUM_VALUE,
										  INPUT_SIGNAL_PER_VALUE, DEFAULT_INPUT_VOLUME);

			for( int Count = 0; Count < POINTS; Count++)
			{
				points.add( new SourceWithVolume( Signal.MINIMUM_VALUE, Signal.MAXIMUM_VALUE,
												  POINT_SIGNAL_PER_VALUE, DEFAULT_POINT_VOLUME));
			}
		}
		catch( ValueOutOfRangeException reason)
		{
			throw new RuntimeException( reason);
		}
	}

	public Map( Map other)
	{
		super( other);

		input = new SourceWithVolume( other.input);

		for( SourceWithVolume otherPoint: other.points)
		{
			points.add( new SourceWithVolume( otherPoint));
		}
	}

	@Override
	public String toString()
	{
		StringBuffer Buffer = new StringBuffer();

		Buffer.append( "Map = {\n");
		Buffer.append( super.toString());
		Buffer.append( " Input: " + input + "\n");

		Buffer.append( " Points = {\n");

		for( SourceWithVolume CurrentSourceTupel: points)
		{
			Buffer.append( CurrentSourceTupel);
		}

		Buffer.append( " }\n");
		Buffer.append( "}\n");

		return Buffer.toString();
	}

	@Override
	public Source clone()
	{
		return new Map( this);
	}

	@Override
	public void replaceSources( HashMap< SourceId, SourceId> sourcesMap)
	{
		input.replaceSource( sourcesMap);

		for( SourceWithVolume point: points)
		{
			point.replaceSource( sourcesMap);
		}
	}

	public SourceWithVolume getInput()
	{
		return input;
	}

	public SourceWithVolume getPoint( int Index)
	{
		return points.get( Index);
	}

	public final List< SourceWithVolume> getPoints()
	{
		return points;
	}
}
