package net.stegemann.configuration.source;

import java.util.HashMap;

import net.stegemann.configuration.Model;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.SourceId;
import net.stegemann.configuration.type.ValueOutOfRangeException;

public final class Proxy extends Source
{
	public static final int SLOT_MINIMUM = 0;
	public static final int SLOT_MAXIMUM = ( SLOT_MINIMUM + Model.PROXIES - 1);

	private final Number slot;

	public Proxy()
	{
		try
		{
			slot = new Number( SLOT_MINIMUM, SLOT_MAXIMUM, SLOT_MINIMUM);
		}
		catch( ValueOutOfRangeException reason)
		{
			throw new RuntimeException( reason);
		}
	}

	public Proxy( Proxy other)
	{
		super( other);

		slot = new Number( other.slot);
	}

	@Override
	public String toString()
	{
		StringBuffer Buffer = new StringBuffer();

		Buffer.append( "Proxy = {\n");
		Buffer.append( super.toString());
		Buffer.append( " Slot: " + slot + "\n");
		Buffer.append( "}\n");

		return Buffer.toString();
	}

	@Override
	public Source clone()
	{
		return new Proxy( this);
	}

	@Override
	public void replaceSources( HashMap< SourceId, SourceId> sourcesMap)
	{
	}

	public Number getSlot()
	{
		return slot;
	}
}
