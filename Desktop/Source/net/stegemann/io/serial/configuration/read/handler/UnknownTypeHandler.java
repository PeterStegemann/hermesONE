package net.stegemann.io.serial.configuration.read.handler;

import net.stegemann.io.serial.base.DesktopProtocol;

public class UnknownTypeHandler extends DesktopConnectionHandler
{
    @Override
    public void complexClosed( DesktopProtocol.Id id)
    {
        java.lang.System.err.println( "Unknown complex closed: " + id);
    }
}
