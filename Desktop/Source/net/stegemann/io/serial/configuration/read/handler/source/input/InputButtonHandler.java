package net.stegemann.io.serial.configuration.read.handler.source.input;

import net.stegemann.configuration.source.input.Button;
import net.stegemann.io.serial.base.DesktopProtocol;
import net.stegemann.io.serial.configuration.read.handler.DesktopConnectionHandler;

public class InputButtonHandler extends DesktopConnectionHandler
{
	private final Button button;

	public InputButtonHandler( Button button)
	{
		this.button = button;
	}

	@Override
	public void valueRead( DesktopProtocol.Id id, String textContent)
	{
        switch( id)
        {
            case SourceInputButtonInput ->
            {
                readValue( button.getInputId(), textContent);
            }

            case SourceInputButtonInit ->
            {
                readValue( button.getInit(), textContent);
            }

            case SourceInputButtonStore ->
            {
                readValue( button.getStore(), textContent);
            }

            case SourceInputButtonToggle ->
            {
                readValue( button.getToggle(), textContent);
            }

            case SourceInputButtonTop ->
            {
                readValue( button.getTop(), textContent);
            }

            case SourceInputButtonBottom ->
            {
                readValue( button.getBottom(), textContent);
            }

            default -> super.valueRead( id, textContent);
        }
	}
}
