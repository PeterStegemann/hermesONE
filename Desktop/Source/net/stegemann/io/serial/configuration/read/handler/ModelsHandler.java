package net.stegemann.io.serial.configuration.read.handler;

import net.stegemann.configuration.Model;
import net.stegemann.configuration.Models;
import net.stegemann.configuration.type.ValueOutOfRangeException;
import net.stegemann.io.serial.base.DesktopProtocol;

class ModelsHandler extends DesktopConnectionHandler
{
    private final Models models;
    private int modelId = 0;

    public ModelsHandler( Models models)
    {
        this.models = models;
    }

    @Override
    public void complexOpened( DesktopProtocol.Id id)
    {
        switch( id)
        {
            case Model ->
            {
                Model model = new Model();

                try
                {
                    model.getId().setValue( modelId);
                }
                catch( ValueOutOfRangeException ignored) {}

                pushHandler( new ModelHandler( models, model));

                // Always count up model id, it doesn't matter if a model was skipped.
                modelId++;
            }

            default -> super.complexOpened( id);
        }
    }
}
