package net.stegemann.io.serial.base;

public abstract class DesktopConnectionHandler
	implements TypedConnectionHandler
{
	private static final boolean debug = false;

	@Override
	public void complexOpened( byte id)
	{
		DesktopProtocol.Id desktopId = DesktopProtocol.enumFromByte(id);

		debug("Reading complex open '" + desktopId + "'");

		complexOpened( desktopId);
	}

	public abstract void complexOpened( DesktopProtocol.Id id);

	@Override
	public void complexClosed( byte id)
	{
		DesktopProtocol.Id desktopId = DesktopProtocol.enumFromByte(id);

		debug("Reading complex close '" + desktopId + "'");

		complexClosed( desktopId);
	}

	public abstract void complexClosed( DesktopProtocol.Id id);

	@Override
	public void valueRead( byte id, String value)
	{
		DesktopProtocol.Id desktopId = DesktopProtocol.enumFromByte(id);

		debug("Read value " + desktopId + " '" + value + "'");

		valueRead( desktopId, value);
	}

	public abstract void valueRead( DesktopProtocol.Id id, String textContent);

	private static void debug( String text)
	{
		if( debug)
		{
			System.out.println( text);
		}
	}
}
