package net.stegemann.controller;

import net.stegemann.configuration.source.*;
import net.stegemann.configuration.source.input.*;

public class SourceUtility
{
	private static final String[] names =
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
		String typeString = "";

		if( source instanceof Proxy)
		{
			typeString = "Proxy";
		}
		else if( source instanceof Analog)
		{
			typeString = names[ 0];
		}
		else if( source instanceof Button)
		{
			typeString = names[ 1];
		}
		else if( source instanceof Rotary)
		{
			typeString = names[ 2];
		}
		else if( source instanceof Switch)
		{
			typeString = names[ 3];
		}
		else if( source instanceof Ticker)
		{
			typeString = names[ 4];
		}
		else if( source instanceof Map)
		{
			typeString = names[ 5];
		}
		else if( source instanceof Mix)
		{
			typeString = names[ 6];
		}
		else if( source instanceof Store)
		{
			typeString = names[ 7];
		}
		else if( source instanceof Follower)
		{
			typeString = names[ 8];
		}
		else if( source instanceof Timer)
		{
			typeString = names[ 9];
		}
		else if( source instanceof Trimmer)
		{
			typeString = names[ 10];
		}

		return typeString;
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
			return new Trimmer();
		}

		return null;
	}
}
