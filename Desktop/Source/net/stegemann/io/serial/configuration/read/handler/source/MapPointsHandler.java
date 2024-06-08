package net.stegemann.io.serial.configuration.read.handler.source;

import java.util.List;
import net.stegemann.configuration.type.SourceWithVolume;
import net.stegemann.io.serial.base.DesktopProtocol;
import net.stegemann.io.serial.configuration.read.handler.DesktopConnectionHandler;

class MapPointsHandler extends DesktopConnectionHandler
{
	private final List< SourceWithVolume> points;
	private int pointIndex = 0;

	public MapPointsHandler( List< SourceWithVolume> points)
	{
		this.points = points;
    }

	@Override
	public void complexOpened( DesktopProtocol.Id id)
	{
		switch( id)
		{
			case SourceMapPoint -> pushHandler( new SourceTupleHandler( points.get( pointIndex++)));

			default -> super.complexOpened( id);
		}
	}
}
