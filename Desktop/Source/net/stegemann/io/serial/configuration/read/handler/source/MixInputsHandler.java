package net.stegemann.io.serial.configuration.read.handler.source;

import java.util.List;
import net.stegemann.configuration.type.SourceWithVolume;
import net.stegemann.io.serial.base.DesktopProtocol;
import net.stegemann.io.serial.configuration.read.handler.DesktopConnectionHandler;

class MixInputsHandler extends DesktopConnectionHandler
{
	private final List< SourceWithVolume> mixInputs;
	private int inputIndex = 0;

	public MixInputsHandler( List< SourceWithVolume> mixInputs)
	{
		this.mixInputs = mixInputs;
    }

	@Override
	public void complexOpened( DesktopProtocol.Id id)
	{
		switch( id)
		{
			case SourceMixInput -> pushHandler( new SourceTupleHandler( mixInputs.get( inputIndex++)));

			default -> super.complexOpened( id);
		}
	}
}
