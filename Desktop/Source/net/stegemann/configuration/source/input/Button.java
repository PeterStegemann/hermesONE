package net.stegemann.configuration.source.input;

import lombok.Getter;
import net.stegemann.configuration.source.Source;
import net.stegemann.configuration.type.Bool;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.ValueOutOfRangeException;
import net.stegemann.configuration.type.Volume;
import net.stegemann.configuration.util.ConfigurationField;
import net.stegemann.io.xml.Names;

import static net.stegemann.misc.Utility.indent;

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
		init = new Volume( INIT_SIGNAL_PER_VALUE);
		store = new Bool();
		toggle = new Bool();

		try
		{
			top = new Volume( TOP_SIGNAL_PER_VALUE, TOP_DEFAULT_VALUE);
			bottom = new Volume( BOTTOM_SIGNAL_PER_VALUE, BOTTOM_DEFAULT_VALUE);
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
	public Source duplicate()
	{
		return new Button( this);
	}

	@Override
	public String toString()
	{
        return String.format
        (
            """
            Button
            {
                %s
                inputId: %s
                init: %s
                store: %s
                toggle: %s
                top: %s
                bottom: %s
            }
            """,
            indent( super.toString()), inputId, init, store, toggle, top, bottom
        );
    }
}
