package net.stegemann.misc;

import java.util.HashMap;
import java.util.Map;
import java.util.function.Supplier;

public class ObjectFactory
{
	private static final Map<Supplier< ?>, Object> singletons = new HashMap<>();

	public static < Type> Type singleton( Supplier< Type> supplier)
	{
		if( singletons.containsKey( supplier))
		{
			Object object = singletons.get( supplier);

			// noinspection unchecked
			return ( Type) object;
		}
		else
		{
			Type generated = supplier.get();
			singletons.put(supplier, generated);

			return generated;
		}
    }
}
