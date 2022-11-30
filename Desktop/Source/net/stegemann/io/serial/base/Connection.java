package net.stegemann.io.serial.base;

import gnu.io.PortInUseException;
import gnu.io.SerialPort;
import gnu.io.UnsupportedCommOperationException;
import net.stegemann.io.ReadException;
import net.stegemann.io.WriteException;

import java.io.IOException;

public class Connection
{
	private SerialPort port;

	public boolean open( String device, int baudRate, int receiveTimeout)
	{
		port = null;

		try
		{
			port = Ports.openPort( device);
		}
		catch( PortInUseException reason)
		{
			reason.printStackTrace();
		}

		if( port == null)
		{
			return false;
		}

		try
		{
			port.setFlowControlMode( SerialPort.FLOWCONTROL_NONE);
			port.setSerialPortParams( baudRate, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);
			port.enableReceiveTimeout( receiveTimeout);
		}
		catch( UnsupportedCommOperationException reason)
		{
			reason.printStackTrace();

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

	public byte readByte()
	 	throws ReadException
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

		return value;
	}

	public void writeByte( Enum<?> value)
		throws WriteException
	{
		writeByte(( byte) value.ordinal());
	}

	public void writeByte( byte value)
	 	throws WriteException
	{
		internalWriteByte( value);

		// Read echo.
		byte echo;

		try
		{
			echo = internalReadByte();
		}
		catch( ReadException reason)
		{
			throw new WriteException( "Reading echo failed!", reason);
		}

		if( echo != value)
		{
			throw new WriteException( "Echo doesn't match byte sent! ( " +  echo +" !=" + value + " )");
		}
	}

	@SuppressWarnings("unused")
	public void writeWord( int word)
		throws WriteException
	{
		writeByte(( byte)(( word >> 8) & 0xff));
		writeByte(( byte)( word & 0xff));
	}

	public void writeString( String string)
		throws WriteException
	{
		byte[] bytes = string.getBytes();

		for( byte aByte: bytes)
		{
			writeByte( aByte);
		}
	}

	private byte internalReadByte()
		throws ReadException
	{
		byte[] inputBytes = new byte[ 1];

		try
		{
			// noinspection ResultOfMethodCallIgnored
			port.getInputStream().read( inputBytes);
		}
		catch( IOException reason)
		{
			throw new ReadException( "Reading byte failed!", reason);
		}

		return inputBytes[ 0];
	}

	private void internalWriteByte( byte useByte)
		throws WriteException
	{
		byte[] outputBytes = new byte[ 1];
		outputBytes[ 0] = useByte;

		try
		{
			port.getOutputStream().write( outputBytes);
		}
		catch( IOException reason)
		{
			throw new WriteException( "Writing byte failed!", reason);
		}
	}
}
