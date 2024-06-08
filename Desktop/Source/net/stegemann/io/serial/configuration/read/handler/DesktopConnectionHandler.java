package net.stegemann.io.serial.configuration.read.handler;

import lombok.Setter;
import net.stegemann.configuration.type.Bool;import net.stegemann.configuration.type.Number;import net.stegemann.configuration.type.Text;import net.stegemann.configuration.type.ValueOutOfRangeException;import net.stegemann.io.serial.base.DesktopProtocol;
import net.stegemann.io.serial.base.TypedConnectionHandler;
import net.stegemann.io.serial.configuration.ConfigurationProgress;
import net.stegemann.io.serial.configuration.read.SerialConfigurationReader;

public abstract class DesktopConnectionHandler implements TypedConnectionHandler
{
    private static final boolean debug = false;

    @Setter
    private SerialConfigurationReader serialConfigurationReader;

    protected void pushHandler( DesktopConnectionHandler handler)
    {
        serialConfigurationReader.pushHandler( handler);
    }

    protected ConfigurationProgress getConfigurationProgress()
    {
        return serialConfigurationReader.getConfigurationProgress();
    }

    @Override
    public void complexOpened( byte id)
    {
        DesktopProtocol.Id desktopId = DesktopProtocol.enumFromByte(id);

        debug("Reading complex open '" + desktopId + "'");

        complexOpened( desktopId);
    }

    @Override
    public void complexClosed( byte id)
    {
        DesktopProtocol.Id desktopId = DesktopProtocol.enumFromByte( id);

        debug("Reading complex close '" + desktopId + "'");

        complexClosed( desktopId);
    }

    @Override
    public void valueRead( byte id, String value)
    {
        DesktopProtocol.Id desktopId = DesktopProtocol.enumFromByte( id);

        debug("Read value " + desktopId + " '" + value + "'");

        valueRead( desktopId, value);
    }

    public void complexOpened( DesktopProtocol.Id id)
    {
        java.lang.System.out.println( "Unknown complex opened: " + id);

        pushHandler( new UnknownTypeHandler());
    }

    public void complexClosed( DesktopProtocol.Id id)
    {
        java.lang.System.out.println( "Unknown complex closed: " + id);
    }

    public void valueRead( DesktopProtocol.Id id, String textContent)
    {
        java.lang.System.out.println( "Unknown value " + id.byteValue() + " {" + textContent + "}");
    }

    protected static void readValue( Number value, String textContent)
    {
        try
        {
            value.setConfigurationValue( textContent);
        }
        catch( NumberFormatException | ValueOutOfRangeException ignored)
        {
        }
    }

    protected static void readValue( Text value, String textContent)
    {
        value.setConfigurationValue( textContent);
    }

    protected static void readValue( Bool value, String textContent)
    {
        value.setConfigurationValue( textContent);
    }

    protected static void debug( String text)
    {
        if( debug)
        {
            System.out.println( text);
        }
    }
}
