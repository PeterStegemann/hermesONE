package net.stegemann.io.xml;

import net.stegemann.configuration.Configuration;
import net.stegemann.configuration.Model;
import net.stegemann.configuration.System;
import net.stegemann.configuration.Type;
import net.stegemann.configuration.source.*;
import net.stegemann.configuration.source.input.*;
import net.stegemann.configuration.type.ValueOutOfRangeException;

abstract class AbstractXMLIntegrationTest
{
    protected static final String TEST_CONFIGURATION_XML = "Test/XMLIntegrationTest.xml";

    protected static Configuration configuration()
            throws ValueOutOfRangeException
    {
        Configuration configuration = new Configuration();

        System system = configuration.getSystem();
        system.getOwner().setValue( "Peter Stegemann");
        system.getCalibrations().getCalibration( 0).getLow().setValue( 42);

        Type type = new Type();
        type.setState( Type.State.USED);
        type.getName().setValue( "Type 0");
        configuration.getTypes().insertType( type);

        Model model = new Model();
        model.setState( Model.State.USED);
        model.getName().setValue( "Model 0");
        model.getTypeId().setValue( type.getId());
        configuration.getModels().insertModel( model);

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

        type = new Type();
        type.setState( Type.State.USED);
        type.getName().setValue( "Type 1");
        configuration.getTypes().insertType( type);

        model = new Model();
        model.setState( Model.State.USED);
        model.getName().setValue( "Model 1");
        model.getTypeId().setValue( type.getId());
        configuration.getModels().insertModel( model);

        insertSource( configuration, model, new Mix());
        insertSource( configuration, type, new Map());

        configuration.fill();

        return configuration;
    }

    protected static void insertSource( Configuration configuration, Model model, Source source)
        throws ValueOutOfRangeException
    {
        source.setModel( model.getId());
        source.getName().setValue( source.getClass().getName());
        configuration.getSources().insertSource( source);
    }

    protected static void insertSource( Configuration configuration, Type type, Source source)
        throws ValueOutOfRangeException
    {
        source.setModel( type.getId());
        source.getName().setValue( source.getClass().getName());
        configuration.getSources().insertSource( source);
    }
}
