package net.stegemann.configuration.source;

import lombok.Getter;
import net.stegemann.configuration.Model;
import net.stegemann.configuration.Named;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.SourceId;
import net.stegemann.configuration.type.Text;
import net.stegemann.configuration.type.ValueOutOfRangeException;
import net.stegemann.configuration.util.ConfigurationField;
import net.stegemann.io.xml.Names;
import net.stegemann.misc.ChangeListener;
import net.stegemann.misc.ChangeObservable;

import java.util.HashMap;

@Getter
@ConfigurationField( name = Names.SOURCE)
public abstract class Source extends ChangeObservable< Source>
						  implements ChangeListener< Text>, Comparable< Source>, Named
{
	public static final int SOURCE_FIXED = 0xfffe;
	public static final int SOURCE_NONE = 0xffff;

	public static final int SOURCE_START = 0;
	public static final int SOURCE_END = SOURCE_NONE;

	private final SourceId id = new SourceId();
	@ConfigurationField( name = Names.SOURCE_NAME)
	private final Text name;
	@ConfigurationField( name = Names.SOURCE_MODEL)
	private final Number model = new Number( Model.MODEL_START, Model.MODEL_GLOBAL);

	protected Source()
	{
		name = new Text();

		name.addChangeListener( this);
	}

	protected Source( Source other)
	{
		name = new Text( other.name);
		try
		{
			model.setValue( other.model);
		}
		catch( ValueOutOfRangeException reason)
		{
			throw new RuntimeException( reason);
		}

		name.addChangeListener( this);
	}

	@Override
	public String toString()
	{
		return String.format( """
				Source
				{
					Id: %s
					Name: %s
					Model Id: %s
				}
				""",
				id, name, model);
	}

	@Override
	public abstract Source clone();

	@Override
	public void hasChanged( Text object)
	{
		notifyChange( this);
	}

	public void setModel( Number newModel)
		throws ValueOutOfRangeException
	{
		model.setValue( newModel);
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
