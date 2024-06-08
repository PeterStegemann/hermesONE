package net.stegemann.io.serial.configuration.read.handler.source;

import net.stegemann.configuration.source.Trimmer;
import net.stegemann.io.serial.base.DesktopProtocol;
import net.stegemann.io.serial.configuration.read.handler.DesktopConnectionHandler;

class TrimmerHandler extends DesktopConnectionHandler
{
	private final Trimmer trimmer;

	public TrimmerHandler( Trimmer trimmer)
	{
		this.trimmer = trimmer;
	}

	@Override
	public void complexOpened( DesktopProtocol.Id id)
	{
		switch( id)
		{
			case SourceTrimmerInput -> pushHandler( new SourceTupleHandler( trimmer.getInput()));
			case SourceTrimmerTrim -> pushHandler( new SourceTupleHandler( trimmer.getTrim()));
			case SourceTrimmerLimit -> pushHandler( new SourceTupleHandler( trimmer.getLimit()));
			case SourceTrimmerPoints -> pushHandler( new TrimmerPointsHandler( trimmer.getPoints()));

			default -> super.complexOpened( id);
		}
	}

	@Override
	public void valueRead( DesktopProtocol.Id id, String textContent)
	{
		switch( id)
		{
			case SourceTrimmerReverse -> readValue( trimmer.getReverse(), textContent);

			default -> super.valueRead( id, textContent);
		}
	}
}
