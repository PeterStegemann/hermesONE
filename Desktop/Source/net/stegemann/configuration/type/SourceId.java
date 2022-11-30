package net.stegemann.configuration.type;

import net.stegemann.configuration.source.Source;

import java.util.HashMap;

public class SourceId extends Number
{
	public SourceId()
	{
		super( Source.SOURCE_START, Source.SOURCE_NONE);

		try
		{
			setValue( Source.SOURCE_NONE);
		}
		catch( ValueOutOfRangeException reason)
		{
			// No exception possible here.
			throw new RuntimeException( reason);
		}
	}

	public SourceId( int value)
		throws ValueOutOfRangeException
	{
		super( Source.SOURCE_START, Source.SOURCE_NONE, value);
	}

	public SourceId( SourceId other)
	{
		super( other);
	}

	@Override
	public String toString()
	{
		StringBuffer Buffer = new StringBuffer();

		Buffer.append( "SourceId = {\n");
		Buffer.append( super.toString());
		Buffer.append( "}\n");

		return Buffer.toString();
	}

	public void replaceSource( HashMap< SourceId, SourceId> sourcesMap)
	{
		SourceId NewSourceId = sourcesMap.get( this);

		if( NewSourceId != null)
		{
			try
			{
				setValue( NewSourceId.getValue());
			}
			catch( ValueOutOfRangeException reason)
			{
				throw new RuntimeException( reason);
			}
		}
	}
}
