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

	public static void main( String[] arguments)
	{
		new hermesONE_Serial().Main( arguments);
	}

	public void Main( String[] Arguments)
	{
//		String defaultPort = "/dev/tty.PL2303-00001004";
		String usePort = "/dev/cu.usbserial-A1048A6Y";

		if( Arguments.length > 0)
		{
			usePort = Arguments[ 0];
		}

		System.out.println( "Available ports: " + Ports.getPorts());

		TypedConnectionForwarder connectionForwarder = new TypedConnectionForwarder();
		DesktopConnection UseConnection = new DesktopConnection();

		if( UseConnection.open( usePort, connectionForwarder) == false)
		{
			System.out.println( "Failed to open connection with port " + usePort + ".");
		}

		try
		{
			UseConnection.ping();
			UseConnection.readConfiguration();
			UseConnection.ping();
		}
		catch( ReadException e)
		{
			e.printStackTrace();
		}

		UseConnection.close();
    }

	private static class TypedConnectionForwarder implements TypedConnectionHandler
	{
		@Override
		public void complexOpened( byte id)
		{
			System.err.println( "Complex opened: " + id);
		}

		@Override
		public void complexClosed( byte id)
		{
			System.err.println( "Complex closed: " + id);
		}

		@Override
		public void valueRead( byte id, String value)
		{
			System.err.println( "Value read: " + id + " {" + value + "}");
		}
	}
}
