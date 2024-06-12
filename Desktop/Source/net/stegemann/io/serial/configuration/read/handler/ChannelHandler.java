package net.stegemann.io.serial.configuration.read.handler;

import net.stegemann.configuration.Channel;
import net.stegemann.io.serial.base.DesktopProtocol;
import net.stegemann.io.serial.configuration.read.handler.source.SourceTupleHandler;

class ChannelHandler extends DesktopConnectionHandler
{
    private final Channel channel;

    public ChannelHandler( Channel channel)
    {
        this.channel = channel;
    }

    @Override
    public void complexOpened( DesktopProtocol.Id id)
    {
        switch( id)
        {
            case ChannelInput -> pushHandler( new SourceTupleHandler( channel.getInput()));
            case ChannelTrim -> pushHandler( new SourceTupleHandler( channel.getTrim()));
            case ChannelLimit -> pushHandler( new SourceTupleHandler( channel.getLimit()));
            case ChannelPoints -> pushHandler( new ChannelPointsHandler( channel.getPoints()));

            default -> super.complexOpened( id);
        }
    }

    @Override
    public void valueRead( DesktopProtocol.Id id, String textContent)
    {
        switch( id)
        {
            case ChannelName -> readValue( channel.getName(), textContent);
            case ChannelReverse -> readValue( channel.getReverse(), textContent);
            case ChannelMode -> readValue( channel.getMode(), textContent);

            default -> super.valueRead( id, textContent);
        }
    }
}
