package net.stegemann.io.serial.configuration.read.handler;

import net.stegemann.configuration.Model;
import net.stegemann.io.serial.base.DesktopProtocol;

class StatusSourcesHandler extends DesktopConnectionHandler
{
	private final Model model;
	private int statusSourceIndex = 0;

	public StatusSourcesHandler( Model model)
	{
		this.model = model;
    }

	@Override
	public void valueRead( DesktopProtocol.Id id, String textContent)
	{
        switch( id)
        {
            case StatusSource -> readValue
            (
                model.getStatusSourceId( Model.StatusSource.values()[ statusSourceIndex++]), textContent
            );

            default -> super.valueRead( id, textContent);
        }
	}
}
