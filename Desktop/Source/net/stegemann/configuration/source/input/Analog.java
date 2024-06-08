package net.stegemann.configuration.source.input;

import lombok.Getter;
import net.stegemann.configuration.source.Source;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.util.ConfigurationField;
import net.stegemann.io.xml.Names;

@Getter
@ConfigurationField( name = Names.SOURCE_INPUT_ANALOG)
public final class Analog extends Input
{
	@ConfigurationField( name = Names.SOURCE_INPUT_ANALOG_INPUT)
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
        return "Analog" +
				"{\n" +
				super.toString() +
				" Input Id: " + inputId + "\n" +
				"}\n";
	}

	@Override
	public Source clone()
	{
		return new Analog( this);
	}
}
