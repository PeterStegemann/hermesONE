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

	private final List< Source> sources = new ArrayList< Source>();
	private PickGlobals pickGlobals = PickGlobals.No;
	private Number typeId = null;
	private Number modelId = null;
	private HasEmpty hasEmpty = HasEmpty.No;
	private HasFixed hasFixed = HasFixed.No;
	private HasProxies hasProxies = HasProxies.No;
	private Class< ?> type = null;

	/**
	 * This view presents a limited selection of sources based on the given criteria.
	 * 
	 * @param UseSources The sources to pick from.
	 * @param PickGlobals There's no id for globals, so this switch turns selection of globals on
	 * or off.
	 * @param TypeId Selects sources that are valid for this type id. If this is null, no sources
	 * will be selected based on the type id.
	 * @param ModelId Selects sources that are valid for this model id. If this is null, no sources
	 * will be selected based on the model id.
	 * @param UseHasEmpty Sources will contain the empty source element.
	 * @param UseHasFixed Sources will contain the fixed source element.
	 * @param UseHasProxies Sources will contain proxies.
	 */
	public SourcesView( Sources UseSources, PickGlobals PickGlobals, Number TypeId, Number ModelId,
						HasEmpty UseHasEmpty, HasFixed UseHasFixed, HasProxies UseHasProxies)
	{
		this( UseSources, PickGlobals, TypeId, ModelId, null, UseHasEmpty, UseHasFixed,
			  UseHasProxies);
	}

	/**
	 * This view presents a limited selection of sources based on the given criteria.
	 * 
	 * @param UseSources The sources to pick from.
	 * @param UsePickGlobals There's no id for globals, so this switch turns selection of globals on
	 * or off.
	 * @param TypeId Selects sources that are valid for this type id. If this is null, no sources
	 * will be selected based on the type id.
	 * @param ModelId Selects sources that are valid for this model id. If this is null, no sources
	 * will be selected based on the model id.
	 * @param Type Limits the above selections to sources of the given type.
	 * @param HasEmpty Sources will contain the empty source element.
	 * @param HasFixed Sources will contain the fixed source element.
	 * @param HasProxies Sources will contain proxies.
	 */
	public SourcesView( Sources UseSources, PickGlobals UsePickGlobals, Number TypeId,
						Number ModelId, Class<?> Type, HasEmpty HasEmpty, HasFixed HasFixed,
						HasProxies HasProxies)
	{
		baseSources = UseSources;
		baseSources.addChangeListener( this);

		this.pickGlobals = UsePickGlobals;
		this.typeId = TypeId;
		this.modelId = ModelId;
		this.type = Type;
		this.hasEmpty = HasEmpty;
		this.hasFixed = HasFixed;
		this.hasProxies = HasProxies;

		rescan();
	}

	@Override
	public String toString()
	{
		StringBuffer Buffer = new StringBuffer();

		Buffer.append( "SourcesView = {\n");

		for( Source Source: sources)
		{
			Buffer.append( Source);
		}

		Buffer.append( "}\n");

		return Buffer.toString();
	}

	@Override
	public void hasChanged( Sources Object)
	{
		rescan();

		notifyChange( this);
	}

	@Override
	public Iterator< Source> iterator()
	{
		return new SourcesViewIterator( this);
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
		Number ModelId = source.getModel();

		boolean IsMatching =
			((( pickGlobals == PickGlobals.Yes) && ( ModelId.equals( Model.MODEL_GLOBAL))) ||
			 (( typeId != null) && ( ModelId.equals( typeId))) ||
		     (( modelId != null) && ( ModelId.equals( modelId))));

		return IsMatching;
	}

	/**
	 * Build the list of sources for the given matching rules.
	 */
	public void rescan()
	{
		SortedSet< Source> sortedSources = new TreeSet< Source>();

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

	private class SourcesViewIterator implements Iterator< Source>
	{
		private Iterator< Source> iterator;

		public SourcesViewIterator( SourcesView UseSourcesView)
		{
			iterator = UseSourcesView.sources.iterator();
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
