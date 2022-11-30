package net.stegemann.configuration.source;

import net.stegemann.configuration.type.SourceId;
import net.stegemann.configuration.type.ValueOutOfRangeException;
import net.stegemann.configuration.util.ConfigurationField;

import java.util.HashMap;

@ConfigurationField( ignore = true)
public final class Fixed extends Source
{
	public Fixed()
	{
		try
		{
			getId().setValue( Source.SOURCE_FIXED);
		}
		catch( ValueOutOfRangeException ignored) {}

		getName().setValue( "<Fest>");
	}

	public Fixed( Fixed other)
	{
		super( other);
	}

	@Override
	public String toString()
	{
		return String.format( """
				Fixed
				{
					%s
				}
				""",
				super.toString());
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
	public int compareTo( Source other)
	{
		if( other instanceof Empty)
		{
			return 1;
		}

		if( other instanceof Fixed)
		{
			return 0;
		}

		return -1;
	}
}
