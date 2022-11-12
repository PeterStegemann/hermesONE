package net.stegemann.configuration.source;

import java.util.HashMap;

import net.stegemann.configuration.type.SourceId;
import net.stegemann.configuration.type.ValueOutOfRangeException;

public final class Empty extends Source
{
	public Empty()
	{
		try
		{
			getId().setValue( Source.SOURCE_NONE);
		}
		catch( ValueOutOfRangeException e) {}

		getName().setValue( "<Leer>");
	}

	public Empty( Empty other)
	{
		super( other);
	}

	@Override
	public String toString()
	{
		StringBuffer Buffer = new StringBuffer();

		Buffer.append( "Empty = {\n");
		Buffer.append( super.toString());
		Buffer.append( "}\n");

		return Buffer.toString();
	}

	@Override
	public Source clone()
	{
		return new Empty( this);
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
			return 0;
		}

		return -1;
	}
}
