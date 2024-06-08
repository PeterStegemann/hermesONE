package net.stegemann.io.xml;

import net.stegemann.configuration.Configuration;
import net.stegemann.configuration.Model;
import net.stegemann.configuration.source.Empty;
import net.stegemann.configuration.source.Fixed;
import net.stegemann.configuration.type.ValueOutOfRangeException;
import net.stegemann.io.DocumentException;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;

import static org.junit.jupiter.api.Assertions.assertEquals;

class XMLWriterIntegrationTest extends AbstractXMLIntegrationTest
{
	private final XMLWriter xmlWriter = new XMLWriter( new DocumentGenerator());

	private final String expected = Files.readString( Path.of( TEST_CONFIGURATION_XML));

	private static final String RESULT_XML = "Target/XMLIntegrationTest.xml";

	XMLWriterIntegrationTest()
		throws IOException {}

	@Test
	void writeToFile()
		throws DocumentException, IOException, ValueOutOfRangeException
	{
		Configuration configuration = configuration();
		Model model = configuration.getModels().getModelFromIndex( 0);
		insertSource( configuration, model, new Empty());
		insertSource( configuration, model, new Fixed());

		xmlWriter.writeToFile( configuration, RESULT_XML);

		String result = Files.readString( Path.of( RESULT_XML));

		assertEquals( expected, result);
	}
}
