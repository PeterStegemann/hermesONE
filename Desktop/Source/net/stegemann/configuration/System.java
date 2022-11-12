package net.stegemann.configuration;

import net.stegemann.configuration.type.Bool;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.Text;
import net.stegemann.configuration.type.ValueOutOfRangeException;

public class System
{
	public static final int SYSTEM_ANALOG_INPUTS_DEFAULT = 6;
	public static final int SYSTEM_DIGITAL_INPUTS_DEFAULT = 16;
	public static final int SYSTEM_OUTPUTS_DEFAULT = 2;
	public static final int SYSTEM_OUTPUT_CHANNELS_DEFAULT = 9;

	public static final int SYSTEM_BACKLIGHT_MINIMUM = 0;
	public static final int SYSTEM_BACKLIGHT_MAXIMUM = 100;
	public static final int SYSTEM_CONTRAST_MINIMUM = 0;
	public static final int SYSTEM_CONTRAST_MAXIMUM = 100;
	public static final int SYSTEM_BLANKTIME_MINIMUM = 0;
	public static final int SYSTEM_BLANKTIME_MAXIMUM = 5 * 60;

	private final Number analogInputs = new Number();
	private final Number digitalInputs = new Number();
	private final Number outputChannels = new Number();
	private final Number outputs = new Number();

	private final Text owner = new Text();
	private final Number setupBacklight = new Number( SYSTEM_BACKLIGHT_MINIMUM, SYSTEM_BACKLIGHT_MAXIMUM);
	private final Number setupBlankTime = new Number( SYSTEM_BLANKTIME_MINIMUM, SYSTEM_BLANKTIME_MAXIMUM);
	private final Number statusBacklight = new Number( SYSTEM_BACKLIGHT_MINIMUM, SYSTEM_BACKLIGHT_MAXIMUM);
	private final Number statusContrast = new Number( SYSTEM_CONTRAST_MINIMUM, SYSTEM_CONTRAST_MAXIMUM);
	private final Number statusBlankTime = new Number( SYSTEM_BLANKTIME_MINIMUM, SYSTEM_BLANKTIME_MAXIMUM);
	private final Bool statusInverted = new Bool();
	private final Number selectedModel = new Number( Model.MODEL_START, Model.MODEL_END);

	private final Battery battery = new Battery();
	private final Calibrations calibrations = new Calibrations();
	private final PPMs ppms = new PPMs();

	public System()
	{
		clear();
	}

	@Override
	public String toString()
	{
		StringBuffer Buffer = new StringBuffer();

		Buffer.append( "System = {\n");
		Buffer.append( " Analog Inputs: " + analogInputs + "\n");
		Buffer.append( " Digital Inputs: " + digitalInputs + "\n");
		Buffer.append( " Output Channels: " + outputChannels + "\n");
		Buffer.append( " Outputs: " + outputs + "\n");
		Buffer.append( " Owner: " + owner + "\n");
		Buffer.append( " Setup Backlight: " + setupBacklight + "\n");
		Buffer.append( " Setup BlankTime: " + setupBlankTime + "\n");
		Buffer.append( " Status Backlight: " + statusBacklight + "\n");
		Buffer.append( " Status Contrast: " + statusContrast + "\n");
		Buffer.append( " Status BlankTime: " + statusBlankTime + "\n");
		Buffer.append( " Status Inverted: " + statusInverted + "\n");
		Buffer.append( " Selected Model: " + selectedModel + "\n");
		Buffer.append( battery);
		Buffer.append( calibrations);
		Buffer.append( ppms);
		Buffer.append( "}\n");

		return Buffer.toString();
	}

	public void clear()
	{
		try
		{
			analogInputs.setValue( SYSTEM_ANALOG_INPUTS_DEFAULT);
			digitalInputs.setValue( SYSTEM_DIGITAL_INPUTS_DEFAULT);
			outputChannels.setValue( SYSTEM_OUTPUT_CHANNELS_DEFAULT);
			outputs.setValue( SYSTEM_OUTPUTS_DEFAULT);

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

	public Number getAnalogInputs()
	{
		return analogInputs;
	}

	public Number getDigitalInputs()
	{
		return digitalInputs;
	}

	public Number getOutputChannels()
	{
		return outputChannels;
	}

	public Number getOutputs()
	{
		return outputs;
	}

	public Text getOwner()
	{
		return owner;
	}

	public Number getSetupBacklight()
	{
		return setupBacklight;
	}

	public Number getSetupBlankTime()
	{
		return setupBlankTime;
	}

	public Number getStatusBacklight()
	{
		return statusBacklight;
	}

	public Number getStatusContrast()
	{
		return statusContrast;
	}

	public Number getStatusBlankTime()
	{
		return statusBlankTime;
	}

	public Bool getStatusInverted()
	{
		return statusInverted;
	}

	public Number getSelectedModel()
	{
		return selectedModel;
	}

	public Battery getBattery()
	{
		return battery;
	}

	public Calibrations getCalibrations()
	{
		return calibrations;
	}

	public PPMs getPPMs()
	{
		return ppms;
	}
}
