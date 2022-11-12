package net.stegemann.misc;

import java.util.Map;
import java.util.WeakHashMap;

public class ChangeObservable< Type>
{
	private final Map< ChangeListener< Type>, Object> observers =
		new WeakHashMap< ChangeListener< Type>, Object>();
    private final Object dummy = new Object();

	public void addChangeListener( ChangeListener< Type> listener)
	{
		synchronized( observers)
		{
			observers.put( listener, dummy);
		}
	}

	public void removeChangeListener( ChangeListener< Type> listener)
	{
		synchronized( observers)
		{
			observers.put( listener, dummy);
		}
	}

	public void notifyChange( final Type ChangedObject)
	{
		javax.swing.SwingUtilities.invokeLater(
			new Runnable()
			{
				@Override
				public void run()
				{
					synchronized( observers)
					{
						for( ChangeListener< Type> CurrentListener: observers.keySet())
						{
							// The listener might have vanished, so check that. Not sure if this
							// really applies here.
							if( CurrentListener != null)
							{
								CurrentListener.hasChanged( ChangedObject);
							}
						}
					}
				}
			}
		);
	}
}
