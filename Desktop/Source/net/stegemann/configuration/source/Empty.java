package net.stegemann.configuration.source;

import net.stegemann.configuration.type.SourceId;
import net.stegemann.configuration.type.ValueOutOfRangeException;
import net.stegemann.configuration.util.ConfigurationField;

import java.util.HashMap;

@ConfigurationField( ignore = true)
public final class Empty extends Source
{
	public Empty()
	{
		try
		{
			getId().setValue( Source.SOURCE_NONE);
		}
		catch( ValueOutOfRangeException ignored) {}

		getName().setValue( "<Leer>");
	}

	public Empty( Empty other)
	{
		super( other);
	}

	@Override
	public String toString()
	{
		return String.format( """
				Empty
				{
					%s
				}
				""",
				super.toString());
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
