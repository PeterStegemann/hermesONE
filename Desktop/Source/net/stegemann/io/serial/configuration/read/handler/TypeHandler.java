package net.stegemann.io.serial.configuration.read.handler;

import net.stegemann.configuration.Type;
import net.stegemann.configuration.Types;
import net.stegemann.io.Utility;
import net.stegemann.io.serial.base.DesktopProtocol;

class TypeHandler extends DesktopConnectionHandler
{
	private final Types types;
	private final Type type;

	public TypeHandler( Types types, Type type)
	{
		this.types = types;
		this.type = type;
	}

	@Override
	public void complexClosed( DesktopProtocol.Id id)
	{
		if( type.getState() == Type.State.USED)
		{
			types.addType( type);

			getConfigurationProgress().setTypeCount( types.getCount());
		}
	}

	@Override
	public void valueRead( DesktopProtocol.Id id, String textContent)
	{
		switch( id)
		{
			case TypeName :
			{
				readValue( type.getName(), textContent);
			}
			break;

			case TypeState :
			{
				type.setState( Utility.convertTypeState( textContent));
			}
			break;

			default : super.valueRead( id, textContent); break;
		}
	}
}
