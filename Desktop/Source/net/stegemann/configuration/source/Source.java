package net.stegemann.configuration.source;

import java.util.HashMap;

import net.stegemann.configuration.Named;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.SourceId;
import net.stegemann.configuration.type.Text;
import net.stegemann.misc.ChangeListener;
import net.stegemann.misc.ChangeObservable;

public abstract class Source extends ChangeObservable< Source>
						  		  implements ChangeListener< Text>, Comparable< Source>, Named
{
	public static final int SOURCE_FIXED = 0xfffe;
	public static final int SOURCE_NONE = 0xffff;

	public static final int SOURCE_START = 0;
	public static final int SOURCE_END = SOURCE_NONE;

	private final SourceId id = new SourceId();
	private final Text name;
	private Number model = null;

	protected Source()
	{
		name = new Text();

		name.addChangeListener( this);
	}

	protected Source( Source other)
	{
		name = new Text( other.name);
		model = other.model;

		name.addChangeListener( this);
	}

	@Override
	public String toString()
	{
		StringBuffer Buffer = new StringBuffer();

		Buffer.append( "Source = {\n");
		Buffer.append(  " Id: " + id + "\n");
		Buffer.append(  " Name: " + name + "\n");
		Buffer.append(  " Model Id: " + model + "\n");
		Buffer.append( "}\n");

		return Buffer.toString();
	}

	@Override
	public abstract Source clone();

	@Override
	public void hasChanged( Text object)
	{
		notifyChange( this);
	}

	public SourceId getId()
	{
		return id;
	}

	@Override
	public Text getName()
	{
		return name;
	}

	public void setModel( Number model)
	{
		this.model = model;
	}

	public Number getModel()
	{
		return model;
	}

	public abstract void replaceSources( HashMap< SourceId, SourceId> sourcesMap);

	@Override
	public int compareTo( Source Other)
	{
		if(( Other instanceof Empty) || ( Other instanceof Fixed))
		{
			return 1;
		}

		return name.getValue().compareToIgnoreCase( Other.name.getValue());
	}
}
