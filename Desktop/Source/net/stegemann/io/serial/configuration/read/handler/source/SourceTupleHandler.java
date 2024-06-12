package net.stegemann.io.serial.configuration.read.handler.source;

import net.stegemann.configuration.type.SourceWithVolume;
import net.stegemann.io.serial.base.DesktopProtocol;
import net.stegemann.io.serial.configuration.read.handler.DesktopConnectionHandler;

public class SourceTupleHandler extends DesktopConnectionHandler
{
	private final SourceWithVolume sourceTuple;

	public SourceTupleHandler( SourceWithVolume sourceTuple)
	{
		this.sourceTuple = sourceTuple;
	}

	@Override
	public void valueRead( DesktopProtocol.Id id, String textContent)
	{
        switch( id)
        {
            case SourceTupleSource -> readValue( sourceTuple.getSourceId(), textContent);
            case SourceTupleVolume -> readValue( sourceTuple.getVolume(), textContent);

            default -> super.valueRead( id, textContent);
        }
	}
}
