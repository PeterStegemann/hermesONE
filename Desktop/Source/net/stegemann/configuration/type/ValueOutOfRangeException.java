package net.stegemann.configuration.type;

public class ValueOutOfRangeException extends Exception
{
	private static final long serialVersionUID = 3918638909869326264L;

	public ValueOutOfRangeException( String message)
	{
		super( message);
	}
}
