package net.stegemann.io.serial.base;

import net.stegemann.io.ReadException;
import net.stegemann.io.WriteException;

public class TypedConnection
{
	private static boolean Debug = true;

	private TypedConnectionHandler handler = null;
	private Connection connection = new Connection();

	public TypedConnection( TypedConnectionHandler handler)
	{
		this.handler = handler;
	}

	public boolean Open( String device)
	{
		return connection.open( device, 115200);
	}

	public void close()
	{
		connection.close();
	}

	public void writeCommand( byte Command) throws WriteException
	{
		connection.writeByte( TypedProtocol.T_Command);
		connection.writeByte( Command);
	}

	public byte readResponse() throws ReadException
	{
		return connection.readByte();
	}

	public void readItem( byte Type) throws ReadException
	{
		byte Id = connection.readByte();

		switch( Type)
		{
			case TypedProtocol.T_Value :		readValue( Id);	break;
			case TypedProtocol.T_Complex :	readComplex( Id);	break;

			default :	throw new ReadException( "ReadItem unexpected type: " + Type);
		}
	}

	void readValue( byte Id) throws ReadException
	{
		boolean Loop = true;

		byte[] Response = new byte[ 256];

		int Count = 0;

		while( Loop)
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

		if( Debug)	System.out.println( "Read value " + Id + " " + new String( Response, 0, Count));

		if( handler != null)
		{
			handler.valueRead( Id, new String( Response, 0, Count));
		}
	}

	void readComplex( byte Id) throws ReadException
	{
		if( Debug)	System.out.println( "Reading complex open " + Id);

		if( handler != null)
		{
			handler.complexOpened( Id);
		}

		boolean Loop = true;

		while( Loop)
		{
			byte Type = connection.readByte();

			switch( Type)
			{
				case TypedProtocol.T_State :
				{
					byte State = connection.readByte();

					throw new ReadException( "ReadComplex error: " + State);
				}

				case TypedProtocol.T_Value :
				case TypedProtocol.T_Complex :		readItem( Type);	break;

				case TypedProtocol.T_ComplexEnd :
				{
					if( Debug)	System.out.println( "Reading complex close " + Id);

					if( handler != null)
					{
						handler.complexClosed();
					}

					Loop = false;
				}
				break;

				default : throw new ReadException( "ReadComplex unexpected type: " + Type);
			}
		}
	}

	public void writeValue( byte Id, String Value) throws WriteException
	{
		if( Debug)	System.out.println( "Writing value " + Id + " " + Value);

		connection.writeByte( TypedProtocol.T_Value);
		connection.writeByte( Id);

		if( Value != null)
		{
			connection.WriteString( Value);
		}

		connection.writeByte(( byte) 0);
	}

	public void openComplex( byte Id) throws WriteException
	{
		if( Debug)	System.out.println( "Writing complex open " + Id);

		connection.writeByte( TypedProtocol.T_Complex);
		connection.writeByte( Id);
	}

	public void closeComplex() throws WriteException
	{
		if( Debug)	System.out.println( "Writing complex close");

		connection.writeByte( TypedProtocol.T_ComplexEnd);
	}
}
