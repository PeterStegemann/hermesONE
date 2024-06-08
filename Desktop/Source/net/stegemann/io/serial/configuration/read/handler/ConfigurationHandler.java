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
			case Owner :
			{
				readValue( UseSystem.getOwner(), textContent);
			}
			break;

			case AnalogInputs :
			{
				readValue( UseSystem.getAnalogInputs(), textContent);
			}
			break;

			case DigitalInputs :
			{
				readValue( UseSystem.getDigitalInputs(), textContent);
			}
			break;

			case OutputChannels :
			{
				readValue( UseSystem.getOutputChannels(), textContent);
			}
			break;

			case Outputs :
			{
				readValue( UseSystem.getOutputs(), textContent);
			}
			break;

			case SetupBacklight :
			{
				readValue( UseSystem.getSetupBacklight(), textContent);
			}
			break;

			case SetupBlankTime :
			{
				readValue( UseSystem.getSetupBlankTime(), textContent);
			}
			break;

			case StatusBacklight :
			{
				readValue( UseSystem.getStatusBacklight(), textContent);
			}
			break;

			case StatusContrast :
			{
				readValue( UseSystem.getStatusContrast(), textContent);
			}
			break;

			case StatusBlankTime :
			{
				readValue( UseSystem.getStatusBlankTime(), textContent);
			}
			break;

			case StatusInverted :
			{
				readValue( UseSystem.getStatusInverted(), textContent);
			}
			break;

			case SelectedModel :
			{
				readValue( UseSystem.getSelectedModel(), textContent);
			}
			break;

			default : super.valueRead( id, textContent); break;
		}
	}
}
