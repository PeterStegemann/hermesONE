package net.stegemann.io.serial.configuration.read.handler;

import net.stegemann.configuration.ProxyReferences;
import net.stegemann.io.serial.base.DesktopProtocol;
import net.stegemann.io.serial.configuration.read.handler.source.SourceTupleHandler;

class ProxyReferencesHandler extends DesktopConnectionHandler
{
	private final ProxyReferences proxyReferences;
	private int proxyIndex;

	public ProxyReferencesHandler( ProxyReferences proxyReferences)
	{
		this.proxyReferences = proxyReferences;

		proxyIndex = 0;
	}

	@Override
	public void complexOpened( DesktopProtocol.Id id)
	{
		switch( id)
		{
			case ModelProxyReference :
			{
				pushHandler( new SourceTupleHandler( proxyReferences.getProxyReferenceFromIndex( proxyIndex++)));
			}
			break;

			default : super.complexOpened( id); break;
		}
	}
}
