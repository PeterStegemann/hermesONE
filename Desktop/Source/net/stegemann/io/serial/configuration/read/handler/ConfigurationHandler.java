package net.stegemann.io.serial.configuration.read.handler;

import net.stegemann.configuration.Configuration;
import net.stegemann.configuration.System;
import net.stegemann.io.serial.base.DesktopProtocol;
import net.stegemann.io.serial.configuration.read.handler.source.SourcesHandler;

class ConfigurationHandler extends DesktopConnectionHandler
{
	final Configuration configuration;

	public ConfigurationHandler( Configuration configuration)
	{
		this.configuration = configuration;
	}

	@Override
	public void complexOpened( DesktopProtocol.Id id)
	{
		System UseSystem = configuration.getSystem();

		switch( id)
		{
			case Battery -> pushHandler( new BatteryHandler( UseSystem.getBattery()));
			case Calibrations -> pushHandler( new CalibrationsHandler( UseSystem.getCalibrations()));
			case Models -> pushHandler( new ModelsHandler( configuration.getModels()));
			case Types -> pushHandler( new TypesHandler( configuration.getTypes()));
			case Sources -> pushHandler( new SourcesHandler( configuration.getSources()));
			case PPMs -> pushHandler( new PPMsHandler( UseSystem.getPpms()));

			default -> super.complexOpened( id);
		}
	}

	@Override
	public void complexClosed( DesktopProtocol.Id id)
	{
		configuration.fill();
	}

	@Override
	public void valueRead( DesktopProtocol.Id id, String textContent)
	{
		System UseSystem = configuration.getSystem();

        switch( id)
        {
            case Owner -> readValue( UseSystem.getOwner(), textContent);
            case AnalogInputs -> readValue( UseSystem.getAnalogInputs(), textContent);
            case DigitalInputs -> readValue( UseSystem.getDigitalInputs(), textContent);
            case OutputChannels -> readValue( UseSystem.getOutputChannels(), textContent);
            case Outputs -> readValue( UseSystem.getOutputs(), textContent);
            case SetupBacklight -> readValue( UseSystem.getSetupBacklight(), textContent);
            case SetupBlankTime -> readValue( UseSystem.getSetupBlankTime(), textContent);
            case StatusBacklight -> readValue( UseSystem.getStatusBacklight(), textContent);
            case StatusContrast -> readValue( UseSystem.getStatusContrast(), textContent);
            case StatusBlankTime -> readValue( UseSystem.getStatusBlankTime(), textContent);
            case StatusInverted -> readValue( UseSystem.getStatusInverted(), textContent);
            case SelectedModel -> readValue( UseSystem.getSelectedModel(), textContent);

            default -> super.valueRead( id, textContent);
        }
	}
}
