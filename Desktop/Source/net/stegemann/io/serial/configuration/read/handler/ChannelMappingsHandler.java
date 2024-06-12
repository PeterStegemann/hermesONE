package net.stegemann.io.serial.configuration.read.handler;

import net.stegemann.configuration.ChannelMappings;
import net.stegemann.io.serial.base.DesktopProtocol;

class ChannelMappingsHandler extends DesktopConnectionHandler
{
	private final ChannelMappings channelMappings;
	private int channelIndex = 0;

	public ChannelMappingsHandler( ChannelMappings channelMappings)
	{
		this.channelMappings = channelMappings;
    }

	@Override
	public void valueRead( DesktopProtocol.Id id, String textContent)
	{
        switch( id)
        {
            case ChannelMapping -> readValue(channelMappings.getChannelMapping(channelIndex++), textContent);

            default -> super.valueRead( id, textContent);
        }
	}
}
