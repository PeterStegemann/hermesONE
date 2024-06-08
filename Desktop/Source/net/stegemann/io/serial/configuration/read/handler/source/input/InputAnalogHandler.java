package net.stegemann.io.serial.configuration.read.handler.source.input;

import net.stegemann.configuration.source.input.Analog;
import net.stegemann.io.serial.base.DesktopProtocol;
import net.stegemann.io.serial.configuration.read.handler.DesktopConnectionHandler;

public class InputAnalogHandler extends DesktopConnectionHandler
{
	private final Analog analog;

	public InputAnalogHandler( Analog analog)
	{
		this.analog = analog;
	}

	@Override
	public void valueRead( DesktopProtocol.Id id, String textContent)
	{
        switch( id)
        {
            case SourceInputAnalogInput ->
            {
                readValue( analog.getInputId(), textContent);
            }

            default -> super.valueRead( id, textContent);
        }
	}
}
