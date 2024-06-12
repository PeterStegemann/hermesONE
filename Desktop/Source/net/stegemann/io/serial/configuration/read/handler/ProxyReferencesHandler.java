package net.stegemann.io.serial.configuration.read.handler;

import net.stegemann.configuration.ProxyReferences;
import net.stegemann.io.serial.base.DesktopProtocol;
import net.stegemann.io.serial.configuration.read.handler.source.SourceTupleHandler;

class ProxyReferencesHandler extends DesktopConnectionHandler
{
    private final ProxyReferences proxyReferences;
    private int proxyIndex = 0;

    public ProxyReferencesHandler( ProxyReferences proxyReferences)
    {
        this.proxyReferences = proxyReferences;
    }

    @Override
    public void complexOpened( DesktopProtocol.Id id)
    {
        switch( id)
        {
            case ModelProxyReference -> pushHandler
            (
                new SourceTupleHandler( proxyReferences.getProxyReferenceFromIndex( proxyIndex++))
            );

            default -> super.complexOpened( id);
        }
    }
}
