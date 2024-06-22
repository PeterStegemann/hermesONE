package net.stegemann.configuration.type;

public interface ConfigurationValue
{
    void setStringValue( String stringValue) throws ValueOutOfRangeException;

    String getStringValue();
}
