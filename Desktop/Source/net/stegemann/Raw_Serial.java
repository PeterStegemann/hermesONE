package net.stegemann;

import java.io.IOException;

import gnu.io.PortInUseException;
import gnu.io.SerialPort;
import gnu.io.UnsupportedCommOperationException;
import net.stegemann.io.serial.base.Ports;

public class Raw_Serial //implements Runnable, SerialPortEventListener
{
	public static void main( String[] Arguments) throws UnsupportedCommOperationException, PortInUseException, IOException
	{
		new Raw_Serial().Main( Arguments);
	}

	public void Main( String[] Arguments) throws UnsupportedCommOperationException, PortInUseException, IOException
	{
//		String defaultPort = "/dev/tty.PL2303-00001004";
		String Device = "/dev/cu.usbserial-A1048A6Y";
		int BaudRate = 9600;

		if( Arguments.length > 0)
		{
			Device = Arguments[ 0];
		}

		System.out.println( "Available ports: " + Ports.FindPorts());

		SerialPort UsePort = Ports.OpenPort( Device);

		UsePort.setFlowControlMode( SerialPort.FLOWCONTROL_NONE);
		UsePort.setSerialPortParams( BaudRate, SerialPort.DATABITS_8, SerialPort.STOPBITS_1,
											  SerialPort.PARITY_NONE);

		while( true)
		{
			UsePort.getOutputStream().write( "Switches On Off On Off On Off On\n".getBytes());
			UsePort.getOutputStream().write( "Switches Off On Off On Off On Off\n".getBytes());
		}

//		UsePort.close();
    }
}
