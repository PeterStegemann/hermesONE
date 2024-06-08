package net.stegemann.configuration.source.input;

import lombok.Getter;
import net.stegemann.configuration.Signal;
import net.stegemann.configuration.source.Source;
import net.stegemann.configuration.type.Bool;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.ValueOutOfRangeException;
import net.stegemann.configuration.type.Volume;
import net.stegemann.configuration.util.ConfigurationField;
import net.stegemann.io.xml.Names;

@Getter
@ConfigurationField( name = Names.SOURCE_INPUT_BUTTON)
public final class Button extends Input
{
	@ConfigurationField( name = Names.SOURCE_INPUT_BUTTON_INPUT)
	private final Number inputId;
	@ConfigurationField( name = Names.SOURCE_INPUT_BUTTON_INIT)
	private final Volume init;
	@ConfigurationField( name = Names.SOURCE_INPUT_BUTTON_STORE)
	private final Bool store;
	@ConfigurationField( name = Names.SOURCE_INPUT_BUTTON_TOGGLE)
	private final Bool toggle;
	@ConfigurationField( name = Names.SOURCE_INPUT_BUTTON_TOP)
	private final Volume top;
	@ConfigurationField( name = Names.SOURCE_INPUT_BUTTON_BOTTOM)
	private final Volume bottom;

	public Button()
	{
		inputId = new Number( DIGITAL_INPUT_MINIMUM, DIGITAL_INPUT_MAXIMUM);
		init = new Volume( Signal.MINIMUM_VALUE, Signal.MAXIMUM_VALUE, INIT_SIGNAL_PER_VALUE);
		store = new Bool();
		toggle = new Bool();

		try
		{
			top = new Volume( Signal.MINIMUM_VALUE, Signal.MAXIMUM_VALUE, TOP_SIGNAL_PER_VALUE, TOP_DEFAULT_VALUE);
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
        return "Button = {\n" +
				super.toString() +
				" Input Id: " + inputId + "\n" +
				" Init: " + init + "\n" +
				" Store: " + store + "\n" +
				" Toggle: " + toggle + "\n" +
				" Top: " + top + "\n" +
				" Bottom: " + bottom + "\n" +
				"}\n";
	}

	@Override
	public Source clone()
	{
		return new Button( this);
	}
}
