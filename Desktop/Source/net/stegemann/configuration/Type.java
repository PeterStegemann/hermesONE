package net.stegemann.configuration;

import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.Text;
import net.stegemann.misc.ChangeListener;
import net.stegemann.misc.ChangeObservable;

public class Type extends ChangeObservable< Type> implements ChangeListener< Text>, Named
{
	public enum State
	{
		EMPTY,
		USED
	}

	private final Number id;
	private final Text name;
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
	public Text getName()
	{
		return name;
	}

	public void setState( State UseState)
	{
		this.state = UseState;		
	}

	public State getState()
	{
		return state;
	}

	/**
	 * Get unique type id for reference by models and sources. 
	 *
	 * @return Type id.
	 */
	public Number getId()
	{
		return id;
	}
}
