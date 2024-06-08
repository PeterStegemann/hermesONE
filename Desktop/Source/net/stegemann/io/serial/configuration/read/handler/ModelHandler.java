package net.stegemann.io.serial.configuration.read.handler;

import net.stegemann.configuration.Model;
import net.stegemann.configuration.Models;
import net.stegemann.io.Utility;
import net.stegemann.io.serial.base.DesktopProtocol;

class ModelHandler extends DesktopConnectionHandler
{
	private final Models models;
	private final Model model;

	public ModelHandler( Models models, Model model)
	{
		this.models = models;
		this.model = model;
	}

	@Override
	public void complexOpened( DesktopProtocol.Id id)
	{
		switch( id)
		{
			case StatusSources :
			{
				pushHandler( new StatusSourcesHandler( model));
			}
			break;

			case StatusTimers :
			{
				pushHandler( new StatusTimesHandler( model));
			}
			break;

			case Channels :
			{
				pushHandler( new ChannelsHandler( model.getChannels()));
			}
			break;

			case ModelProxyReferences :
			{
				pushHandler( new ProxyReferencesHandler( model.getProxyReferences()));
			}
			break;

			default : super.complexOpened( id); break;
		}
	}

	@Override
	public void complexClosed( DesktopProtocol.Id id)
	{
		if( model.getState() == Model.State.USED)
		{
			models.addModel( model);

			getConfigurationProgress().setModelCount( models.getCount());
		}
	}

	@Override
	public void valueRead( DesktopProtocol.Id id, String textContent)
	{
		switch( id)
		{
			case ModelName :
			{
				readValue( model.getName(), textContent);
			}
			break;

			case ModelState :
			{
				model.setState( Utility.convertModelState( textContent));
			}
			break;

			case ModelType :
			{
				readValue( model.getTypeId(), textContent);
			}
			break;

			case ModelRFMode :
			{
				readValue( model.getRfMode(), textContent);
			}
			break;

			default : super.valueRead( id, textContent); break;
		}
	}
}
