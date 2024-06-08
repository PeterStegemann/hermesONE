package net.stegemann.io.serial.configuration.read.handler;

import net.stegemann.configuration.Channel;
import net.stegemann.configuration.Channels;
import net.stegemann.io.serial.base.DesktopProtocol;

class ChannelsHandler extends DesktopConnectionHandler
{
	private final Channels channels;
	private int channelIndex;

	public ChannelsHandler( Channels channels)
	{
		this.channels = channels;

		channelIndex = 0;
	}

	@Override
	public void complexOpened( DesktopProtocol.Id id)
	{
		switch( id)
		{
			case Channel ->
			{
				Channel channel = channels.addChannel( channelIndex++);

				pushHandler( new ChannelHandler( channel));
			}

			default -> super.complexOpened( id);
		}
	}
}
