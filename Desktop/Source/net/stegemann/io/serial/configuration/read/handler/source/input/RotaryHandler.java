package net.stegemann.io.serial.configuration.read.handler.source.input;

import net.stegemann.configuration.source.input.Rotary;
import net.stegemann.io.serial.base.DesktopProtocol;
import net.stegemann.io.serial.configuration.read.handler.DesktopConnectionHandler;

public class RotaryHandler extends DesktopConnectionHandler
{
	private final Rotary rotary;

	public RotaryHandler( Rotary rotary)
	{
		this.rotary = rotary;
	}

	@Override
	public void valueRead( DesktopProtocol.Id id, String textContent)
	{
        switch( id)
        {
            case SourceInputRotaryAInput ->
            {
                readValue( rotary.getAInputId(), textContent);
            }

            case SourceInputRotaryBInput ->
            {
                readValue( rotary.getBInputId(), textContent);
            }

            case SourceInputRotaryStore ->
            {
                readValue( rotary.getStore(), textContent);
            }

            case SourceInputRotaryInit ->
            {
                readValue( rotary.getInit(), textContent);
            }

            case SourceInputRotaryStep ->
            {
                readValue( rotary.getStep(), textContent);
            }

            case SourceInputRotaryTop ->
            {
                readValue( rotary.getTop(), textContent);
            }

            case SourceInputRotaryBottom ->
            {
                readValue( rotary.getBottom(), textContent);
            }

            default -> super.valueRead( id, textContent);
        }
	}
}
