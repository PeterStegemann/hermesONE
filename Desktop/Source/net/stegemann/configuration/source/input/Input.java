package net.stegemann.configuration.source.input;

import net.stegemann.configuration.Signal;
import net.stegemann.configuration.source.Source;
import net.stegemann.configuration.type.SourceId;

import java.util.HashMap;

public abstract class Input extends Source
{
	public static final int INIT_SIGNAL_PER_VALUE = ( Signal.VALUE_RANGE / 200);
	public static final int STEP_SIGNAL_PER_VALUE = ( Signal.VALUE_RANGE / 200);
	public static final int TOP_SIGNAL_PER_VALUE = ( Signal.VALUE_RANGE / 200);
	public static final int BOTTOM_SIGNAL_PER_VALUE = ( Signal.VALUE_RANGE / 200);

	public static final int STEP_DEFAULT_VALUE = 5;
	public static final int TOP_DEFAULT_VALUE = ( Signal.MAXIMUM_VALUE / TOP_SIGNAL_PER_VALUE);
	public static final int BOTTOM_DEFAULT_VALUE = ( Signal.MINIMUM_VALUE / BOTTOM_SIGNAL_PER_VALUE);

	public static final int ANALOG_INPUT_MINIMUM = 0;
	public static final int ANALOG_INPUT_MAXIMUM = 5;
	public static final int DIGITAL_INPUT_MINIMUM = 0;
	public static final int DIGITAL_INPUT_MAXIMUM = 39;

	public Input()
	{
	}

	public Input( Input other)
	{
		super( other);
	}

	@Override
	public String toString()
	{
        return "Input = {\n" +
				super.toString() +
				"}\n";
	}

	@Override
	public void replaceSources( HashMap< SourceId, SourceId> sourcesMap)
	{
	}
}
