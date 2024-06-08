package net.stegemann.io.serial.configuration.read.handler.source;

import net.stegemann.configuration.source.Timer;
import net.stegemann.io.serial.base.DesktopProtocol;
import net.stegemann.io.serial.configuration.read.handler.DesktopConnectionHandler;

class TimerHandler extends DesktopConnectionHandler
{
	private final Timer timer;

	public TimerHandler( Timer timer)
	{
		this.timer = timer;
	}

	@Override
	public void valueRead( DesktopProtocol.Id id, String textContent)
	{
		switch( id)
		{
			case SourceTimerInitTime -> readValue( timer.getInitTime(), textContent);
			case SourceTimerCurrentTime -> readValue( timer.getCurrentTime(), textContent);
			case SourceTimerStore -> readValue( timer.getStore(), textContent);
			case SourceTimerReverse -> readValue( timer.getReverse(), textContent);
			case SourceTimerTrigger -> readValue( timer.getTrigger(), textContent);
			case SourceTimerTriggerHighLimit -> readValue( timer.getTriggerHighLimit(), textContent);
			case SourceTimerTriggerLowLimit -> readValue( timer.getTriggerLowLimit(), textContent);
			case SourceTimerWarnLowTime -> readValue( timer.getWarnLowTime(), textContent);
			case SourceTimerWarnCriticalTime -> readValue( timer.getWarnCriticalTime(), textContent);
			case SourceTimerWarnPauseTime -> readValue( timer.getWarnPauseTime(), textContent);

			default -> super.valueRead( id, textContent);
		}
	}
}
