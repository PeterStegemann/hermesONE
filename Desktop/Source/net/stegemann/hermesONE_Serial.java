package net.stegemann;

import net.stegemann.io.ReadException;
import net.stegemann.io.serial.base.DesktopConnection;
import net.stegemann.io.serial.base.Ports;
import net.stegemann.io.serial.base.TypedConnectionHandler;

public class hermesONE_Serial //implements Runnable, SerialPortEventListener
{
//	InputStream inputStream;
//	SerialPort serialPort;
//	Thread readThread;

	public static void main( String[] Arguments)
	{
		new hermesONE_Serial().Main( Arguments);
	}

	public void Main( String[] Arguments)
	{
//		String defaultPort = "/dev/tty.PL2303-00001004";
		String UsePort = "/dev/cu.usbserial-A1048A6Y";

		if( Arguments.length > 0)
		{
			UsePort = Arguments[ 0];
		}

		System.out.println( "Available ports: " + Ports.findPorts());

		TypedConnectionForwarder ConnectionForwarder = new TypedConnectionForwarder();
		DesktopConnection UseConnection = new DesktopConnection();

		if( UseConnection.open( UsePort, ConnectionForwarder) == false)
		{
			System.out.println( "Failed to open connection with port " + UsePort + ".");
		}

		try
		{
			UseConnection.ping();
			UseConnection.readConfiguration();
			UseConnection.ping();
		}
		catch( ReadException e)
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		UseConnection.close();
    }

	private class TypedConnectionForwarder implements TypedConnectionHandler
	{
		@Override
		public void complexOpened( byte id)
		{
			System.err.println( "Complex opened: " + id);
		}

		@Override
		public void complexClosed()
		{
			System.err.println( "Complex closed");
		}

		@Override
		public void valueRead( byte id, String value)
		{
			System.err.println( "Value read: " + id + " {" + value + "}");
		}
	}
}
