package net.stegemann.io.serial.base;

import net.stegemann.misc.ObjectFactory;

public final class BaseObjectFactory
	extends ObjectFactory
{
	public static Connection connection()
	{
		return singleton( Connection::new);
	}

	public static DesktopConnection desktopConnection()
	{
		return singleton( DesktopConnection::new);
	}

	public static TypedConnection typedConnection( TypedConnectionHandler handler)
	{
		return singleton( handler, () -> new TypedConnection( connection(), handler));
	}
}
