package net.stegemann.io.serial.base;

public interface TypedConnectionHandler
{
	void complexOpened( byte Id);
	void complexClosed();
	void valueRead( byte Id, String Value);
}
