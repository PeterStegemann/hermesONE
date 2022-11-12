package net.stegemann.configuration.source.input;

import net.stegemann.configuration.source.Source;
import net.stegemann.configuration.type.Number;

public final class Analog extends Input
{
	private final Number inputId;

	public Analog()
	{
		inputId = new Number( ANALOG_INPUT_MINIMUM, ANALOG_INPUT_MAXIMUM);
	}

	public Analog( Analog other)
	{
		super( other);

		inputId = new Number( other.inputId);
	}

	@Override
	public String toString()
	{
		StringBuffer Buffer = new StringBuffer();

		Buffer.append( "Analog = {\n");
		Buffer.append( super.toString());
		Buffer.append( " Input Id: " + inputId + "\n");
		Buffer.append( "}\n");

		return Buffer.toString();
	}

	@Override
	public Source clone()
	{
		return new Analog( this);
	}

	public Number getInputId()
	{
		return inputId;
	}
}
