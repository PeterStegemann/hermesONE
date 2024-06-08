package net.stegemann.misc;

import java.util.HashMap;
import java.util.Map;
import java.util.function.Supplier;

/**
 *  Factory and storage for objects of all kinds. This allows to create and keep singleton instances of objects.
 */
public class ObjectFactory
{
	private static final Map< String, Object> singletons = new HashMap<>();

	/**
	 * Create or retrieve a singleton.
	 *
	 * @param supplier The supplier to identify or create the singleton if needed.
	 * @return The singleton
	 * @param <Type> The type of the singleton to be retrieved or created.
	 */
	public static < Type> Type singleton( Supplier< Type> supplier)
	{
		return( singleton( null, supplier));
    }

	/**
	 * Create or retrieve a singleton.
	 *
	 * @param parameter Parameter that helps identification of the singleton. Different parameters will create different
	 *                  singletons.
	 * @param supplier The supplier to identify or create the singleton if needed.
	 * @return The singleton
	 * @param <Type> The type of the singleton to be retrieved or created.
	 */
	public static < Type> Type singleton( Object parameter, Supplier< Type> supplier)
	{
		String key = System.identityHashCode( parameter) + "_" + System.identityHashCode( supplier);

		if( singletons.containsKey( key))
		{
			Object object = singletons.get( key);

			// noinspection unchecked
			return ( Type) object;
		}
		else
		{
			Type generated = supplier.get();
			singletons.put( key, generated);

			return( generated);
		}
	}
}
