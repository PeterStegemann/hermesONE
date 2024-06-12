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
            case StatusSources -> pushHandler( new StatusSourcesHandler( model));
            case StatusTimers -> pushHandler(new StatusTimesHandler( model));
            case Channels -> pushHandler(new ChannelsHandler( model.getChannels()));
            case ModelProxyReferences -> pushHandler( new ProxyReferencesHandler( model.getProxyReferences()));

            default -> super.complexOpened( id);
        }
    }

    @Override
    public void complexClosed( DesktopProtocol.Id id)
    {
		if( model.getState() == Model.State.USED)
		{
            models.addModel( model);

            getConfigurationProgress().setModelsCount( models.getCount());
        }
	}

    @Override
    public void valueRead( DesktopProtocol.Id id, String textContent)
    {
        switch( id)
        {
            case ModelName -> readValue( model.getName(), textContent);
            case ModelState -> model.setState( Utility.convertModelState(textContent));
            case ModelType -> readValue( model.getTypeId(), textContent);
            case ModelRFMode -> readValue( model.getRfMode(), textContent);

            default -> super.valueRead( id, textContent);
        }
    }
}
