package net.stegemann.configuration;

import net.stegemann.configuration.source.Sources;

public class Configuration
{
	private final System system = new System();
	private final Types types = new Types();
	private final Models models = new Models();
	private final Sources sources = new Sources();

	public Configuration()
	{
		clear();
	}

	@Override
	public String toString()
	{
		StringBuffer Buffer = new StringBuffer();

		Buffer.append( "Configuration = {\n");
		Buffer.append( system);
		Buffer.append( types);
		Buffer.append( models);
		Buffer.append( sources);
		Buffer.append( "}\n");

		return Buffer.toString();
	}

	public void clear()
	{
		clearSystem();
		clearModels();
	}

	public void clearModels()
	{
		types.clear();
		models.clear();
		sources.clear();
	}

	public void clearSystem()
	{
		system.clear();
	}

	public void fill()
	{
		system.fill();
		models.fillChannels( system.getOutputChannels().getValue());
	}

	public System getSystem()
	{
		return system;
	}

	public Types getTypes()
	{
		return types;
	}

	public Models getModels()
	{
		return models;
	}

	public Sources getSources()
	{
		return sources;
	}
}
