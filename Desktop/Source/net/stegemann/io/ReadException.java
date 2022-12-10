package net.stegemann.io;

import java.io.Serial;

public class ReadException extends DocumentException
{
	@Serial
	private static final long serialVersionUID = -6359511165210375059L;

	public ReadException( String message)
	{
		super( message);
	}

	public ReadException( String message, Exception cause)
	{
		super( message, cause);
	}
}
