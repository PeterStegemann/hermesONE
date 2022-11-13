package net.stegemann.misc;

public class Utility
{
	static public String formatTime( int time)
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

	static public int parseTime( String time)
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

	static public String formatMilliSeconds( int milliSeconds)
	{
		if( milliSeconds < 0)
		{
			return "-" + -milliSeconds / 10 + "." + -milliSeconds % 10 + "ms";
		}
		else
		{
			return "" + milliSeconds / 10 + "." + milliSeconds % 10 + "ms";
		}
	}

	static public int parseMilliSeconds( String milliSeconds)
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

	static public String formatVoltage( int voltage)
	{
		return "" + voltage / 10 + "." + voltage % 10 + "V";
	}

	static public int parseVoltage( String voltage)
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
}
