package net.stegemann;

import gnu.io.PortInUseException;
import gnu.io.SerialPort;
import gnu.io.UnsupportedCommOperationException;
import net.stegemann.io.serial.base.Ports;

import java.io.IOException;

public class Raw_Serial //implements Runnable, SerialPortEventListener
{
	public static void main( String[] Arguments) throws UnsupportedCommOperationException, PortInUseException, IOException
	{
		new Raw_Serial().Main( Arguments);
	}

	public void Main( String[] arguments)
		throws UnsupportedCommOperationException, PortInUseException, IOException
	{
//		String defaultPort = "/dev/tty.PL2303-00001004";
		String device = "/dev/cu.usbserial-A1048A6Y";
		int baudRate = 9600;

		if( arguments.length > 0)
		{
			device = arguments[ 0];
		}

		System.out.println( "Available ports: " + Ports.getPorts());

		SerialPort UsePort = Ports.openPort( device);

		UsePort.setFlowControlMode( SerialPort.FLOWCONTROL_NONE);
		UsePort.setSerialPortParams( baudRate, SerialPort.DATABITS_8, SerialPort.STOPBITS_1,
											  SerialPort.PARITY_NONE);

		while( true)
		{
			UsePort.getOutputStream().write( "Switches On Off On Off On Off On\n".getBytes());
			UsePort.getOutputStream().write( "Switches Off On Off On Off On Off\n".getBytes());
		}

//		UsePort.close();
    }
}
