package net.stegemann.io.serial.base;

public abstract class DesktopConnectionHandler implements TypedConnectionHandler
{
	@Override
	public void complexOpened( byte Id)
	{
		complexOpened( DesktopProtocol.enumFromByte( Id));
	}

	@SuppressWarnings( "unused")
	public void complexOpened( DesktopProtocol.Id UseId)
	{
	}

	@Override
	public void valueRead( byte Id, String TextContent)
	{
		valueRead( DesktopProtocol.enumFromByte( Id), TextContent);
	}

	@SuppressWarnings( "unused")
	public void valueRead( DesktopProtocol.Id UseId, String TextContent)
	{		
	}
}
