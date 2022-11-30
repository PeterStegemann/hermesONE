package net.stegemann.configuration.source.input;

import lombok.Getter;
import net.stegemann.configuration.Signal;
import net.stegemann.configuration.source.Source;
import net.stegemann.configuration.type.Bool;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.ValueOutOfRangeException;
import net.stegemann.configuration.type.Volume;
import net.stegemann.configuration.util.ConfigurationField;
import net.stegemann.io.xml.Names;

@Getter
@ConfigurationField( name = Names.SOURCE_INPUT_ROTARY)
public final class Rotary extends Input
{
	@ConfigurationField( name = Names.SOURCE_INPUT_ROTARY_A_INPUT)
	private final Number aInputId;
	@ConfigurationField( name = Names.SOURCE_INPUT_ROTARY_B_INPUT)
	private final Number bInputId;
	@ConfigurationField( name = Names.SOURCE_INPUT_ROTARY_STORE)
	private final Bool store;
	@ConfigurationField( name = Names.SOURCE_INPUT_ROTARY_INIT)
	private final Volume init;
	@ConfigurationField( name = Names.SOURCE_INPUT_ROTARY_STEP)
	private final Volume step;
	@ConfigurationField( name = Names.SOURCE_INPUT_ROTARY_TOP)
	private final Volume top;
	@ConfigurationField( name = Names.SOURCE_INPUT_ROTARY_BOTTOM)
	private final Volume bottom;

	public Rotary()
	{
		aInputId = new Number( DIGITAL_INPUT_MINIMUM, DIGITAL_INPUT_MAXIMUM);
		bInputId = new Number( DIGITAL_INPUT_MINIMUM, DIGITAL_INPUT_MAXIMUM);
		store = new Bool();
		init = new Volume( Signal.MINIMUM_VALUE,  Signal.MAXIMUM_VALUE, INIT_SIGNAL_PER_VALUE);

		try
		{
			step = new Volume( Signal.MINIMUM_VALUE, Signal.MAXIMUM_VALUE, STEP_SIGNAL_PER_VALUE,
					           STEP_DEFAULT_VALUE);
			top = new Volume( Signal.MINIMUM_VALUE, Signal.MAXIMUM_VALUE, TOP_SIGNAL_PER_VALUE,
							  TOP_DEFAULT_VALUE);
			bottom = new Volume( Signal.MINIMUM_VALUE, Signal.MAXIMUM_VALUE, BOTTOM_SIGNAL_PER_VALUE,
								 BOTTOM_DEFAULT_VALUE);
		}
		catch( ValueOutOfRangeException reason)
		{
			throw new RuntimeException( reason);
		}
	}

	public Rotary( Rotary other)
	{
		super( other);

		aInputId = new Number( other.aInputId);
		bInputId = new Number( other.bInputId);
		store = new Bool( other.store);
		init = new Volume( other.init);
		step = new Volume( other.step);
		top = new Volume( other.top);
		bottom = new Volume( other.bottom);
	}

	@Override
	public String toString()
	{
		StringBuffer Buffer = new StringBuffer();

		Buffer.append( "Rotary = {\n");
		Buffer.append( super.toString());
		Buffer.append( " A Input Id: " + aInputId + "\n");
		Buffer.append( " B Input Id: " + bInputId + "\n");
		Buffer.append( " Store: " + store + "\n");
		Buffer.append( " Init: " + init + "\n");
		Buffer.append( " Step: " + step + "\n");
		Buffer.append( " Top: " + top + "\n");
		Buffer.append( " Bottom: " + bottom + "\n");
		Buffer.append( "}\n");

		return Buffer.toString();
	}

	@Override
	public Source clone()
	{
		return new Rotary( this);
	}
}
