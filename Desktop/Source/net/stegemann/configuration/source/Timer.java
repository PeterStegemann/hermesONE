package net.stegemann.configuration.source;

import lombok.Getter;
import net.stegemann.configuration.Signal;
import net.stegemann.configuration.type.Bool;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.SourceId;
import net.stegemann.configuration.type.Volume;
import net.stegemann.configuration.util.ConfigurationField;
import net.stegemann.io.xml.Names;

import java.util.HashMap;import static net.stegemann.misc.Utility.indent;

@Getter
@ConfigurationField( name = Names.SOURCE_TIMER)
public final class Timer extends Source
{
	public static final int TIME_MAXIMUM_VALUE = ( 5 * 60 * 60);

	public static final int TRIGGER_SIGNAL_PER_VALUE = ( Signal.VALUE_RANGE / 200);

	@ConfigurationField( name = Names.SOURCE_TIMER_INIT_TIME)
	private final Number initTime;
	@ConfigurationField( name = Names.SOURCE_TIMER_CURRENT_TIME)
	private final Number currentTime;

	@ConfigurationField( name = Names.SOURCE_TIMER_STORE)
	private final Bool store;
	@ConfigurationField( name = Names.SOURCE_TIMER_REVERSE)
	private final Bool reverse;

	@ConfigurationField( name = Names.SOURCE_TIMER_TRIGGER)
	private final SourceId trigger;

	@ConfigurationField( name = Names.SOURCE_TIMER_TRIGGER_LOW_LIMIT)
	private final Volume triggerLowLimit;
	@ConfigurationField( name = Names.SOURCE_TIMER_TRIGGER_HIGH_LIMIT)
	private final Volume triggerHighLimit;

	@ConfigurationField( name = Names.SOURCE_TIMER_WARN_LOW_TIME)
	private final Number warnLowTime;
	@ConfigurationField( name = Names.SOURCE_TIMER_WARN_CRITICAL_TIME)
	private final Number warnCriticalTime;
	@ConfigurationField( name = Names.SOURCE_TIMER_WARN_PAUSE_TIME)
	private final Number warnPauseTime;

	public Timer()
	{
		initTime = new Number( 0, TIME_MAXIMUM_VALUE);
		currentTime = new Number( 0, TIME_MAXIMUM_VALUE);

		trigger = new SourceId();

		triggerLowLimit = new Volume( TRIGGER_SIGNAL_PER_VALUE);
		triggerHighLimit = new Volume( TRIGGER_SIGNAL_PER_VALUE);

		warnLowTime = new Number( 0, TIME_MAXIMUM_VALUE);
		warnCriticalTime = new Number( 0, TIME_MAXIMUM_VALUE);
		warnPauseTime = new Number( 0, TIME_MAXIMUM_VALUE);

		store = new Bool();
		reverse = new Bool();
	}

	public Timer( Timer other)
	{
		super( other);

		initTime = new Number( other.initTime);
		currentTime = new Number( other.currentTime);

		trigger = new SourceId( other.trigger);

		triggerLowLimit = new Volume( other.triggerLowLimit);
		triggerHighLimit = new Volume( other.triggerHighLimit);

		warnLowTime = new Number( other.warnLowTime);
		warnCriticalTime = new Number( other.warnCriticalTime);
		warnPauseTime = new Number( other.warnPauseTime);

		store = new Bool( other.store);
		reverse = new Bool( other.reverse);
	}

	@Override
	public Source duplicate()
	{
		return new Timer( this);
	}

	@Override
	public void replaceSources( HashMap< SourceId, SourceId> sourcesMap)
	{
		trigger.replaceSource( sourcesMap);
	}

    @Override
    public void switchSources( SourceId sourceIdOne, SourceId sourceIdTwo)
    {
		trigger.switchSource( sourceIdOne, sourceIdTwo);
    }

	@Override
	public String toString()
	{
        return String.format
        (
            """
            Timer
            {
                %s
                initTime: %s
                currentTime: %s
                trigger: %s
                triggerLowLimit: %s
                triggerHighLimit: %s
                warnLowTime: %s
                warnCriticalTime: %s
                warnPauseTime: %s
                store: %s
                reverse: %s
            }
            """,
            indent( super.toString()), initTime, currentTime, trigger, triggerLowLimit, triggerHighLimit, warnLowTime,
            warnCriticalTime, warnPauseTime, store, reverse
        );
	}
}
