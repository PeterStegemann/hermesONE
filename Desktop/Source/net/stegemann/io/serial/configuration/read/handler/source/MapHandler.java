package net.stegemann.io.serial.configuration.read.handler.source;

import net.stegemann.configuration.source.Map;
import net.stegemann.io.serial.base.DesktopProtocol;
import net.stegemann.io.serial.configuration.read.handler.DesktopConnectionHandler;

class MapHandler extends DesktopConnectionHandler
{
	private final Map map;

	public MapHandler( Map map)
	{
		this.map = map;
	}

	@Override
	public void complexOpened( DesktopProtocol.Id id)
	{
        switch( id)
        {
            case SourceMapInput ->
            {
                pushHandler( new SourceTupleHandler( map.getInput()));
            }

            case SourceMapPoints ->
            {
                pushHandler( new MapPointsHandler( map.getPoints()));
            }

            default -> super.complexOpened( id);
        }
	}
}
