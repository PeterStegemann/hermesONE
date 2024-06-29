package net.stegemann.configuration;

import lombok.Getter;
import lombok.Setter;
import net.stegemann.configuration.source.Map;import net.stegemann.configuration.source.Proxy;import net.stegemann.configuration.source.Source;import net.stegemann.configuration.type.ModelId;import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.Text;
import net.stegemann.configuration.util.ConfigurationField;
import net.stegemann.configuration.util.Validatable;import net.stegemann.configuration.view.SourcesView;import net.stegemann.io.xml.Names;
import net.stegemann.misc.ChangeListener;
import net.stegemann.misc.ChangeObservable;import java.util.HashMap;

@Getter
public class Type extends ChangeObservable< Type>
               implements ChangeListener< Text>, Named, Validatable
{
	public enum State
	{
		EMPTY,
		USED
	}

	@ConfigurationField( name = Names.TYPE_ID)
	private final ModelId id;
	@ConfigurationField( name = Names.TYPE_NAME)
	private final Text name;
	@Setter
	@ConfigurationField( name = Names.TYPE_STATE)
	private State state;

	public Type()
	{
		id = new ModelId( Model.TYPE_START, Model.TYPE_END);
		name = new Text();
		state = State.USED;

		name.addChangeListener( this);
	}

	public Type( Type other)
	{
		id = new ModelId( other.id);
		name = new Text( other.name);
		state = other.state;

		name.addChangeListener( this);
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

	public boolean validate( Configuration configuration)
	{
        boolean result = true;

        SourcesView sourcesView =
            new SourcesView
            (
                configuration.getSources(),
                SourcesView.PickGlobals.No,
                null,
                id,
                Proxy.class,
                SourcesView.HasEmpty.No,
                SourcesView.HasFixed.No,
                SourcesView.HasProxies.Yes
            );

        java.util.Map< Number, Proxy> duplicateMap = new HashMap<>();

        for( Source source: sourcesView)
        {
            Proxy proxy = ( Proxy) source;
            Number slot = proxy.getSlot();

            if( duplicateMap.containsKey( slot))
            {
                Proxy duplicateProxy = duplicateMap.get( slot);

                validationError
                (
                    configuration,
                    id,
                    "duplicate proxy slot " + slot.getValue() + " in %s (%s) and %s (%s)",
                    proxy.getId().getValue(),
                    proxy.getName().getValue(),
                    duplicateProxy.getId().getValue(),
                    duplicateProxy.getName().getValue()
                );

                result = false;
            }
            else
            {
                duplicateMap.put( slot, proxy);
            }
        }

        return result;
	}

    @Override
    public String validationName( Configuration configuration, ModelId modelId)
    {
        return "type %s (%s)".formatted( id.getValue(), name.getValue());
    }

	@Override
	public String toString()
	{
        return String.format( """
            Type
            {
                id: %s
                name: %s
                state: %s
            }
            """,
            id, name, state
		);
	}
}
