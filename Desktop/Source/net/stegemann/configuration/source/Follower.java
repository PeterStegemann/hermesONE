package net.stegemann.configuration.source;

import lombok.Getter;
import net.stegemann.configuration.Signal;
import net.stegemann.configuration.type.SourceId;
import net.stegemann.configuration.type.SourceWithVolume;
import net.stegemann.configuration.type.Volume;
import net.stegemann.configuration.util.ConfigurationField;
import net.stegemann.io.xml.Names;

import java.util.HashMap;

@Getter
@ConfigurationField( name = Names.SOURCE_FOLLOWER)
public final class Follower extends Source
{
	public static final int TARGET_SIGNAL_PER_VALUE = ( Signal.VALUE_RANGE / 600);
	public static final int STEP_SIGNAL_PER_VALUE = ( Signal.VALUE_RANGE / 200);
	public static final int TRIGGER_SIGNAL_PER_VALUE = ( Signal.VALUE_RANGE / 200);

	@ConfigurationField( name = Names.SOURCE_FOLLOWER_TARGET)
	private final SourceWithVolume target;
	@ConfigurationField( name = Names.SOURCE_FOLLOWER_STEP)
	private final SourceWithVolume step;
	@ConfigurationField( name = Names.SOURCE_FOLLOWER_TRIGGER)
	private final SourceId trigger;

	@ConfigurationField( name = Names.SOURCE_FOLLOWER_TRIGGER_LOW_LIMIT)
	private final Volume triggerLowLimit;
	@ConfigurationField( name = Names.SOURCE_FOLLOWER_TRIGGER_HIGH_LIMIT)
	private final Volume triggerHighLimit;

	public Follower()
	{
		target = new SourceWithVolume( Signal.MINIMUM_VALUE, Signal.MAXIMUM_VALUE, TARGET_SIGNAL_PER_VALUE);
		step = new SourceWithVolume( Signal.MINIMUM_VALUE, Signal.MAXIMUM_VALUE, STEP_SIGNAL_PER_VALUE);
		trigger = new SourceId();

		triggerLowLimit = new Volume( Signal.MINIMUM_VALUE, Signal.MAXIMUM_VALUE, TRIGGER_SIGNAL_PER_VALUE);
		triggerHighLimit = new Volume( Signal.MINIMUM_VALUE, Signal.MAXIMUM_VALUE, TRIGGER_SIGNAL_PER_VALUE);
	}

	public Follower( Follower other)
	{
		super( other);

		target = new SourceWithVolume( other.target);
		step = new SourceWithVolume( other.step);
		trigger = new SourceId( other.trigger);

		triggerLowLimit = new Volume( other.triggerLowLimit);
		triggerHighLimit = new Volume( other.triggerHighLimit);
	}

	@Override
	public String toString()
	{
		StringBuffer Buffer = new StringBuffer();

		Buffer.append( "Follower = {\n");
		Buffer.append( super.toString());
		Buffer.append( " Target: " + target + "\n");
		Buffer.append( " Step: " + step + "\n");
		Buffer.append( " Trigger Id: " + trigger + "\n");
		Buffer.append( " Trigger Low Limit: " + triggerLowLimit + "\n");
		Buffer.append( " Trigger High Limit: " + triggerHighLimit + "\n");
		Buffer.append( "}\n");

		return Buffer.toString();
	}

	@Override
	public Source clone()
	{
		return new Follower( this);
	}

	@Override
	public void replaceSources( HashMap< SourceId, SourceId> sourcesMap)
	{
		target.replaceSource( sourcesMap);
		step.replaceSource( sourcesMap);
		trigger.replaceSource( sourcesMap);
	}
}