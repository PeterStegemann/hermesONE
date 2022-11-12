package net.stegemann.misc;

public class Utility
{
	static public String formatTime( int time)
	{
		int Seconds = time % 60;
		int Minutes = time / 60;
		int Hours = Minutes / 60;
		Minutes = Minutes % 60;

		String Result;

		if( Hours > 0)
		{
			Result = String.format( "%d:%02d:%02d", Integer.valueOf( Hours),
													Integer.valueOf( Minutes),
													Integer.valueOf(Seconds));
		}
		else if( Minutes > 0)
		{
			Result = String.format( "%d:%02d", Integer.valueOf( Minutes),
											   Integer.valueOf( Seconds));
		}
		else
		{
			Result = String.format( "%d", Integer.valueOf( Seconds));
		}

		return Result;
	}

	static public int parseTime( String time)
	{
		String[] ValueArray = time.split( ":");

		if( ValueArray.length == 0)
		{
			throw new NumberFormatException( "No parseable number.");
		}

		int Seconds = 0;

		for( String Part: ValueArray)
		{
			Seconds *= 60;
			Seconds += Integer.parseInt( Part);
		}

		return Seconds;
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
		String[] ValueArray = milliSeconds.split( "ms");

		if( ValueArray.length == 1)
		{
			ValueArray = ValueArray[ 0].split( "ms");
		}

		if( ValueArray.length == 0)
		{
			throw new NumberFormatException( "No parseable number.");
		}

		return ( int)( Float.parseFloat( ValueArray[ 0]) * 10);
	}

	static public String formatVoltage( int voltage)
	{
		return "" + voltage / 10 + "." + voltage % 10 + "V";
	}

	static public int parseVoltage( String voltage)
	{
		String[] ValueArray = voltage.split( "V");

		if( ValueArray.length == 1)
		{
			ValueArray = ValueArray[ 0].split( "v");
		}

		if( ValueArray.length == 0)
		{
			throw new NumberFormatException( "No parseable number.");
		}

		return ( int)( Float.parseFloat( ValueArray[ 0]) * 10);
	}
}
