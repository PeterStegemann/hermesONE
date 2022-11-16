package net.stegemann.io.serial.base;

public interface TypedConnectionHandler
{
	void complexOpened( byte id);

	void complexClosed();

	void valueRead( byte id, String value);
}
