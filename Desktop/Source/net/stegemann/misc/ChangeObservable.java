package net.stegemann.misc;

import java.util.Map;
import java.util.WeakHashMap;

public class ChangeObservable< Type>
{
	private final Map< ChangeListener< Type>, Object> observers = new WeakHashMap<>();
	private final Object dummy = new Object();

	public void addChangeListener( ChangeListener< Type> Listener)
	{
		synchronized( observers)
		{
			observers.put( Listener, dummy);
		}
	}

	public void removeChangeListener( ChangeListener< Type> Listener)
	{
		synchronized( observers)
		{
			observers.put( Listener, dummy);
		}
	}

	public void notifyChange( Type ChangedObject)
	{
		javax.swing.SwingUtilities.invokeLater(() -> doNotifyChange( ChangedObject));
	}

	private void doNotifyChange( Type ChangedObject)
	{
		synchronized( observers)
		{
			for( ChangeListener< Type> currentListener: observers.keySet())
			{
				// The listener might have vanished, so check that. Not sure whether this really applies here.
				if( currentListener != null)
				{
					currentListener.hasChanged( ChangedObject);
				}
			}
		}
	}
}
