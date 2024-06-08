package net.stegemann.misc;

public interface ThrowingConsumer< Type, ExceptionType extends Exception>
{
	void accept( Type item) throws ExceptionType;
}
