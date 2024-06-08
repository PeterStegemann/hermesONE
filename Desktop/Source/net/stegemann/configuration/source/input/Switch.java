package net.stegemann.configuration.source.input;

import lombok.Getter;
import net.stegemann.configuration.Signal;
import net.stegemann.configuration.source.Source;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.ValueOutOfRangeException;
import net.stegemann.configuration.type.Volume;
import net.stegemann.configuration.util.ConfigurationField;
import net.stegemann.io.xml.Names;

import static net.stegemann.misc.Utility.indent;

@Getter
@ConfigurationField( name = Names.SOURCE_INPUT_SWITCH)
public final class Switch extends Input
{
    @ConfigurationField( name = Names.SOURCE_INPUT_SWITCH_LOW_INPUT)
    private final Number lowInputId;
    @ConfigurationField( name = Names.SOURCE_INPUT_SWITCH_HIGH_INPUT)
    private final Number highInputId;
    @ConfigurationField( name = Names.SOURCE_INPUT_SWITCH_TOP)
    private final Volume top;
    @ConfigurationField( name = Names.SOURCE_INPUT_SWITCH_BOTTOM)
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
        return String.format( """
            Switch
            {
                %s
                lowInputId: %s
                highInputId: %s
                top: %s
                bottom: %s
            }""",
            indent( super.toString()),
            lowInputId,
            highInputId,
            top,
            bottom
        );
    }

    @Override
    public Source clone()
    {
        return new Switch( this);
    }
}
