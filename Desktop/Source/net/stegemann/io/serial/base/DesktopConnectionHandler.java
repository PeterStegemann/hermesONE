package net.stegemann.io.serial.base;

public abstract class DesktopConnectionHandler
	implements TypedConnectionHandler
{
	@Override
	public void complexOpened( byte id)
	{
		complexOpened( DesktopProtocol.enumFromByte( id));
	}

	@SuppressWarnings( "unused")
	public void complexOpened( DesktopProtocol.Id id)
	{
	}

	@Override
	public void valueRead( byte id, String value)
	{
		valueRead( DesktopProtocol.enumFromByte( id), value);
	}

	@SuppressWarnings( "unused")
	public void valueRead( DesktopProtocol.Id id, String textContent)
	{		
	}
}
