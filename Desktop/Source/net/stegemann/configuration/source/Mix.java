package net.stegemann.configuration.source;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import net.stegemann.configuration.Signal;
import net.stegemann.configuration.type.SourceId;
import net.stegemann.configuration.type.SourceWithVolume;

public final class Mix extends Source
{
	public static final int INPUT_SIGNAL_PER_VALUE = ( Signal.VALUE_RANGE / 600);

	public static final int INPUTS = 7;

	private final List< SourceWithVolume> inputs = new ArrayList< SourceWithVolume>();

	public Mix()
	{
		for( int Count = 0; Count < INPUTS; Count++)
		{
			inputs.add( new SourceWithVolume( Signal.MINIMUM_VALUE, Signal.MAXIMUM_VALUE,
											  INPUT_SIGNAL_PER_VALUE));
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

	@Override
	public String toString()
	{
		StringBuffer Buffer = new StringBuffer();

		Buffer.append( "Mix = {\n");
		Buffer.append( super.toString());

		for( SourceWithVolume CurrentSourceTupel: inputs)
		{
			Buffer.append( CurrentSourceTupel);
		}

		Buffer.append( "}\n");

		return Buffer.toString();
	}

	@Override
	public Source clone()
	{
		return new Mix( this);
	}

	@Override
	public void replaceSources( HashMap< SourceId, SourceId> sourcesMap)
	{
		for( SourceWithVolume input: inputs)
		{
			input.replaceSource( sourcesMap);
		}
	}

	public SourceWithVolume getInput( int Index)
	{
		return inputs.get( Index);
	}

	public List< SourceWithVolume> getInputs()
	{
		return inputs;
	}
}
