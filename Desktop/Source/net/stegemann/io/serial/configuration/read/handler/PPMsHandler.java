package net.stegemann.io.serial.configuration.read.handler;

import net.stegemann.configuration.PPMs;
import net.stegemann.io.serial.base.DesktopProtocol;

class PPMsHandler extends DesktopConnectionHandler
{
	private final PPMs ppms;
	private int ppmIndex;

	public PPMsHandler( PPMs ppms)
	{
		this.ppms = ppms;

		ppmIndex = 0;
	}

	@Override
	public void complexOpened( DesktopProtocol.Id id)
	{
		switch( id)
		{
			case PPM :
			{
				pushHandler( new PPMHandler( ppms.getPPM( ppmIndex++)));
			}
			break;

			default : super.complexOpened( id); break;
		}
	}
}
