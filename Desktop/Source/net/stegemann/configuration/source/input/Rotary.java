package net.stegemann.configuration.source.input;

import net.stegemann.configuration.Signal;
import net.stegemann.configuration.source.Source;
import net.stegemann.configuration.type.Bool;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.ValueOutOfRangeException;
import net.stegemann.configuration.type.Volume;

public final class Rotary extends Input
{
	private final Number aInputId;
	private final Number bInputId;
	private final Bool store;
	private final Volume init;
	private final Volume step;
	private final Volume top;
	private final Volume bottom;

	public Rotary()
	{
		aInputId = new Number( DIGITAL_INPUT_MINIMUM, DIGITAL_INPUT_MAXIMUM);
		bInputId = new Number( DIGITAL_INPUT_MINIMUM, DIGITAL_INPUT_MAXIMUM);
		store = new Bool();
		init = new Volume( Signal.MINIMUM_VALUE,  Signal.MAXIMUM_VALUE, INIT_SIGNAL_PER_VALUE);

		try
		{
			step = new Volume( Signal.MINIMUM_VALUE, Signal.MAXIMUM_VALUE, STEP_SIGNAL_PER_VALUE,
					           STEP_DEFAULT_VALUE);
			top = new Volume( Signal.MINIMUM_VALUE, Signal.MAXIMUM_VALUE, TOP_SIGNAL_PER_VALUE,
							  TOP_DEFAULT_VALUE);
			bottom = new Volume( Signal.MINIMUM_VALUE, Signal.MAXIMUM_VALUE, BOTTOM_SIGNAL_PER_VALUE,
								 BOTTOM_DEFAULT_VALUE);
		}
		catch( ValueOutOfRangeException e)
		{
			throw new RuntimeException( e);
		}
	}

	public Rotary( Rotary other)
	{
		super( other);

		aInputId = new Number( other.aInputId);
		bInputId = new Number( other.bInputId);
		store = new Bool( other.store);
		init = new Volume( other.init);
		step = new Volume( other.step);
		top = new Volume( other.top);
		bottom = new Volume( other.bottom);
	}

	@Override
	public String toString()
	{
		StringBuffer Buffer = new StringBuffer();

		Buffer.append( "Rotary = {\n");
		Buffer.append( super.toString());
		Buffer.append( " A Input Id: " + aInputId + "\n");
		Buffer.append( " B Input Id: " + bInputId + "\n");
		Buffer.append( " Store: " + store + "\n");
		Buffer.append( " Init: " + init + "\n");
		Buffer.append( " Step: " + step + "\n");
		Buffer.append( " Top: " + top + "\n");
		Buffer.append( " Bottom: " + bottom + "\n");
		Buffer.append( "}\n");

		return Buffer.toString();
	}

	@Override
	public Source clone()
	{
		return new Rotary( this);
	}

	public Number getAInputId()
	{
		return aInputId;
	}

	public Number getBInputId()
	{
		return bInputId;
	}

	public Bool getStore()
	{
		return store;
	}

	public Number getInit()
	{
		return init;
	}

	public Number getStep()
	{
		return step;
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
