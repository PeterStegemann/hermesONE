package net.stegemann.io.serial.base;

import net.stegemann.io.ReadException;
import net.stegemann.io.WriteException;
import net.stegemann.io.serial.base.DesktopProtocol.Id;

public class DesktopConnection
{
	private TypedConnection connection;

	public boolean open( String device, TypedConnectionHandler handler)
	{
		connection = BaseObjectFactory.typedConnection( handler);

		return connection.open( device);
	}

	public void close()
	{
		connection.close();
		connection = null;
	}

	public void ping()
	 	throws ReadException
	{
		try
		{
			writeCommand( DesktopProtocol.Id.Ping);
		}
		catch( WriteException reason)
		{
			throw new ReadException( "Sending ping failed!", reason);
		}

		byte type = connection.readResponse();

		switch( type)
		{
			case TypedProtocol.T_State :
			{
				DesktopProtocol.Id id = DesktopProtocol.enumFromByte( connection.readResponse());

				if( id != DesktopProtocol.Id.Ok)
				{
					throw new ReadException( "Pong: " + id);
				}
			}
			break;

			default : throw new ReadException( "Ping unexpected type: " + type);
		}
	}

	public void readConfiguration()
		throws ReadException
	{
		try
		{
			writeCommand( DesktopProtocol.Id.ReadConfiguration);
		}
		catch( WriteException reason)
		{
			throw new ReadException( "Reading configuration failed!", reason);
		}

		// Get acknowledge.
		byte type = connection.readResponse();

		switch( type)
		{
			case TypedProtocol.T_State :
			{
				DesktopProtocol.Id Id = DesktopProtocol.enumFromByte( connection.readResponse());

				if( Id != DesktopProtocol.Id.Ok)
				{
					throw new ReadException( "ReadConfiguration error: " + Id);
				}
			}
			break;

			default : connection.readItem( type);
		}
	}

	public void writeConfiguration()
		throws WriteException
	{
		writeCommand( DesktopProtocol.Id.WriteConfiguration);

		try
		{
			// Get acknowledge.
			byte Type = connection.readResponse();

			switch( Type)
			{
				case TypedProtocol.T_State :
				{
					DesktopProtocol.Id id = DesktopProtocol.enumFromByte( connection.readResponse());

					if( id != DesktopProtocol.Id.Ok)
					{
						throw new WriteException( "WriteConfiguration error: " + id);
					}
				}
				break;

				default : break;
			}
		}
		catch( ReadException reason)
		{
			throw new WriteException( "Writing configuration failed!", reason);
		}
	}

	public void writeCommand( DesktopProtocol.Id command)
		throws WriteException
	{
		connection.writeCommand( DesktopProtocol.byteFromEnum( command));		
	}

	public void openComplex( Id id)
		throws WriteException
	{
		connection.openComplex( DesktopProtocol.byteFromEnum( id));
	}

	public void closeComplex()
		throws WriteException
	{
		connection.closeComplex();		
	}

	public void writeValue( DesktopProtocol.Id id, String value)
		throws WriteException
	{
		connection.writeValue( DesktopProtocol.byteFromEnum( id), value);
	}
}
