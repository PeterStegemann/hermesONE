package net.stegemann.io.serial.configuration.read.handler.source;

import net.stegemann.configuration.source.Sources;
import net.stegemann.io.serial.base.DesktopProtocol;
import net.stegemann.io.serial.configuration.read.handler.DesktopConnectionHandler;

public class SourcesHandler extends DesktopConnectionHandler
{
	private final Sources sources;

	private int sourceId = 0;

	public SourcesHandler( Sources sources)
	{
		this.sources = sources;
    }

	@Override
	public void complexOpened( DesktopProtocol.Id id)
	{
		switch( id)
		{
			case Source -> pushHandler( new SourceHandler( sources, sourceId++));

			default -> super.complexOpened( id);
		}
	}
}
