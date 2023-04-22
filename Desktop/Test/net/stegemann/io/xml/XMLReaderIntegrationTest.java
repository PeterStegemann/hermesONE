package net.stegemann.io.xml;

import net.stegemann.configuration.Configuration;
import net.stegemann.configuration.type.ValueOutOfRangeException;
import net.stegemann.io.DocumentException;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class XMLReaderIntegrationTest extends AbstractXMLIntegrationTest
{
    private final XMLReader xmlReader = new XMLReader( new DocumentGenerator());

    @Test
    void readFromFile()
        throws DocumentException, ValueOutOfRangeException
    {
        Configuration configuration = new Configuration();

        xmlReader.readFromFile( configuration, TEST_CONFIGURATION_XML, XMLReader.Mode.All);

        assertEquals( configuration().toString(), configuration.toString());
    }
}
