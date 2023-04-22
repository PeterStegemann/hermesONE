package net.stegemann.io.serial.base;

public interface TypedConnectionHandler
{
	void complexOpened( byte id);

	void complexClosed( byte id);

	void valueRead( byte id, String value);
}
