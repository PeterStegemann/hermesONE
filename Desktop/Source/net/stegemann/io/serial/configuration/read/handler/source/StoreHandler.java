package net.stegemann.io.serial.configuration.read.handler.source;

import net.stegemann.configuration.source.Store;
import net.stegemann.io.serial.base.DesktopProtocol;
import net.stegemann.io.serial.configuration.read.handler.DesktopConnectionHandler;

class StoreHandler extends DesktopConnectionHandler
{
	private final Store store;

	public StoreHandler( Store store)
	{
		this.store = store;
	}

	@Override
	public void valueRead( DesktopProtocol.Id id, String textContent)
	{
		switch( id)
		{
			case SourceStoreInput -> readValue( store.getInput(), textContent);
			case SourceStoreInit -> readValue( store.getInit(), textContent);

			default -> super.valueRead( id, textContent);
		}
	}
}
