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
@ConfigurationField( name = Names.SOURCE_INPUT_TICKER)
public final class Ticker extends Input
{
	@ConfigurationField( name = Names.SOURCE_INPUT_TICKER_LOW_INPUT)
	private final Number lowInputId;
	@ConfigurationField( name = Names.SOURCE_INPUT_TICKER_HIGH_INPUT)
	private final Number highInputId;
	@ConfigurationField( name = Names.SOURCE_INPUT_TICKER_INIT)
	private final Volume init;
	@ConfigurationField( name = Names.SOURCE_INPUT_TICKER_STEP)
	private final Volume step;
	@ConfigurationField( name = Names.SOURCE_INPUT_TICKER_STORE)
	private final Bool store;
	@ConfigurationField( name = Names.SOURCE_INPUT_TICKER_TOP)
	private final Volume top;
	@ConfigurationField( name = Names.SOURCE_INPUT_TICKER_BOTTOM)
	private final Volume bottom;

	public Ticker()
	{
		lowInputId = new Number( DIGITAL_INPUT_MINIMUM, DIGITAL_INPUT_MAXIMUM);
		highInputId = new Number( DIGITAL_INPUT_MINIMUM, DIGITAL_INPUT_MAXIMUM);
		init = new Volume( Signal.MINIMUM_VALUE, Signal.MAXIMUM_VALUE, INIT_SIGNAL_PER_VALUE);
		store = new Bool();

		try
		{
			step = new Volume( Signal.MINIMUM_VALUE, Signal.MAXIMUM_VALUE, STEP_SIGNAL_PER_VALUE, STEP_DEFAULT_VALUE);
			top = new Volume( Signal.MINIMUM_VALUE, Signal.MAXIMUM_VALUE, TOP_SIGNAL_PER_VALUE,	TOP_DEFAULT_VALUE);
			bottom = new Volume( Signal.MINIMUM_VALUE, Signal.MAXIMUM_VALUE, BOTTOM_SIGNAL_PER_VALUE,
								 BOTTOM_DEFAULT_VALUE);
		}
		catch( ValueOutOfRangeException reason)
		{
			throw new RuntimeException( reason);
		}
	}

	public Ticker( Ticker other)
	{
		super( other);

		lowInputId = new Number( other.lowInputId);
		highInputId = new Number( other.highInputId);
		init = new Volume( other.init);
		store = new Bool( other.store);
		step = new Volume( other.step);
		top = new Volume( other.top);
		bottom = new Volume( other.bottom);
	}

	@Override
	public String toString()
	{
		StringBuffer Buffer = new StringBuffer();

		Buffer.append( "Ticker = {\n");
		Buffer.append( super.toString());
		Buffer.append( " Low Input Id: " + lowInputId + "\n");
		Buffer.append( " High Input Id: " + highInputId + "\n");
		Buffer.append( " Init: " + init + "\n");
		Buffer.append( " Step: " + step + "\n");
		Buffer.append( " Store: " + store + "\n");
		Buffer.append( " Top: " + top + "\n");
		Buffer.append( " Bottom: " + bottom + "\n");
		Buffer.append( "}\n");

		return Buffer.toString();
	}

	@Override
	public Source clone()
	{
		return new Ticker( this);
	}
}
