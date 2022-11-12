package net.stegemann.io.serial.base;

import java.util.ArrayList;
import java.util.Enumeration;
import java.util.List;

import gnu.io.CommPortIdentifier;
import gnu.io.PortInUseException;
import gnu.io.SerialPort;

public class Ports
{
	@SuppressWarnings( "unchecked")
	public static SerialPort OpenPort( String PortName) throws PortInUseException
	{
		Enumeration< CommPortIdentifier> PortList = CommPortIdentifier.getPortIdentifiers();

		while( PortList.hasMoreElements())
		{
			CommPortIdentifier PortId = PortList.nextElement();

			if( PortId.getPortType() == CommPortIdentifier.PORT_SERIAL)
			{
				if( PortId.getName().equals( PortName))
				{
					return ( SerialPort) PortId.open( PortName, 2000);
				}
			}
		}

		return null;
	}

	@SuppressWarnings( "unchecked")
	public static List< String> FindPorts()
	{
		ArrayList< String> Ports = new ArrayList< String>();

		Enumeration< CommPortIdentifier> PortList = CommPortIdentifier.getPortIdentifiers();

		while( PortList.hasMoreElements())
		{
			CommPortIdentifier PortId = PortList.nextElement();

			if( PortId.getPortType() == CommPortIdentifier.PORT_SERIAL)
			{
				Ports.add( PortId.getName());
			}
		}

		return Ports;
	}
}
