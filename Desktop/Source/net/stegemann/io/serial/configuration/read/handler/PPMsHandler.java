package net.stegemann.io.serial.configuration.read.handler;

import net.stegemann.configuration.PPMs;
import net.stegemann.io.serial.base.DesktopProtocol;

class PPMsHandler extends DesktopConnectionHandler
{
	private final PPMs ppms;
	private int ppmIndex = 0;

	public PPMsHandler( PPMs ppms)
	{
		this.ppms = ppms;
    }

	@Override
	public void complexOpened( DesktopProtocol.Id id)
	{
        switch( id)
        {
            case PPM -> pushHandler( new PPMHandler( ppms.getPPM( ppmIndex++)));

            default -> super.complexOpened( id);
        }
	}
}
