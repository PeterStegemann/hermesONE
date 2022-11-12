package net.stegemann.gui.misc;

import net.stegemann.configuration.source.Follower;
import net.stegemann.configuration.source.Map;
import net.stegemann.configuration.source.Mix;
import net.stegemann.configuration.source.Proxy;
import net.stegemann.configuration.source.Source;
import net.stegemann.configuration.source.Store;
import net.stegemann.configuration.source.Timer;
import net.stegemann.configuration.source.Trim;
import net.stegemann.configuration.source.input.Analog;
import net.stegemann.configuration.source.input.Button;
import net.stegemann.configuration.source.input.Rotary;
import net.stegemann.configuration.source.input.Switch;
import net.stegemann.configuration.source.input.Ticker;

public class SourceUtility
{
	private static String[] names =
	{
		"Geber Analog",
		"Geber Taster",
		"Geber Drehgeber",
		"Geber Schalter",
		"Geber Ticker",
		"Kurve",
		"Mischer",
		"Speicher",
		"Folger",
		"Uhr",
		"Trimmer"
	};

	public static String[] getSelectableTypeNames()
	{
		return names;
	}

	public static String getTypeNameForSource( Source source)
	{
		String TypeString = "";

		if( source instanceof Proxy)
		{
			TypeString = "Proxy";
		}
		else if( source instanceof Analog)
		{
			TypeString = names[ 0];
		}
		else if( source instanceof Button)
		{
			TypeString = names[ 1];
		}
		else if( source instanceof Rotary)
		{
			TypeString = names[ 2];
		}
		else if( source instanceof Switch)
		{
			TypeString = names[ 3];
		}
		else if( source instanceof Ticker)
		{
			TypeString = names[ 4];
		}
		else if( source instanceof Map)
		{
			TypeString = names[ 5];
		}
		else if( source instanceof Mix)
		{
			TypeString = names[ 6];
		}
		else if( source instanceof Store)
		{
			TypeString = names[ 7];
		}
		else if( source instanceof Follower)
		{
			TypeString = names[ 8];
		}
		else if( source instanceof Timer)
		{
			TypeString = names[ 9];
		}
		else if( source instanceof Trim)
		{
			TypeString = names[ 10];
		}

		return TypeString;
	}

	public static Source createSourceForTypeName( String typeName)
	{
		if( typeName == null)
		{
			return null;
		}

		if( "Proxy".compareToIgnoreCase( typeName) == 0)
		{
			return new Proxy();
		}
		else if( names[ 0].compareToIgnoreCase( typeName) == 0)
		{
			return new Analog();
		}
		else if( names[ 1].compareToIgnoreCase( typeName) == 0)
		{
			return new Button();
		}
		else if( names[ 2].compareToIgnoreCase( typeName) == 0)
		{
			return new Rotary();
		}
		else if( names[ 3].compareToIgnoreCase( typeName) == 0)
		{
			return new Switch();
		}
		else if( names[ 4].compareToIgnoreCase( typeName) == 0)
		{
			return new Ticker();
		}
		else if( names[ 5].compareToIgnoreCase( typeName) == 0)
		{
			return new Map();
		}
		else if( names[ 6].compareToIgnoreCase( typeName) == 0)
		{
			return new Mix();
		}
		else if( names[ 7].compareToIgnoreCase( typeName) == 0)
		{
			return new Store();
		}
		else if( names[ 8].compareToIgnoreCase( typeName) == 0)
		{
			return new Follower();
		}
		else if( names[ 9].compareToIgnoreCase( typeName) == 0)
		{
			return new Timer();
		}
		else if( names[ 10].compareToIgnoreCase( typeName) == 0)
		{
			return new Trim();
		}

		return null;
	}
}
