package net.stegemann.io.serial.configuration.read;

import lombok.Getter;
import net.stegemann.configuration.Configuration;
import net.stegemann.io.ReadException;
import net.stegemann.io.serial.base.DesktopConnection;
import net.stegemann.io.serial.base.TypedConnectionHandler;
import net.stegemann.io.serial.configuration.ConfigurationProgress;
import net.stegemann.io.serial.configuration.read.handler.DesktopConnectionHandler;import net.stegemann.io.serial.configuration.read.handler.StartHandler;
import net.stegemann.misc.ChangeListener;

import java.util.ArrayList;

public class SerialConfigurationReader
{
    private static final boolean debug = false;

    @Getter
    private final ConfigurationProgress configurationProgress;
    private final DesktopConnection connection;

    private final ArrayList< TypedConnectionHandler> handlerStack = new ArrayList<>();

    public SerialConfigurationReader( ConfigurationProgress useConfigurationProgress, DesktopConnection useConnection)
    {
        configurationProgress = useConfigurationProgress;
        connection = useConnection;
    }

    public void readFromPort( Configuration configuration, String portName,
                              ChangeListener< ConfigurationProgress> configurationListener)
        throws ReadException
    {
        TypedConnectionForwarder connectionForwarder = new TypedConnectionForwarder();

        if( connection.open( portName, connectionForwarder) == false)
        {
            throw new ReadException( "Failed to open connection with port " + portName + ".");
        }

        configuration.clear();

        configurationProgress.reset();
        configurationProgress.addChangeListener( configurationListener);

        pushHandler( new StartHandler( configuration));

        try
        {
            connection.readConfiguration();
        }
        catch( ReadException reason)
        {
            throw new ReadException( "Failed to read configuration!", reason);
        }
        finally
        {
            configurationProgress.removeChangeListener( configurationListener);

            handlerStack.remove( 0);

            connection.close();
        }

//		java.lang.System.err.println( "End, stack size: " + handlerStack.size());
    }

    private class TypedConnectionForwarder
        implements TypedConnectionHandler
    {
        @Override
        public void complexOpened( byte id)
        {
//			java.lang.System.out.print( ".");

            handlerStack.get( 0).complexOpened( id);
        }

        @Override
        public void complexClosed( byte id)
        {
            handlerStack.get( 0).complexClosed( id);

            handlerStack.remove( 0);

//			java.lang.System.err.println( "Complex closed, stack size: " + handlerStack.size());
        }

        @Override
        public void valueRead( byte id, String value)
        {
            handlerStack.get( 0).valueRead( id, value);
        }
    }

    public void pushHandler( DesktopConnectionHandler handler)
    {
        handler.setSerialConfigurationReader( this);
        handlerStack.add( 0, handler);
    }

    private static void debug( String text)
    {
        if( debug)
        {
            java.lang.System.out.println( text);
        }
    }
}
