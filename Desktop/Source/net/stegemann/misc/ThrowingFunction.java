package net.stegemann.misc;

public interface ThrowingFunction< ExceptionType extends Exception>
{
	void apply() throws ExceptionType;
}
