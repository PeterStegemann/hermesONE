package net.stegemann.configuration;

import net.stegemann.configuration.type.SourceId;
import net.stegemann.configuration.type.SourceWithVolume;
import net.stegemann.configuration.type.ValueOutOfRangeException;
import net.stegemann.misc.ChangeListener;
import net.stegemann.misc.ChangeObservable;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;

public class ProxyReferences extends ChangeObservable< ProxyReferences>
						  implements Iterable< SourceWithVolume>, ChangeListener< SourceWithVolume>
{
	public static final int PROXY_REFERENCE_SIGNAL_PER_VALUE = ( Signal.VALUE_RANGE / 200);

	private static final int DEFAULT_PROXY_REFERENCE_VOLUME = 100;

	private final List< SourceWithVolume> proxyReferences = new ArrayList<>();

	public ProxyReferences( int Proxies)
	{
		fill( Proxies);
	}

	public ProxyReferences( final ProxyReferences other)
	{
		for( SourceWithVolume OtherProxyReference: other.proxyReferences)
		{
			proxyReferences.add( new SourceWithVolume( OtherProxyReference));
		}
	}

	@Override
	public String toString()
	{
		StringBuffer Buffer = new StringBuffer();

		Buffer.append( "ProxyReferencess = {\n");

		for( SourceWithVolume ProxyReference: proxyReferences)
		{
			Buffer.append( ProxyReference);
		}

		Buffer.append( "}\n");

		return Buffer.toString();
	}

	@Override
	public void hasChanged( SourceWithVolume object)
	{
		notifyChange( this);
	}

	@Override
	public Iterator< SourceWithVolume> iterator()
	{
		return new ProxyReferenceIterator(this);
	}

	public void fill( int size)
	{
		proxyReferences.clear();

		// Fill up missing proxy slots.
		int fill = size;

		while( fill > 0)
		{
			try
			{
				proxyReferences.add( new SourceWithVolume( PROXY_REFERENCE_SIGNAL_PER_VALUE,
														   DEFAULT_PROXY_REFERENCE_VOLUME));
			}
			catch( ValueOutOfRangeException reason)
			{
				throw new RuntimeException( reason);
			}

			fill--;
		}
	}

	public SourceWithVolume getProxyReferenceFromIndex(int Index)
	{
		if( Index == -1)
		{
			return( null);
		}

		try
		{
			return proxyReferences.get( Index);
		}
		catch( IndexOutOfBoundsException reason)
		{
			return null;
		}
	}

	public void replaceSources( HashMap< SourceId, SourceId> SourcesMap)
	{
		for( SourceWithVolume OldProxyReference: proxyReferences)
		{
			OldProxyReference.replaceSource( SourcesMap);
		}
	}

	private static class ProxyReferenceIterator implements Iterator< SourceWithVolume>
	{
		private final Iterator< SourceWithVolume> iterator;

		public ProxyReferenceIterator( ProxyReferences UseProxies)
		{
			iterator = UseProxies.proxyReferences.iterator();
		}

		@Override
		public boolean hasNext()
		{
			return iterator.hasNext();
		}

		@Override
		public SourceWithVolume next()
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
