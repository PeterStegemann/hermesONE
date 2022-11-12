package net.stegemann.configuration.source.input;

import net.stegemann.configuration.Signal;
import net.stegemann.configuration.source.Source;
import net.stegemann.configuration.type.Bool;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.ValueOutOfRangeException;
import net.stegemann.configuration.type.Volume;

public final class Ticker extends Input
{
	private final Number lowInputId;
	private final Number highInputId;
	private final Volume init;
	private final Volume step;
	private final Bool store;
	private final Volume top;
	private final Volume bottom;

	public Ticker()
	{
		lowInputId = new Number( DIGITAL_INPUT_MINIMUM, DIGITAL_INPUT_MAXIMUM);
		highInputId = new Number( DIGITAL_INPUT_MINIMUM, DIGITAL_INPUT_MAXIMUM);
		init = new Volume( Signal.MINIMUM_VALUE, Signal.MAXIMUM_VALUE, INIT_SIGNAL_PER_VALUE);
		store = new Bool();

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

	public Ticker( Ticker other)
	{
		super( other);

		lowInputId = new Number( other.lowInputId);
		highInputId = new Number( other.highInputId);
		init = new Volume( other.init);
		store = new Bool( other.store);
		step = new Volume( other.step);
		top = new Volume( other.top);
		bottom = new Volume( other.bottom);
	}

	@Override
	public String toString()
	{
		StringBuffer Buffer = new StringBuffer();

		Buffer.append( "Ticker = {\n");
		Buffer.append( super.toString());
		Buffer.append( " Low Input Id: " + lowInputId + "\n");
		Buffer.append( " High Input Id: " + highInputId + "\n");
		Buffer.append( " Init: " + init + "\n");
		Buffer.append( " Step: " + step + "\n");
		Buffer.append( " Store: " + store + "\n");
		Buffer.append( " Top: " + top + "\n");
		Buffer.append( " Bottom: " + bottom + "\n");
		Buffer.append( "}\n");

		return Buffer.toString();
	}

	@Override
	public Source clone()
	{
		return new Ticker( this);
	}

	public Number getLowInputId()
	{
		return lowInputId;
	}

	public Number getHighInputId()
	{
		return highInputId;
	}

	public Volume getInit()
	{
		return init;
	}

	public Volume getStep()
	{
		return step;
	}

	public Bool getStore()
	{
		return store;
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
