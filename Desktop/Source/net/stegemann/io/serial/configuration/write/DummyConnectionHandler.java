package net.stegemann.io.serial.configuration.write;

import net.stegemann.io.serial.base.TypedConnectionHandler;

class DummyConnectionHandler implements TypedConnectionHandler
{
    @Override
    public void complexOpened( byte id) {}

    @Override
    public void complexClosed( byte id) {}

    @Override
    public void valueRead( byte id, String value) {}
}
