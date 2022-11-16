package net.stegemann.misc;

import java.util.HashMap;
import java.util.Map;
import java.util.function.Supplier;

public class ObjectFactory
{
	private static final Map< String, Object> singletons = new HashMap<>();

	public static < Type> Type singleton( Supplier< Type> supplier)
	{
		return( singleton( null, supplier));
    }

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
