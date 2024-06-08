package net.stegemann.io.serial.base;

import gnu.io.CommPortIdentifier;
import gnu.io.PortInUseException;
import gnu.io.SerialPort;

import java.util.ArrayList;
import java.util.Enumeration;
import java.util.List;

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

	public static List< String> getPorts()
	{
		ArrayList< String> ports = new ArrayList<>();

		@SuppressWarnings( "unchecked")
		Enumeration< CommPortIdentifier> portIdentifiers = CommPortIdentifier.getPortIdentifiers();

		while( portIdentifiers.hasMoreElements())
		{
			CommPortIdentifier portIdentifier = portIdentifiers.nextElement();

			if( portIdentifier.getPortType() == CommPortIdentifier.PORT_SERIAL)
			{
				ports.add( portIdentifier.getName());
			}
		}

		return ports;
	}
}
