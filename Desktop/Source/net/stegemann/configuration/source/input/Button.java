package net.stegemann.configuration.source.input;

import net.stegemann.configuration.Signal;
import net.stegemann.configuration.source.Source;
import net.stegemann.configuration.type.Bool;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.ValueOutOfRangeException;
import net.stegemann.configuration.type.Volume;

public final class Button extends Input
{
	private final Number inputId;
	private final Volume init;
	private final Bool store;
	private final Bool toggle;
	private final Volume top;
	private final Volume bottom;

	public Button()
	{
		inputId = new Number( DIGITAL_INPUT_MINIMUM, DIGITAL_INPUT_MAXIMUM);
		init = new Volume( Signal.MINIMUM_VALUE, Signal.MAXIMUM_VALUE, INIT_SIGNAL_PER_VALUE);
		store = new Bool();
		toggle = new Bool();

		try
		{
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

	public Button( Button other)
	{
		super( other);

		inputId = new Number( other.inputId);
		init = new Volume( other.init);
		store = new Bool( other.store);
		toggle = new Bool( other.toggle);
		top = new Volume( other.top);
		bottom = new Volume( other.bottom);
	}

	@Override
	public String toString()
	{
		StringBuffer Buffer = new StringBuffer();

		Buffer.append( "Button = {\n");
		Buffer.append( super.toString());
		Buffer.append( " Input Id: " + inputId + "\n");
		Buffer.append( " Init: " + init + "\n");
		Buffer.append( " Store: " + store + "\n");
		Buffer.append( " Toggle: " + toggle + "\n");
		Buffer.append( " Top: " + top + "\n");
		Buffer.append( " Bottom: " + bottom + "\n");
		Buffer.append( "}\n");

		return Buffer.toString();
	}

	@Override
	public Source clone()
	{
		return new Button( this);
	}

	public Number getInputId()
	{
		return inputId;
	}

	public Volume getInit()
	{
		return init;
	}

	public Bool getStore()
	{
		return store;
	}

	public Bool getToggle()
	{
		return toggle;
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
