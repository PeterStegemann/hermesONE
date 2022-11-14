package net.stegemann.io.serial.configuration;

import net.stegemann.misc.ObjectFactory;

public final class ConfigurationObjectFactory extends ObjectFactory
{
	public static SerialConfigurationReader serialConfigurationReader()
	{
		return singleton(() -> new SerialConfigurationReader( configurationProgress()));
	}

	public static SerialConfigurationWriter serialConfigurationWriter()
	{
		return singleton(() -> new SerialConfigurationWriter( configurationProgress()));
	}

	private static ConfigurationProgress configurationProgress()
	{
		return singleton( ConfigurationProgress::new);
	}
}
