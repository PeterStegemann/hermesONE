package net.stegemann.configuration.source;

import lombok.Getter;
import net.stegemann.configuration.Configuration;import net.stegemann.configuration.Signal;
import net.stegemann.configuration.type.SourceId;
import net.stegemann.configuration.type.SourceWithVolume;
import net.stegemann.configuration.util.ConfigurationField;
import net.stegemann.io.xml.Names;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

@Getter
@ConfigurationField( name = Names.SOURCE_MIX)
public final class Mix extends Source
{
    public static final int INPUT_SIGNAL_PER_VALUE = ( Signal.VALUE_RANGE / 600);

    public static final int INPUTS = 7;

    @ConfigurationField( name = Names.SOURCE_MIX_INPUTS, itemName = Names.SOURCE_MIX_INPUT)
    private final List< SourceWithVolume> inputs = new ArrayList<>();

    public Mix()
    {
        for( int count = 0; count < INPUTS; count++)
        {
            inputs.add( new SourceWithVolume( INPUT_SIGNAL_PER_VALUE));
        }
    }

    public Mix( Mix other)
    {
        super( other);

        for( SourceWithVolume otherInput: other.inputs)
        {
            inputs.add( new SourceWithVolume( otherInput));
        }
    }

    public SourceWithVolume getInput( int index)
    {
        return inputs.get( index);
    }

    @Override
    public Source duplicate()
    {
        return new Mix( this);
    }

    @Override
    public void replaceSources( HashMap< SourceId, SourceId> sourcesMap)
    {
        inputs.forEach( input -> input.replaceSource( sourcesMap));
    }

    @Override
    public void switchSources( SourceId sourceIdOne, SourceId sourceIdTwo)
    {
        inputs.forEach( input -> input.switchSource( sourceIdOne, sourceIdTwo));
    }

    @Override
    public boolean validate( Configuration configuration)
    {
        return
            super.validate( configuration) &&
            validateReferencedSources( configuration, inputs,"inputs");
    }

    @Override
    public String toString()
    {
        StringBuffer Buffer = new StringBuffer();

        Buffer.append( "Mix = {\n");
        Buffer.append( super.toString());

        for( SourceWithVolume input: inputs)
        {
            Buffer.append( input);
        }

        Buffer.append( "}\n");

        return Buffer.toString();
    }
}
