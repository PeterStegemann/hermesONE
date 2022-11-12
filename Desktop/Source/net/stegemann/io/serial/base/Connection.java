package net.stegemann.io.serial.base;

import java.io.IOException;

import net.stegemann.io.ReadException;
import net.stegemann.io.WriteException;
import gnu.io.PortInUseException;
import gnu.io.SerialPort;
import gnu.io.UnsupportedCommOperationException;

public class Connection
{
	private SerialPort port;

	public boolean open( String Device, int BaudRate)
	{
		port = null;

		try
		{
			port = Ports.OpenPort( Device);
		}
		catch( PortInUseException e)
		{
			e.printStackTrace();
		}

		if( port == null)
		{
			return false;
		}

		try
		{
			port.setFlowControlMode( SerialPort.FLOWCONTROL_NONE);
			port.setSerialPortParams( BaudRate, SerialPort.DATABITS_8, SerialPort.STOPBITS_1,
									  		  SerialPort.PARITY_NONE);
		}
		catch( UnsupportedCommOperationException e)
		{
			e.printStackTrace();

			port.close();

			return false;
		}

		return true;
	}

	public void close()
	{
		if( port != null)
		{
			port.close();
		}
	}

	public byte readByte() throws ReadException
	{
		byte value = internalReadByte();

		// Write echo.
		try
		{
			internalWriteByte( value);
		}
		catch( WriteException reason)
		{
			throw new ReadException( "Writing echo failed!", reason);
		}

		return( value);
	}

	public void writeByte( byte value) throws WriteException
	{
		internalWriteByte( value);

		// Read echo.
		byte Echo;

		try
		{
			Echo = internalReadByte();
		}
		catch( ReadException reason)
		{
			throw new WriteException( "Reading echo failed!", reason);
		}

		if( Echo != value)
		{
			throw new WriteException( "Echo doesn't match byte sent! ( " +  Echo +" !=" + value +
									  " )");
		}
	}

	public void WriteWord( int Word) throws WriteException
	{
		writeByte(( byte)(( Word >> 8) & 0xff));
		writeByte(( byte)( Word & 0xff));
	}

	public void WriteString( String UseString) throws WriteException
	{
		byte[] Bytes = UseString.getBytes();

		for( int Index = 0; Index < Bytes.length; Index++)
		{
			writeByte( Bytes[ Index]);
		}
	}

	private byte internalReadByte() throws ReadException
	{
		byte[] InputBytes = new byte[ 1];

		try
		{
			port.getInputStream().read( InputBytes);
		}
		catch( IOException reason)
		{
			throw new ReadException( "Reading byte failed!", reason);
		}

		return InputBytes[ 0];
	}

	private void internalWriteByte( byte UseByte) throws WriteException
	{
		byte[] OutputBytes = new byte[ 1];
		OutputBytes[ 0] = UseByte;

		try
		{
			port.getOutputStream().write( OutputBytes);
		}
		catch( IOException reason)
		{
			throw new WriteException( "Writing byte failed!", reason);
		}
	}
}
