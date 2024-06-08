package net.stegemann.io.serial.configuration.read.handler.source;

import net.stegemann.configuration.source.Mix;
import net.stegemann.io.serial.base.DesktopProtocol;
import net.stegemann.io.serial.configuration.read.handler.DesktopConnectionHandler;

class MixHandler extends DesktopConnectionHandler
{
	private final Mix mix;

	public MixHandler( Mix mix)
	{
		this.mix = mix;
	}

	@Override
	public void complexOpened( DesktopProtocol.Id id)
	{
		switch( id)
		{
			case SourceMixInputs -> pushHandler( new MixInputsHandler( mix.getInputs()));

			default -> super.complexOpened( id);
		}
	}
}
