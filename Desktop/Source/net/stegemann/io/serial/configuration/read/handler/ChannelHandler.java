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
			case ChannelInput :
			{
				pushHandler( new SourceTupleHandler( channel.getInput()));
			}
			break;

			case ChannelTrim :
			{
				pushHandler( new SourceTupleHandler( channel.getTrim()));
			}
			break;

			case ChannelLimit :
			{
				pushHandler( new SourceTupleHandler( channel.getLimit()));
			}
			break;

			case ChannelPoints :
			{
				pushHandler( new ChannelPointsHandler( channel));
			}
			break;

			default : super.complexOpened( id); break;
		}
	}

	@Override
	public void valueRead( DesktopProtocol.Id id, String textContent)
	{
		switch( id)
		{
			case ChannelName :
			{
				readValue( channel.getName(), textContent);
			}
			break;

			case ChannelReverse :
			{
				readValue( channel.getReverse(), textContent);
			}
			break;

			case ChannelMode :
			{
				readValue( channel.getMode(), textContent);
			}
			break;

			default : super.valueRead( id, textContent); break;
		}
	}
}
