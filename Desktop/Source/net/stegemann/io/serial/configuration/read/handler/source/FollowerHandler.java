package net.stegemann.io.serial.configuration.read.handler.source;

import net.stegemann.configuration.source.Follower;
import net.stegemann.io.serial.base.DesktopProtocol;
import net.stegemann.io.serial.configuration.read.handler.DesktopConnectionHandler;

class FollowerHandler extends DesktopConnectionHandler
{
	private final Follower follower;

	public FollowerHandler( Follower follower)
	{
		this.follower = follower;
	}

	@Override
	public void complexOpened( DesktopProtocol.Id id)
	{
		switch( id)
		{
			case SourceFollowerTarget -> pushHandler( new SourceTupleHandler( follower.getTarget()));
			case SourceFollowerStep -> pushHandler( new SourceTupleHandler( follower.getStep()));

			default -> super.complexOpened( id);
		}
	}

	@Override
	public void valueRead( DesktopProtocol.Id id, String textContent)
	{
		switch( id)
		{
			case SourceFollowerTrigger -> readValue( follower.getTrigger(), textContent);
			case SourceFollowerTriggerLowLimit -> readValue( follower.getTriggerLowLimit(), textContent);
			case SourceFollowerTriggerHighLimit -> readValue( follower.getTriggerHighLimit(), textContent);

			default -> super.valueRead( id, textContent);
		}
	}
}
