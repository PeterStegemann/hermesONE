package net.stegemann.io.serial.configuration.read.handler;

import net.stegemann.configuration.ChannelMappings;
import net.stegemann.io.serial.base.DesktopProtocol;

class ChannelMappingsHandler extends DesktopConnectionHandler
{
	private final ChannelMappings channelMappings;
	private int channelIndex;

	public ChannelMappingsHandler( ChannelMappings channelMappings)
	{
		this.channelMappings = channelMappings;

		channelIndex = 0;
	}

	@Override
	public void valueRead( DesktopProtocol.Id id, String textContent)
	{
		switch( id)
		{
			case ChannelMapping :
			{
				readValue( channelMappings.getChannelMapping( channelIndex++), textContent);
			}
			break;

			default : super.valueRead( id, textContent); break;
		}
	}
}
