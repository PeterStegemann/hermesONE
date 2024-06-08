package net.stegemann.io.serial.configuration.read.handler.source;

import java.util.List;
import net.stegemann.configuration.type.Volume;
import net.stegemann.io.serial.base.DesktopProtocol;
import net.stegemann.io.serial.configuration.read.handler.DesktopConnectionHandler;

class TrimmerPointsHandler extends DesktopConnectionHandler
{
	private final List< Volume> trimmerPoints;
	private int pointIndex = 0;

	public TrimmerPointsHandler( List< Volume> trimmerPoints)
	{
		this.trimmerPoints = trimmerPoints;
    }

	@Override
	public void valueRead( DesktopProtocol.Id id, String textContent)
	{
		switch( id)
		{
			case SourceTrimmerPoint -> readValue( trimmerPoints.get( pointIndex++), textContent);

			default -> super.valueRead(id, textContent);
		}
	}
}
