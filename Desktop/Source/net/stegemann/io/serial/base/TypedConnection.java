package net.stegemann.io.serial.base;

import net.stegemann.io.ReadException;
import net.stegemann.io.WriteException;

public class TypedConnection
{
	private static final boolean debug = true;

	private static final int BAUD_RATE = 115200;
	private static final int RECEIVE_TIMEOUT = 1000;

	private final TypedConnectionHandler handler;
	private final Connection connection;

	public TypedConnection( Connection connection, TypedConnectionHandler handler)
	{
		this.connection = connection;
		this.handler = handler;
	}

	public boolean open( String device)
	{
		return connection.open( device, BAUD_RATE, RECEIVE_TIMEOUT);
	}

	public void close()
	{
		connection.close();
	}

	public void writeCommand( byte command)
		throws WriteException
	{
		connection.writeByte( TypedProtocol.T_Command);
		connection.writeByte( command);
	}

	public byte readResponse()
	 	throws ReadException
	{
		return connection.readByte();
	}

	public void readItem( byte type)
		throws ReadException
	{
		byte id = connection.readByte();

		switch( type)
		{
			case TypedProtocol.T_Value -> readValue( id);
			case TypedProtocol.T_Complex -> readComplex( id);

			default -> throw new ReadException( "ReadItem unexpected type: " + type);
		}
	}

	void readValue( byte id)
	 	throws ReadException
	{
		byte[] Response = new byte[ 256];

		int Count = 0;

		while( true)
		{
			byte Read = connection.readByte();

			Response[ Count] = Read;

			if( Read == 0)
			{
				break;
			}

			if( Count == 255)
			{
				throw new ReadException( "ReadValue too big");
			}

			Count++;
		}

		if(debug)	System.out.println( "Read value " + id + " " + new String( Response, 0, Count));

		if( handler != null)
		{
			handler.valueRead( id, new String( Response, 0, Count));
		}
	}

	void readComplex( byte id)
	 	throws ReadException
	{
		if(debug)	System.out.println( "Reading complex open " + id);

		if( handler != null)
		{
			handler.complexOpened( id);
		}

		boolean loop = true;

		while( loop)
		{
			byte type = connection.readByte();

			switch( type)
			{
				case TypedProtocol.T_State ->
				{
					byte state = connection.readByte();

					throw new ReadException( "ReadComplex error: " + state);
				}

				case TypedProtocol.T_Value, TypedProtocol.T_Complex ->
				{
					readItem( type);
				}

				case TypedProtocol.T_ComplexEnd ->
				{
					if(debug)	System.out.println( "Reading complex close " + id);

					if( handler != null)
					{
						handler.complexClosed();
					}

					loop = false;
				}

				default -> throw new ReadException( "ReadComplex unexpected type: " + type);
			}
		}
	}

	public void writeValue( byte id, String value)
	 	throws WriteException
	{
		if(debug)	System.out.println( "Writing value " + id + " " + value);

		connection.writeByte( TypedProtocol.T_Value);
		connection.writeByte( id);

		if( value != null)
		{
			connection.writeString( value);
		}

		connection.writeByte(( byte) 0);
	}

	public void openComplex( byte id)
		throws WriteException
	{
		if(debug)	System.out.println( "Writing complex open " + id);

		connection.writeByte( TypedProtocol.T_Complex);
		connection.writeByte( id);
	}

	public void closeComplex()
	 	throws WriteException
	{
		if(debug)	System.out.println( "Writing complex close");

		connection.writeByte( TypedProtocol.T_ComplexEnd);
	}
}
