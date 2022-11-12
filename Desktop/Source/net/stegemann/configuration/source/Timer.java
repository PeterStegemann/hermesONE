package net.stegemann.configuration.source;

import java.util.HashMap;

import net.stegemann.configuration.Signal;
import net.stegemann.configuration.type.Bool;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.SourceId;
import net.stegemann.configuration.type.Volume;

public final class Timer extends Source
{
	public static final int TIME_MAXIMUM_VALUE = ( 5 * 60 * 60);

	public static final int TRIGGER_SIGNAL_PER_VALUE = ( Signal.VALUE_RANGE / 200);

	private final Number initTime;
	private final Number currentTime;

	private final SourceId trigger;

	private final Volume triggerLowLimit;
	private final Volume triggerHighLimit;

	private final Number warnLowTime;
	private final Number warnCriticalTime;
	private final Number warnPauseTime;

	private final Bool store;
	private final Bool reverse;

	public Timer()
	{
		initTime = new Number( 0, TIME_MAXIMUM_VALUE);
		currentTime = new Number( 0, TIME_MAXIMUM_VALUE);

		trigger = new SourceId();

		triggerLowLimit = new Volume( Signal.MINIMUM_VALUE, Signal.MAXIMUM_VALUE,
									  TRIGGER_SIGNAL_PER_VALUE);
		triggerHighLimit = new Volume( Signal.MINIMUM_VALUE, Signal.MAXIMUM_VALUE,
									   TRIGGER_SIGNAL_PER_VALUE);

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
	public String toString()
	{
		StringBuffer Buffer = new StringBuffer();

		Buffer.append( "Timer = {\n");
		Buffer.append( super.toString());
		Buffer.append( " Init Time: " + initTime + "\n");
		Buffer.append( " Current Time: " + currentTime + "\n");
		Buffer.append( " Trigger Id: " + trigger + "\n");
		Buffer.append( " Trigger Low Limit: " + triggerLowLimit + "\n");
		Buffer.append( " Trigger High Limit: " + triggerHighLimit + "\n");
		Buffer.append( " Warn Low Time: " + warnLowTime + "\n");
		Buffer.append( " Warn Critical Time: " + warnCriticalTime + "\n");
		Buffer.append( " Warn Pause Time: " + warnPauseTime + "\n");
		Buffer.append( " Store: " + store + "\n");
		Buffer.append( " Reverse: " + reverse + "\n");
		Buffer.append( "}\n");

		return Buffer.toString();
	}

	@Override
	public Source clone()
	{
		return new Timer( this);
	}

	@Override
	public void replaceSources( HashMap< SourceId, SourceId> sourcesMap)
	{
		trigger.replaceSource( sourcesMap);
	}

	public Number getInitTime()
	{
		return initTime;
	}

	public Number getCurrentTime()
	{
		return currentTime;
	}

	public Number getTrigger()
	{
		return trigger;
	}

	public Volume getTriggerLowLimit()
	{
		return triggerLowLimit;
	}

	public Volume getTriggerHighLimit()
	{
		return triggerHighLimit;
	}

	public Number getWarnLowTime()
	{
		return warnLowTime;
	}

	public Number getWarnCriticalTime()
	{
		return warnCriticalTime;
	}

	public Number getWarnPauseTime()
	{
		return warnPauseTime;
	}

	public Bool getStore()
	{
		return store;
	}

	public Bool getReverse()
	{
		return reverse;
	}
}
