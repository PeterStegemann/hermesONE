package net.stegemann.configuration;

import lombok.Getter;
import lombok.Setter;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.Text;
import net.stegemann.configuration.util.ConfigurationField;
import net.stegemann.io.xml.Names;
import net.stegemann.misc.ChangeListener;
import net.stegemann.misc.ChangeObservable;

@Getter
public class Type extends ChangeObservable< Type> implements ChangeListener< Text>, Named
{
	public enum State
	{
		EMPTY,
		USED
	}

	@ConfigurationField( name = Names.TYPE_ID)
	private final Number id;
	@ConfigurationField( name = Names.TYPE_NAME)
	private final Text name;
	@Setter
	@ConfigurationField( name = Names.TYPE_STATE)
	private State state;

	public Type()
	{
		id = new Number( Model.TYPE_START, Model.TYPE_END);
		name = new Text();
		state = State.USED;

		name.addChangeListener( this);
	}

	public Type( Type other)
	{
		id = new Number( other.id);
		name = new Text( other.name);
		state = other.state;

		name.addChangeListener( this);
	}

	@Override
	public String toString()
	{
		StringBuffer Buffer = new StringBuffer();

		Buffer.append( "Type = {\n");
		Buffer.append(  " Id: " + id + "\n");
		Buffer.append(  " Name: " + name + "\n");
		Buffer.append(  " State: " + state + "\n");
		Buffer.append( "}\n");

		return Buffer.toString();
	}

	@SuppressWarnings("MethodDoesntCallSuperMethod")
	@Override
	public Type clone()
	{
		return new Type( this);
	}

	@Override
	public void hasChanged( Text object)
	{
		notifyChange( this);
	}

	@Override
	public Number getModelId()
	{
		return id;
	}
}
