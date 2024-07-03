package net.stegemann.configuration;

import lombok.Getter;
import net.stegemann.configuration.type.*;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.util.ConfigurationField;
import net.stegemann.io.xml.Names;
import static net.stegemann.misc.Utility.indent;

@Getter
public class System
{
	public static final int SYSTEM_ANALOG_INPUTS_DEFAULT	= 6;
	public static final int SYSTEM_DIGITAL_INPUTS_DEFAULT	= 16;
	public static final int SYSTEM_OUTPUTS_DEFAULT			= 2;
	public static final int SYSTEM_OUTPUT_CHANNELS_DEFAULT	= 9;

    public static final int SYSTEM_STORAGE_TYPES_DEFAULT   = 0;
    public static final int SYSTEM_STORAGE_MODELS_DEFAULT  = 0;
    public static final int SYSTEM_STORAGE_SOURCES_DEFAULT = 0;

	public static final int SYSTEM_BACKLIGHT_MINIMUM	= 0;
	public static final int SYSTEM_BACKLIGHT_MAXIMUM	= 100;
	public static final int SYSTEM_CONTRAST_MINIMUM		= 0;
	public static final int SYSTEM_CONTRAST_MAXIMUM		= 100;
	public static final int SYSTEM_BLANKTIME_MINIMUM	= 0;
	public static final int SYSTEM_BLANKTIME_MAXIMUM	= 5 * 60;

	@ConfigurationField( name = Names.SYSTEM_ANALOG_INPUTS)
	private final Number analogInputs = new Number();
	@ConfigurationField( name = Names.SYSTEM_DIGITAL_INPUTS)
	private final Number digitalInputs = new Number();
	@ConfigurationField( name = Names.SYSTEM_OUTPUT_CHANNELS)
	private final Number outputChannels = new Number();
	@ConfigurationField( name = Names.SYSTEM_OUTPUTS)
	private final Number outputs = new Number();

    @ConfigurationField( name = Names.SYSTEM_STORAGE_TYPES)
    private final Number storageTypes = new Number();
    @ConfigurationField( name = Names.SYSTEM_STORAGE_MODELS)
    private final Number storageModels = new Number();
    @ConfigurationField( name = Names.SYSTEM_STORAGE_SOURCES)
    private final Number storageSources = new Number();

	@ConfigurationField( name = Names.SYSTEM_OWNER)
	private final Text owner = new Text();
	@ConfigurationField( name = Names.SYSTEM_SETUP_BACKLIGHT)
	private final Number setupBacklight = new Number( SYSTEM_BACKLIGHT_MINIMUM, SYSTEM_BACKLIGHT_MAXIMUM);
	@ConfigurationField( name = Names.SYSTEM_SETUP_BLANK_TIME)
	private final Number setupBlankTime = new Number( SYSTEM_BLANKTIME_MINIMUM, SYSTEM_BLANKTIME_MAXIMUM);
	@ConfigurationField( name = Names.SYSTEM_STATUS_BACKLIGHT)
	private final Number statusBacklight = new Number( SYSTEM_BACKLIGHT_MINIMUM, SYSTEM_BACKLIGHT_MAXIMUM);
	@ConfigurationField( name = Names.SYSTEM_STATUS_CONTRAST)
	private final Number statusContrast = new Number( SYSTEM_CONTRAST_MINIMUM, SYSTEM_CONTRAST_MAXIMUM);
	@ConfigurationField( name = Names.SYSTEM_STATUS_BLANK_TIME)
	private final Number statusBlankTime = new Number( SYSTEM_BLANKTIME_MINIMUM, SYSTEM_BLANKTIME_MAXIMUM);
	@ConfigurationField( name = Names.SYSTEM_STATUS_INVERTED)
	private final Bool statusInverted = new Bool();
	@ConfigurationField( name = Names.SYSTEM_SELECTED_MODEL)
	private final ModelId selectedModel = new ModelId( Model.MODEL_START, Model.MODEL_END);

	@ConfigurationField( name = Names.BATTERY)
	private final Battery battery = new Battery();
	@ConfigurationField( name = Names.CALIBRATIONS, itemName = Names.CALIBRATION)
	private final Calibrations calibrations = new Calibrations();
	@ConfigurationField( name = Names.PPMS, itemName = Names.PPM)
	private final PPMs ppms = new PPMs();

	public System()
	{
		clear();
	}

	@Override
	public String toString()
	{
        return String.format( """
            System
            {
                analogInputs: %s
                digitalInputs: %s
                outputChannels: %s
                storageTypes: %s
                storageModels: %s
                storageSources: %s
                outputs: %s
                owner: %s
                setupBacklight: %s
                setupBlankTime: %s
                statusBacklight: %s
                statusContrast: %s
                statusBlankTime: %s
                statusInverted: %s
                selectedModel: %s
                battery: %s
                calibrations: %s
                ppms: %s
            }
            """,
            analogInputs, digitalInputs, outputChannels,
            storageTypes, storageModels, storageSources,
            outputs, owner,
            setupBacklight, setupBlankTime,
            statusBacklight, statusContrast, statusBlankTime, statusInverted,
            selectedModel,
            indent( battery), indent( calibrations), indent( ppms)
        );
	}

	public void clear()
	{
		try
		{
			analogInputs.setValue( SYSTEM_ANALOG_INPUTS_DEFAULT);
			digitalInputs.setValue( SYSTEM_DIGITAL_INPUTS_DEFAULT);
			outputChannels.setValue( SYSTEM_OUTPUT_CHANNELS_DEFAULT);
			outputs.setValue( SYSTEM_OUTPUTS_DEFAULT);

            storageTypes.setValue( SYSTEM_STORAGE_TYPES_DEFAULT);
            storageModels.setValue( SYSTEM_STORAGE_MODELS_DEFAULT);
            storageSources.setValue( SYSTEM_STORAGE_SOURCES_DEFAULT);

			owner.setValue( "");
			setupBacklight.setValue( SYSTEM_BACKLIGHT_MAXIMUM / 2);
			setupBlankTime.setValue( SYSTEM_BLANKTIME_MAXIMUM / 2);
			statusBacklight.setValue( SYSTEM_BACKLIGHT_MAXIMUM / 2);
			statusContrast.setValue( SYSTEM_CONTRAST_MAXIMUM / 2);
			statusBlankTime.setValue( SYSTEM_BLANKTIME_MAXIMUM / 2);
			statusInverted.setValue( false);
			selectedModel.setValue( Model.MODEL_START);

			battery.clear();
			calibrations.clear( analogInputs.getValue());
			ppms.clear( outputs.getValue(), outputChannels.getValue());
		}
		catch( ValueOutOfRangeException reason)
		{
			throw new RuntimeException( reason);
		}
	}

	public void fill()
	{
		calibrations.fill( analogInputs.getValue());
		ppms.fill( outputs.getValue(), outputChannels.getValue());
	}
}
