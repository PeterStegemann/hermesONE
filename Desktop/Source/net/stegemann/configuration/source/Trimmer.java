package net.stegemann.configuration.source;

import lombok.Getter;
import net.stegemann.configuration.Signal;
import net.stegemann.configuration.type.*;
import net.stegemann.configuration.util.ConfigurationField;
import net.stegemann.io.xml.Names;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

@Getter
@ConfigurationField( name = Names.SOURCE_TRIMMER)
public final class Trimmer extends Source
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

	@ConfigurationField( name = Names.SOURCE_TRIMMER_REVERSE)
	private final Bool reverse;

	@ConfigurationField( name = Names.SOURCE_TRIMMER_INPUT_SOURCE)
	private final SourceWithVolume input;
	@ConfigurationField( name = Names.SOURCE_TRIMMER_TRIM_SOURCE)
	private final SourceWithVolume trim;
	@ConfigurationField( name = Names.SOURCE_TRIMMER_LIMIT_SOURCE)
	private final SourceWithVolume limit;

	@ConfigurationField( name = Names.SOURCE_TRIMMER_POINTS, itemName = Names.SOURCE_TRIMMER_POINT)
	private final List<Volume> points = new ArrayList<>();

	public Trimmer()
	{
		reverse = new Bool();

		try
		{
			input = new SourceWithVolume( INPUT_SIGNAL_PER_VALUE, DEFAULT_INPUT_VOLUME);
			trim = new SourceWithVolume
			(
                Signal.MINIMUM_VALUE, Signal.MAXIMUM_VALUE, TRIM_SIGNAL_PER_VALUE, DEFAULT_TRIM_VOLUME
            );
			limit = new SourceWithVolume
			(
                Signal.MINIMUM_VALUE, Signal.MAXIMUM_VALUE, LIMIT_SIGNAL_PER_VALUE, DEFAULT_LIMIT_VOLUME
            );
			limit.getSourceId().setValue( Source.SOURCE_FIXED);
		}
		catch( ValueOutOfRangeException reason)
		{
			throw new RuntimeException( reason);
		}

		for( int index = 0; index < POINTS; index++)
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

	public Trimmer( Trimmer other)
	{
		super( other);

		input = new SourceWithVolume( other.input);
		trim = new SourceWithVolume( other.trim);
		limit = new SourceWithVolume( other.limit);

		reverse = new Bool( other.reverse);

		for( int index = 0; index < POINTS; index++)
		{
			points.add( new Volume( other.getPoints().get( index)));
		}
	}

	@Override
	public String toString()
	{
		StringBuffer Buffer = new StringBuffer();

		Buffer.append( "Trimmer = {\n");
		Buffer.append( super.toString());
		Buffer.append( input);
		Buffer.append( trim);
		Buffer.append( limit);
		Buffer.append( " Reverse: " + reverse + "\n");
		Buffer.append( " Points = {\n");

		for( Volume point: points)
		{
			Buffer.append( "  Point: " + point + "\n");
		}

		Buffer.append( "}\n");
		Buffer.append( "}\n");

		return Buffer.toString();
	}

	@Override
	public Source clone()
	{
		return new Trimmer( this);
	}

	@Override
	public void replaceSources( HashMap< SourceId, SourceId> sourcesMap)
	{
		input.replaceSource( sourcesMap);
		trim.replaceSource( sourcesMap);
		limit.replaceSource( sourcesMap);
	}

	public Volume getPoint( int index)
	{
		return points.get( index);
	}
}
