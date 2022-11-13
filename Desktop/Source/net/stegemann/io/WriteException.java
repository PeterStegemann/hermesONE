package net.stegemann.io;

import java.io.Serial;

public class WriteException extends Exception
{
	@Serial
	private static final long serialVersionUID = -6323577888763997277L;

	public WriteException( String message)
	{
		super( message);
	}

	public WriteException( String message, Exception cause)
	{
		super( message, cause);
	}
}
