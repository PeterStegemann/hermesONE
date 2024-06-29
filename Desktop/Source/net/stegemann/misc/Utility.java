package net.stegemann.misc;

import java.util.Arrays;
import java.util.function.Function;
import java.util.function.Supplier;
import java.util.stream.StreamSupport;

public class Utility
{
	public static String formatTime( int time)
	{
		int seconds = time % 60;
		int minutes = time / 60;
		int hours = minutes / 60;
		minutes = minutes % 60;

		String result;

		if( hours > 0)
		{
			result = String.format( "%d:%02d:%02d", hours, minutes, seconds);
		}
		else if( minutes > 0)
		{
			result = String.format( "%d:%02d", minutes, seconds);
		}
		else
		{
			result = String.format( "%d", seconds);
		}

		return result;
	}

	public static int parseTime( String time)
	{
		String[] valueArray = time.split( ":");

		if( valueArray.length == 0)
		{
			throw new NumberFormatException( "No parse able number.");
		}

		int seconds = 0;

		for( String part: valueArray)
		{
			seconds *= 60;
			seconds += Integer.parseInt( part);
		}

		return seconds;
	}

	public static String formatMilliSeconds( int milliSeconds)
	{
		if( milliSeconds < 0)
		{
			return "-" + -milliSeconds / 10 + "." + -milliSeconds % 10 + "ms";
		}
		else
		{
			return milliSeconds / 10 + "." + milliSeconds % 10 + "ms";
		}
	}

	public static int parseMilliSeconds( String milliSeconds)
	{
		String[] valueArray = milliSeconds.split( "ms");

		if( valueArray.length == 1)
		{
			valueArray = valueArray[ 0].split( "ms");
		}

		if( valueArray.length == 0)
		{
			throw new NumberFormatException( "No parse able number.");
		}

		return ( int)( Float.parseFloat( valueArray[ 0]) * 10);
	}

	public static String formatVoltage( int voltage)
	{
		return voltage / 10 + "." + voltage % 10 + "V";
	}

	public static int parseVoltage( String voltage)
	{
		String[] valueArray = voltage.split( "V");

		if( valueArray.length == 1)
		{
			valueArray = valueArray[ 0].split( "v");
		}

		if( valueArray.length == 0)
		{
			throw new NumberFormatException( "No parse able number.");
		}

		return ( int)( Float.parseFloat( valueArray[ 0]) * 10);
	}

	public static String indent( Object object)
	{
		return object.toString().indent(4).trim();
	}

    public static String toString( Iterable< ?> items)
    {
        StringBuilder builder = new StringBuilder();

		for( Object item: items)
		{
			builder.append( indent( item));
		}

        return builder.toString();
    }

	public static < Type, IterableType extends Iterable< Type>, ExceptionType extends Exception>
		void forEach( IterableType iterable, ThrowingConsumer< Type, ExceptionType> consumer)
			throws ExceptionType
	{
		for( Type item : iterable)
		{
			consumer.accept( item);
		}
	}

    public static < T> Boolean doAll( Iterable< T> items, Function< T, Boolean> function)
    {
        return StreamSupport.stream(items.spliterator(), false)
            .map( function).reduce(( a,b) -> a && b)
            .orElse(true);
    }

    @SafeVarargs
    public static boolean doAll( Supplier< Boolean> ... suppliers)
    {
        return Arrays.stream( suppliers).map( Supplier::get).reduce(( a,b) -> a && b).orElse(true);
    }
}
