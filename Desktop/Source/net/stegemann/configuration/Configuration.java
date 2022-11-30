package net.stegemann.configuration;

import lombok.Builder;
import lombok.Getter;
import net.stegemann.configuration.source.Sources;
import net.stegemann.configuration.util.ConfigurationField;
import net.stegemann.io.xml.Names;

@Getter
@Builder
public class Configuration
{
	@ConfigurationField( name = Names.SYSTEM)
	private final System system = new System();
	@ConfigurationField( name = Names.TYPES, itemName = Names.TYPE)
	private final Types types = new Types();
	@ConfigurationField( name = Names.MODELS, itemName = Names.MODEL)
	private final Models models = new Models();
	@ConfigurationField( name = Names.SOURCES, itemName = Names.SOURCE)
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
}
