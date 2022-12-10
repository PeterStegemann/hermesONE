package net.stegemann.io.xml;

import net.stegemann.misc.ObjectFactory;

public final class XMLObjectFactory extends ObjectFactory
{
	public static XMLReader xmlReader()
	{
		return singleton(() -> new XMLReader( documentGenerator()));
	}

	public static XMLWriter xmlWriter()
	{
		return singleton(() -> new XMLWriter( documentGenerator()));
	}

	private static DocumentGenerator documentGenerator()
	{
		return singleton( DocumentGenerator::new);
	}
}
