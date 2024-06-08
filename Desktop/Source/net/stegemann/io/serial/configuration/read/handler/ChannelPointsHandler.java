package net.stegemann.io.serial.configuration.read.handler;

import net.stegemann.configuration.Channel;
import net.stegemann.io.serial.base.DesktopProtocol;

class ChannelPointsHandler extends DesktopConnectionHandler
{
	private final Channel channel;
	private int channelPointIndex;

	public ChannelPointsHandler( Channel channel)
	{
		this.channel = channel;

		channelPointIndex = 0;
	}

	@Override
	public void valueRead( DesktopProtocol.Id id, String textContent)
	{
		switch( id)
		{
			case ChannelPoint :
			{
				readValue( channel.getPoint( channelPointIndex++), textContent);
			}
			break;

			default : super.valueRead( id, textContent); break;
		}
	}
}
