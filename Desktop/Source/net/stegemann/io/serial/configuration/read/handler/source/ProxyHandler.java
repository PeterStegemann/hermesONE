package net.stegemann.io.serial.configuration.read.handler.source;

import net.stegemann.configuration.source.Proxy;
import net.stegemann.io.serial.base.DesktopProtocol;
import net.stegemann.io.serial.configuration.read.handler.DesktopConnectionHandler;

class ProxyHandler extends DesktopConnectionHandler
{
	private final Proxy proxy;

	public ProxyHandler( Proxy proxy)
	{
		this.proxy = proxy;
	}

	@Override
	public void valueRead( DesktopProtocol.Id id, String textContent)
	{
		switch( id)
		{
			case SourceProxySlot -> readValue( proxy.getSlot(), textContent);

			default -> super.valueRead(id, textContent);
		}
	}
}
