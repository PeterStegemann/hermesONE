package net.stegemann.io.serial.configuration.read.handler;

import net.stegemann.configuration.Model;
import net.stegemann.configuration.Type;
import net.stegemann.configuration.Types;
import net.stegemann.configuration.type.ValueOutOfRangeException;
import net.stegemann.io.serial.base.DesktopProtocol;

class TypesHandler extends DesktopConnectionHandler
{
    private final Types types;
    private int typeId = 0;

    public TypesHandler( Types types)
    {
        this.types = types;
    }

    @Override
    public void complexOpened( DesktopProtocol.Id id)
    {
        switch( id)
        {
            case Type ->
            {
                Type type = new Type();

                try
                {
                    type.getId().setValue( typeId + Model.TYPE_START);
                }
                catch( ValueOutOfRangeException ignored) {}

                pushHandler( new TypeHandler( types, type));

                // Always count up type id, it doesn't matter if a type was skipped.
                typeId++;
            }

            default -> super.complexOpened( id);
        }
    }
}
