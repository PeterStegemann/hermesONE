package net.stegemann.io.xml;

import net.stegemann.configuration.Configuration;
import net.stegemann.configuration.Model;
import net.stegemann.configuration.System;
import net.stegemann.configuration.Type;
import net.stegemann.configuration.source.*;
import net.stegemann.configuration.source.input.*;
import net.stegemann.configuration.type.ValueOutOfRangeException;
import net.stegemann.io.WriteException;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;

import static org.hamcrest.CoreMatchers.equalTo;
import static org.hamcrest.MatcherAssert.assertThat;

class XMLWriterIntegrationTest
{
	private final DocumentGenerator documentGenerator = new DocumentGenerator();
	private final XMLWriter xmlWriter = new XMLWriter( documentGenerator);

	private final Configuration configuration = configuration();

	private static final String EXPECTED_XML = "Test/XMLWriterIntegrationTest.xml";
	private static final String RESULT_XML	 = "Target/XMLWriterIntegrationTest.xml";

	XMLWriterIntegrationTest()
		throws ValueOutOfRangeException {}

	@Test
	void writeToFile()
		throws WriteException, IOException
	{
		xmlWriter.writeToFile( configuration, RESULT_XML);

		String expected = Files.readString( Path.of(EXPECTED_XML));
		String result = Files.readString( Path.of(RESULT_XML));

		assertThat( result, equalTo( expected));
	}

	private static Configuration configuration()
		throws ValueOutOfRangeException
	{
		Configuration configuration = new Configuration();

		System system = configuration.getSystem();
		system.getOwner().setValue( "Peter Stegemann");
		system.getCalibrations().getCalibration( 0).getLow().setValue( 42);

		Type type = new Type();
		type.setState( Type.State.USED);
		type.getName().setValue( "Type");
		configuration.getTypes().insertType( type);

		Model model = new Model();
		model.setState( Model.State.USED);
		model.getName().setValue( "Model");
		model.getTypeId().setValue( type.getId());
		configuration.getModels().insertModel( model);

		insertSource( configuration, model, new Empty());
		insertSource( configuration, model, new Fixed());
		insertSource( configuration, model, new Follower());
		insertSource( configuration, model, new Map());
		insertSource( configuration, model, new Mix());
		insertSource( configuration, model, new Proxy());
		insertSource( configuration, model, new Store());
		insertSource( configuration, model, new Timer());
		insertSource( configuration, model, new Trimmer());

		insertSource( configuration, model, new Analog());
		insertSource( configuration, model, new Button());
		insertSource( configuration, model, new Rotary());
		insertSource( configuration, model, new Switch());
		insertSource( configuration, model, new Ticker());

		configuration.fill();

		return configuration;
	}

	private static void insertSource( Configuration configuration, Model model, Source source)
	{
		source.setModel( model.getId());
		source.getName().setValue( source.getClass().getName());
		configuration.getSources().insertSource( source);
	}
}
