package net.stegemann.io.serial.configuration.read.handler;

import net.stegemann.configuration.Model;
import net.stegemann.io.serial.base.DesktopProtocol;

class StatusTimesHandler extends DesktopConnectionHandler
{
	private final Model model;
	private int statusTimeIndex;

	public StatusTimesHandler( Model model)
	{
		this.model = model;

		statusTimeIndex = 0;
	}

	@Override
	public void valueRead( DesktopProtocol.Id id, String textContent)
	{
		switch( id)
		{
			case StatusTimer :
			{
				readValue( model.getStatusTimeId( Model.StatusTime.values()[ statusTimeIndex]), textContent);

				statusTimeIndex++;
			}
			break;

			default : super.valueRead( id, textContent); break;
		}
	}
}
