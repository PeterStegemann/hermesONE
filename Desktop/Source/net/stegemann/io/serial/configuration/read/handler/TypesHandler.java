package net.stegemann.io.serial.configuration.read.handler;

import net.stegemann.configuration.Model;
import net.stegemann.configuration.Type;
import net.stegemann.configuration.Types;
import net.stegemann.configuration.type.ValueOutOfRangeException;
import net.stegemann.io.serial.base.DesktopProtocol;

class TypesHandler extends DesktopConnectionHandler
{
	private final Types types;

	private int typeId;

	public TypesHandler( Types types)
	{
		this.types = types;

		// TypeIds don't start with zero!
		typeId = Model.TYPE_START;
	}

	@Override
	public void complexOpened( DesktopProtocol.Id id)
	{
		switch( id)
		{
			case Type :
			{
				Type NewType = new Type();

				try
				{
					NewType.getId().setValue( typeId);
				}
				catch( ValueOutOfRangeException ignored)
				{
				}

				// Always count up type id, it doesn't matter if a type was skipped.
				typeId++;

				pushHandler( new TypeHandler( types, NewType));
			}
			break;

			default : super.complexOpened( id); break;
		}
	}
}
