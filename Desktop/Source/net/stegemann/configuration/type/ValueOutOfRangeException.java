package net.stegemann.configuration.type;

import java.io.Serial;

public class ValueOutOfRangeException extends Exception
{
	@Serial
	private static final long serialVersionUID = 3918638909869326264L;

	public ValueOutOfRangeException( String message)
	{
		super( message);
	}
}
