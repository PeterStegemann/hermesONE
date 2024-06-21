package net.stegemann.configuration.source;

import lombok.Getter;
import net.stegemann.configuration.Signal;
import net.stegemann.configuration.type.SourceId;
import net.stegemann.configuration.type.SourceWithVolume;
import net.stegemann.configuration.type.Volume;
import net.stegemann.configuration.util.ConfigurationField;
import net.stegemann.io.xml.Names;

import java.util.HashMap;import static net.stegemann.misc.Utility.indent;

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
        target = new SourceWithVolume( TARGET_SIGNAL_PER_VALUE);
        step = new SourceWithVolume( STEP_SIGNAL_PER_VALUE);
        trigger = new SourceId();

        triggerLowLimit = new Volume( TRIGGER_SIGNAL_PER_VALUE);
        triggerHighLimit = new Volume( TRIGGER_SIGNAL_PER_VALUE);
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
    public Source duplicate()
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

    @Override
    public void switchSources( SourceId sourceIdOne, SourceId sourceIdTwo)
    {
        target.switchSource( sourceIdOne, sourceIdTwo);
        step.switchSource( sourceIdOne, sourceIdTwo);
        trigger.switchSource( sourceIdOne, sourceIdTwo);
    }

    @Override
    public String toString()
    {
        return String.format
        (
            """
            Follower
            {
                %s
                target: %s
                step: %s
                trigger: %s
                triggerLowLimit: %s
                triggerHighLimit: %s
            }
            """,
            indent( super.toString()), target, step, trigger, triggerLowLimit, triggerHighLimit
        );
    }
}
