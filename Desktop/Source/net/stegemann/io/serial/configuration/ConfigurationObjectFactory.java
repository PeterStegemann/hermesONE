package net.stegemann.io.serial.configuration;

import net.stegemann.io.serial.base.BaseObjectFactory;
import net.stegemann.io.serial.configuration.read.SerialConfigurationReader;
import net.stegemann.io.serial.configuration.write.SerialConfigurationWriter;
import net.stegemann.misc.ObjectFactory;

public final class ConfigurationObjectFactory extends ObjectFactory
{
	public static SerialConfigurationReader serialConfigurationReader()
	{
		return singleton(
			() -> new SerialConfigurationReader( configurationProgress(), BaseObjectFactory.desktopConnection()));
	}

	public static SerialConfigurationWriter serialConfigurationWriter()
	{
		return singleton(
			() -> new SerialConfigurationWriter( configurationProgress(), BaseObjectFactory.desktopConnection()));
	}

	private static ConfigurationProgress configurationProgress()
	{
		return singleton( ConfigurationProgress::new);
	}
}
