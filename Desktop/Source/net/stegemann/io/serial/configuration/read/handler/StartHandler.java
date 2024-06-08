package net.stegemann.io.serial.configuration.read.handler;

import net.stegemann.configuration.Configuration;
import net.stegemann.io.serial.base.DesktopProtocol;

public class StartHandler extends DesktopConnectionHandler
{
	final Configuration configuration;

	public StartHandler( Configuration configuration)
	{
		this.configuration = configuration;
	}

	@Override
	public void complexOpened( DesktopProtocol.Id id)
	{
		switch( id)
		{
			case Configuration -> pushHandler( new ConfigurationHandler( configuration));

			default -> super.complexOpened( id);
		}
	}
}
