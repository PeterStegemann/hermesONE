package net.stegemann.configuration.view;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.SortedSet;
import java.util.TreeSet;

import net.stegemann.configuration.Model;
import net.stegemann.configuration.source.Empty;
import net.stegemann.configuration.source.Fixed;
import net.stegemann.configuration.source.Proxy;
import net.stegemann.configuration.source.Source;
import net.stegemann.configuration.source.Sources;
import net.stegemann.configuration.type.Number;
import net.stegemann.misc.ChangeListener;
import net.stegemann.misc.ChangeObservable;

public class SourcesView extends ChangeObservable< SourcesView>
					  implements Iterable< Source>, ChangeListener< Sources>
{
	public enum HasEmpty
	{
		Yes,
		No
	}

	public enum HasFixed
	{
		Yes,
		No
	}

	public enum HasProxies
	{
		Yes,
		No
	}

	public enum PickGlobals
	{
		Yes,
		No
	}

	private final Sources baseSources;

	private final List< Source> sources = new ArrayList<>();
	private final PickGlobals pickGlobals;
	private final Number typeId;
	private final Number modelId;
	private final HasEmpty hasEmpty;
	private final HasFixed hasFixed;
	private final HasProxies hasProxies;
	private final Class< ?> type;

	/**
	 * This view presents a limited selection of sources based on the given criteria.
	 * 
	 * @param sources The sources to pick from.
	 * @param pickGlobals There's no id for globals, so this switch turns selection of globals on
	 * or off.
	 * @param typeId Selects sources that are valid for this type id. If this is null, no sources
	 * will be selected based on the type id.
	 * @param modelId Selects sources that are valid for this model id. If this is null, no sources
	 * will be selected based on the model id.
	 * @param hasEmpty Sources will contain the empty source element.
	 * @param hasFixed Sources will contain the fixed source element.
	 * @param hasProxies Sources will contain proxies.
	 */
	public SourcesView( Sources sources, PickGlobals pickGlobals, Number typeId, Number modelId,
						HasEmpty hasEmpty, HasFixed hasFixed, HasProxies hasProxies)
	{
		this( sources, pickGlobals, typeId, modelId, null, hasEmpty, hasFixed, hasProxies);
	}

	/**
	 * This view presents a limited selection of sources based on the given criteria.
	 * 
	 * @param sources The sources to pick from.
	 * @param pickGlobals There's no id for globals, so this switch turns selection of globals on
	 * or off.
	 * @param typeId Selects sources that are valid for this type id. If this is null, no sources
	 * will be selected based on the type id.
	 * @param modelId Selects sources that are valid for this model id. If this is null, no sources
	 * will be selected based on the model id.
	 * @param type Limits the above selections to sources of the given type.
	 * @param hasEmpty Sources will contain the empty source element.
	 * @param hasFixed Sources will contain the fixed source element.
	 * @param hasProxies Sources will contain proxies.
	 */
	public SourcesView
	(
		Sources sources,
		PickGlobals pickGlobals,
		Number typeId,
		Number modelId,
		Class< ?> type,
		HasEmpty hasEmpty,
		HasFixed hasFixed,
		HasProxies hasProxies
	)
	{
		baseSources = sources;
		baseSources.addChangeListener( this);

		this.pickGlobals = pickGlobals;
		this.typeId = typeId;
		this.modelId = modelId;
		this.type = type;
		this.hasEmpty = hasEmpty;
		this.hasFixed = hasFixed;
		this.hasProxies = hasProxies;

		rescan();
	}

	@Override
	public String toString()
	{
		StringBuilder builder = new StringBuilder();

		builder.append( "SourcesView = {\n");

		for( Source Source: sources)
		{
			builder.append( Source);
		}

		builder.append( "}\n");

		return builder.toString();
	}

	@Override
	public void hasChanged( Sources sources)
	{
		rescan();

		notifyChange( this);
	}

	@Override
	public Iterator< Source> iterator()
	{
		return new SourcesViewIterator( this );
	}

	private boolean isMatchingSource( Source source)
	{
		// If a type is set, it has to match.
		if( type != null)
		{
			if( type != source.getClass())
			{
				return false;
			}
		}
		else
		{
			// If no specific type is set, filter proxies.
			if(( hasProxies == HasProxies.No) && ( source.getClass() == Proxy.class))
			{
				return false;
			}
		}

		// Match if the source is global, same type or same model.
		Number sourceModelId = source.getModel();

		boolean isMatching =
			((( pickGlobals == PickGlobals.Yes) && ( sourceModelId.equals( Model.MODEL_GLOBAL))) ||
			 (( typeId != null) && ( sourceModelId.equals( typeId))) ||
		     ((this.modelId != null) && ( sourceModelId.equals( this.modelId))));

		return isMatching;
	}

	/**
	 * Build the list of sources for the given matching rules.
	 */
	public void rescan()
	{
		SortedSet< Source> sortedSources = new TreeSet<>();

		if( hasEmpty == HasEmpty.Yes)
		{
			sortedSources.add( new Empty());
		}

		if( hasFixed == HasFixed.Yes)
		{
			sortedSources.add( new Fixed());
		}

		// Copy references to all matching sources.
		for( Source CurrentSource: baseSources)
		{
			// Match if the source is global, same type or same model.
			if( isMatchingSource( CurrentSource) == true)
			{
				sortedSources.add( CurrentSource);
			}
		}

		sources.clear();
		sources.addAll( sortedSources);
		
		notifyChange( this);
	}

	public Source getSourceFromIndex( int Index)
	{
		if( Index == -1)
		{
			return null;
		}

		try
		{
			return sources.get( Index);
		}
		catch( IndexOutOfBoundsException reason)
		{
			return null;
		}
	}

	public int getFullSourceIndex( int Index)
	{
		if( Index == -1)
		{
			return -1;
		}

		try
		{
			Source Source = sources.get( Index);

			return baseSources.getIndexFromSource( Source);
		}
		catch( IndexOutOfBoundsException reason)
		{
			return -1;
		}
	}

	public Number getSourceIdFromIndex( int Index)
	{
		Source CurrentSource = getSourceFromIndex( Index);

		if( CurrentSource == null)
		{
			return null;
		}

		return CurrentSource.getId();
	}

	public int getSourceIndexFromId( Number Id)
	{
		int Index = 0;

		for( Source CurrentSource: sources)
		{
			if( CurrentSource.getId().equals( Id))
			{
				return( Index);
			}

			Index++;
		}

		return -1;
	}

	public int getSourceCount()
	{
		return sources.size();
	}

	private static class SourcesViewIterator implements Iterator< Source>
	{
		private final Iterator< Source> iterator;

		public SourcesViewIterator( SourcesView sourcesView)
		{
			iterator = sourcesView.sources.iterator();
		}

		@Override
		public boolean hasNext()
		{
			return iterator.hasNext();
		}

		@Override
		public Source next()
		{
			return iterator.next();
		}

		@Override
		public void remove()
		{
			iterator.remove();
		}
	}
}
