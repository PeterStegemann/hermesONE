package net.stegemann.io.serial.configuration.read.handler;

import net.stegemann.configuration.Model;
import net.stegemann.configuration.Models;
import net.stegemann.configuration.type.ValueOutOfRangeException;
import net.stegemann.io.serial.base.DesktopProtocol;

class ModelsHandler extends DesktopConnectionHandler
{
	private final Models models;

	private int modelId;

	public ModelsHandler( Models models)
	{
		this.models = models;

		modelId = 0;
	}

	@Override
	public void complexOpened( DesktopProtocol.Id id)
	{
		switch( id)
		{
			case Model :
			{
				Model newModel = new Model();

				pushHandler( new ModelHandler( models, newModel));

				try
				{
					newModel.getId().setValue( modelId);
				}
				catch( ValueOutOfRangeException ignored)
				{
				}

				// Always count up model id, it doesn't matter if a model was skipped.
				modelId++;
			}
			break;

			default : super.complexOpened( id); break;
		}
	}
}
