package net.stegemann.io.serial.configuration.read.handler;

import net.stegemann.configuration.type.Volume;import net.stegemann.io.serial.base.DesktopProtocol;import java.util.List;

class ChannelPointsHandler extends DesktopConnectionHandler
{
    private final List<Volume> channelPoints;
	private int channelPointIndex = 0;

	public ChannelPointsHandler( List< Volume> channelPoints)
	{
        this.channelPoints = channelPoints;
    }

	@Override
	public void valueRead( DesktopProtocol.Id id, String textContent)
	{
        switch( id)
        {
            case ChannelPoint -> readValue( channelPoints.get( channelPointIndex++), textContent);

            default -> super.valueRead( id, textContent);
        }
	}
}
