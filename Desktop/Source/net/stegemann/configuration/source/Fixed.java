package net.stegemann.configuration.source;

import java.util.HashMap;

import net.stegemann.configuration.type.SourceId;
import net.stegemann.configuration.type.ValueOutOfRangeException;

public final class Fixed extends Source
{
	public Fixed()
	{
		try
		{
			getId().setValue( Source.SOURCE_FIXED);
		}
		catch( ValueOutOfRangeException e) {}

		getName().setValue( "<Fest>");
	}

	public Fixed( Fixed other)
	{
		super( other);
	}

	@Override
	public String toString()
	{
		StringBuffer Buffer = new StringBuffer();

		Buffer.append( "Fixed = {\n");
		Buffer.append( super.toString());
		Buffer.append( "}\n");

		return Buffer.toString();
	}

	@Override
	public Source clone()
	{
		return new Fixed( this);
	}

	@Override
	public void replaceSources( HashMap< SourceId, SourceId> sourcesMap)
	{
	}

	@Override
	public int compareTo( Source Other)
	{
		if( Other instanceof Empty)
		{
			return 1;
		}

		if( Other instanceof Fixed)
		{
			return 0;
		}

		return -1;
	}
}
