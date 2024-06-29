package net.stegemann.configuration;

import lombok.Getter;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.util.ConfigurationField;
import net.stegemann.io.xml.Names;

@Getter
public class Calibration
{
    public static final int MINIMUM_VALUE = 0;
    public static final int MAXIMUM_VALUE = 1023;

    @ConfigurationField( name = Names.CALIBRATION_HIGH)
    private final Number high = new Number( MINIMUM_VALUE, MAXIMUM_VALUE);
    @ConfigurationField( name = Names.CALIBRATION_CENTER)
    private final Number center = new Number( MINIMUM_VALUE, MAXIMUM_VALUE);
    @ConfigurationField( name = Names.CALIBRATION_LOW)
    private final Number low = new Number( MINIMUM_VALUE, MAXIMUM_VALUE);

    @Override
    public String toString()
    {
        return String.format
        (
            """
            Calibration
            {
                high: %s
                center: %s
                low: %s
            }
            """,
            high, center, low
		);
    }
}
