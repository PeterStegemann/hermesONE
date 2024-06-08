package net.stegemann.io.serial.configuration.read.handler;

import net.stegemann.configuration.PPM;
import net.stegemann.io.serial.base.DesktopProtocol;

class PPMHandler extends DesktopConnectionHandler
{
	private final PPM ppm;

	public PPMHandler( PPM ppm)
	{
		this.ppm = ppm;
	}

	@Override
	public void complexOpened( DesktopProtocol.Id id)
	{
		switch( id)
		{
			case ChannelMappings :
			{
				pushHandler( new ChannelMappingsHandler( ppm.getChannelMappings()));
			}
			break;

			default : super.complexOpened( id); break;
		}
	}

	@Override
	public void valueRead( DesktopProtocol.Id id, String textContent)
	{
		switch( id)
		{
			case PPMInverted :
			{
				readValue( ppm.getInverted(), textContent);
			}
			break;

			case PPMCenter :
			{
				readValue( ppm.getCenter(), textContent);
			}
			break;

			case PPMName :
			{
				readValue( ppm.getName(), textContent);
			}
			break;

			default : super.valueRead( id, textContent); break;
		}
	}
}
