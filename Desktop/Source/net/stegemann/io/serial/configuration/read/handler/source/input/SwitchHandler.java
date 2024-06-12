package net.stegemann.io.serial.configuration.read.handler.source.input;

import net.stegemann.configuration.source.input.Switch;
import net.stegemann.io.serial.base.DesktopProtocol;
import net.stegemann.io.serial.configuration.read.handler.DesktopConnectionHandler;

public class SwitchHandler extends DesktopConnectionHandler
{
	private final Switch aSwitch;

	public SwitchHandler( Switch aSwitch)
	{
		this.aSwitch = aSwitch;
	}

	@Override
	public void valueRead( DesktopProtocol.Id id, String textContent)
	{
        switch( id)
        {
            case SourceInputSwitchLowInput ->
            {
                readValue( aSwitch.getLowInputId(), textContent);
            }

            case SourceInputSwitchHighInput ->
            {
                readValue( aSwitch.getHighInputId(), textContent);
            }

            case SourceInputSwitchTop ->
            {
                readValue( aSwitch.getTop(), textContent);
            }

            case SourceInputSwitchBottom ->
            {
                readValue( aSwitch.getBottom(), textContent);
            }

            default -> super.valueRead( id, textContent);
        }
	}
}
