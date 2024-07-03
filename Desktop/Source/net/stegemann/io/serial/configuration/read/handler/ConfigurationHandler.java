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
		System system = configuration.getSystem();

        switch( id)
        {
            case AnalogInputs -> readValue( system.getAnalogInputs(), textContent);
            case DigitalInputs -> readValue( system.getDigitalInputs(), textContent);
            case OutputChannels -> readValue( system.getOutputChannels(), textContent);
            case Outputs -> readValue( system.getOutputs(), textContent);
            case Owner -> readValue( system.getOwner(), textContent);
            case SelectedModel -> readValue( system.getSelectedModel(), textContent);
            case SetupBacklight -> readValue( system.getSetupBacklight(), textContent);
            case SetupBlankTime -> readValue( system.getSetupBlankTime(), textContent);
            case StatusBacklight -> readValue( system.getStatusBacklight(), textContent);
            case StatusBlankTime -> readValue( system.getStatusBlankTime(), textContent);
            case StatusContrast -> readValue( system.getStatusContrast(), textContent);
            case StatusInverted -> readValue( system.getStatusInverted(), textContent);

            case StorageModels ->
            {
                 readValue( system.getStorageModels(), textContent);
                 getConfigurationProgress().setModelsMaximum( system.getStorageModels().getValue());
            }

            case StorageSources ->
            {
                readValue( system.getStorageSources(), textContent);
                getConfigurationProgress().setSourcesMaximum( system.getStorageSources().getValue());
            }

            case StorageTypes ->
            {
                readValue( system.getStorageTypes(), textContent);
                getConfigurationProgress().setTypesMaximum( system.getStorageTypes().getValue());
            }

            default -> super.valueRead( id, textContent);
        }
    }
}
