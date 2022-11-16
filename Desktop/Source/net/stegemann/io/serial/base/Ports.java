package net.stegemann.io.serial.base;

import java.util.ArrayList;
import java.util.Enumeration;
import java.util.List;

import gnu.io.CommPortIdentifier;
import gnu.io.PortInUseException;
import gnu.io.SerialPort;

public class Ports
{
	public static SerialPort openPort( String portName)
		throws PortInUseException
	{
		@SuppressWarnings("unchecked")
	 	Enumeration< CommPortIdentifier> portList = CommPortIdentifier.getPortIdentifiers();

		while( portList.hasMoreElements())
		{
			CommPortIdentifier portId = portList.nextElement();

			if(
				portId.getPortType() == CommPortIdentifier.PORT_SERIAL &&
				portId.getName().equals( portName)
			)
			{
				return ( SerialPort) portId.open( portName, 2000);
			}
		}

		return null;
	}

	@SuppressWarnings( "unchecked")
	public static List< String> findPorts()
	{
		ArrayList< String> ports = new ArrayList<>();

		Enumeration< CommPortIdentifier> portList = CommPortIdentifier.getPortIdentifiers();

		while( portList.hasMoreElements())
		{
			CommPortIdentifier portId = portList.nextElement();

			if( portId.getPortType() == CommPortIdentifier.PORT_SERIAL)
			{
				ports.add( portId.getName());
			}
		}

		return ports;
	}
}
