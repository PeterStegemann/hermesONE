package net.stegemann.io.xml;

import net.stegemann.configuration.Configuration;
import net.stegemann.configuration.type.ValueOutOfRangeException;
import net.stegemann.io.DocumentException;
import org.junit.jupiter.api.Test;

import static org.hamcrest.CoreMatchers.equalTo;
import static org.hamcrest.MatcherAssert.assertThat;

public class XMLReaderIntegrationTest extends AbstractXMLIntegrationTest
{
    private final XMLReader xmlReader = new XMLReader( new DocumentGenerator());

    @Test
    void readFromFile()
        throws DocumentException, ValueOutOfRangeException
    {
        Configuration configuration = new Configuration();

        xmlReader.readFromFile( configuration, TEST_CONFIGURATION_XML, XMLReader.Mode.All);

        assertThat( configuration.toString(), equalTo( configuration().toString()));
    }
}
