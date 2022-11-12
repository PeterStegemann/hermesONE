package net.stegemann.configuration.type;

import java.util.HashMap;

public class SourceWithVolume
{
	private final SourceId sourceId;
	private final Volume volume;

	public SourceWithVolume( int minimum, int maximum, int signalPerValue)
	{
		sourceId = new SourceId();
		volume = new Volume( minimum, maximum, signalPerValue);
	}

	public SourceWithVolume( int minimum, int maximum, int signalPerValue, int value)
		throws ValueOutOfRangeException
	{
		sourceId = new SourceId();
		volume = new Volume( minimum, maximum, signalPerValue, value);
	}

	public SourceWithVolume( SourceWithVolume other)
	{
		sourceId = new SourceId( other.sourceId);
		volume = new Volume( other.volume);
	}

	@Override
	public String toString()
	{
		StringBuffer Buffer = new StringBuffer();

		Buffer.append( "SourceWithVolume = {\n");
		Buffer.append(  " SourceId: " + sourceId + "\n");
		Buffer.append(  " Volume: " + volume + "\n");
		Buffer.append( "}\n");

		return Buffer.toString();
	}

	@Override
	public int hashCode()
	{
		final int prime = 31;

		int result = 1;

		result = prime * result + sourceId.hashCode();
		result = prime * result + volume.hashCode();

		return result;
	}

	@Override
	public boolean equals( Object other)
	{
		if( this == other)
		{
			return true;
		}

		if( other == null)
		{
			return false;
		}

		if(( other instanceof SourceWithVolume) == false)
		{
			return false;
		}

		return equals(( SourceWithVolume) other);
	}

	public boolean equals( SourceWithVolume other)
	{
		if( sourceId.equals( other.sourceId) == false)
		{
			return false;
		}

		if( volume.equals( other.volume) == false)
		{
			return false;
		}

		return true;
	}

	public void replaceSource( HashMap< SourceId, SourceId> sourcesMap)
	{
		sourceId.replaceSource( sourcesMap);
	}

	public SourceId getSourceId()
	{
		return sourceId;
	}

	public Volume getVolume()
	{
		return volume;
	}
}
