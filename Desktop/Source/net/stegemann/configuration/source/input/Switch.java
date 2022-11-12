package net.stegemann.configuration.source.input;

import net.stegemann.configuration.Signal;
import net.stegemann.configuration.source.Source;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.ValueOutOfRangeException;
import net.stegemann.configuration.type.Volume;

public final class Switch extends Input
{
	private final Number lowInputId;
	private final Number highInputId;
	private final Volume top;
	private final Volume bottom;

	public Switch()
	{
		lowInputId = new Number( DIGITAL_INPUT_MINIMUM, DIGITAL_INPUT_MAXIMUM);
		highInputId = new Number( DIGITAL_INPUT_MINIMUM, DIGITAL_INPUT_MAXIMUM);

		try
		{
			top = new Volume( Signal.MINIMUM_VALUE, Signal.MAXIMUM_VALUE, TOP_SIGNAL_PER_VALUE,
							  TOP_DEFAULT_VALUE);
			bottom = new Volume( Signal.MINIMUM_VALUE, Signal.MAXIMUM_VALUE, BOTTOM_SIGNAL_PER_VALUE,
								 BOTTOM_DEFAULT_VALUE);
		}
		catch( ValueOutOfRangeException reason)
		{
			throw new RuntimeException( reason);
		}
	}

	public Switch( Switch other)
	{
		super( other);

		lowInputId = new Number( other.lowInputId);
		highInputId = new Number( other.highInputId);
		top = new Volume( other.top);
		bottom = new Volume( other.bottom);
	}

	@Override
	public String toString()
	{
		StringBuffer Buffer = new StringBuffer();

		Buffer.append( "Switch = {\n");
		Buffer.append( super.toString());
		Buffer.append( " LowInput Id: " + lowInputId + "\n");
		Buffer.append( " HighInput Id: " + highInputId + "\n");
		Buffer.append( " Top: " + top + "\n");
		Buffer.append( " Bottom: " + bottom + "\n");
		Buffer.append( "}\n");

		return Buffer.toString();
	}

	@Override
	public Source clone()
	{
		return new Switch( this);
	}

	public Number getLowInputId()
	{
		return lowInputId;
	}

	public Number getHighInputId()
	{
		return highInputId;
	}

	public Number getTop()
	{
		return top;
	}

	public Number getBottom()
	{
		return bottom;
	}
}
