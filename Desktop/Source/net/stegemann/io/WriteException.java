package net.stegemann.io;

public class WriteException extends Exception
{
	private static final long serialVersionUID = -6323577888763997277L;

	public WriteException( String Message)
	{
		super( Message);
	}

	public WriteException( String Message, Exception Cause)
	{
		super( Message, Cause);
	}
}
