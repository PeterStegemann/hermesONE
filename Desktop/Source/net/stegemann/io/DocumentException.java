package net.stegemann.io;

public class DocumentException extends Exception
{
    protected DocumentException( String message)
    {
        super( message);
    }

    public DocumentException(String message, Exception cause)
    {
        super( message, cause);
    }
}
