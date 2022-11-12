package net.stegemann.io;

public class ReadException extends Exception
{
	private static final long serialVersionUID = -6359511165210375059L;

	public ReadException( String Message)
	{
		super( Message);
	}

	public ReadException( String Message, Exception Cause)
	{
		super( Message, Cause);
	}
}
