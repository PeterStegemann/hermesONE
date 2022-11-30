package net.stegemann.configuration.type;

public interface ConfigurationValue
{
    void setConfigurationValue( String stringValue) throws ValueOutOfRangeException;

    String getConfigurationValue();
}
