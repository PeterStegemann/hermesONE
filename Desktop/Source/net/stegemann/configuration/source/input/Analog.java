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
}
