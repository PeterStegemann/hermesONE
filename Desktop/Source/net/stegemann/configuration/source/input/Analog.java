package net.stegemann.configuration.source.input;

import lombok.Getter;
import net.stegemann.configuration.source.Source;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.util.ConfigurationField;
import net.stegemann.io.xml.Names;

import static net.stegemann.misc.Utility.indent;

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
    public Source duplicate()
    {
        return new Analog( this);
    }

    @Override
    public String toString()
    {
        return String.format
        (
            """
            Analog
            {
                %s
                inputId: %s
            }
            """,
            indent( super.toString()), inputId
        );
    }
}
