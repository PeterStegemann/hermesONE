package net.stegemann.io.serial.configuration.read.handler.source.input;

import net.stegemann.configuration.source.input.Ticker;
import net.stegemann.io.serial.base.DesktopProtocol;
import net.stegemann.io.serial.configuration.read.handler.DesktopConnectionHandler;

public class TickerHandler extends DesktopConnectionHandler
{
	private final Ticker ticker;

	public TickerHandler( Ticker ticker)
	{
		this.ticker = ticker;
	}

	@Override
	public void valueRead( DesktopProtocol.Id id, String textContent)
	{
        switch( id)
        {
            case SourceInputTickerLowInput ->
            {
                readValue( ticker.getLowInputId(), textContent);
            }

            case SourceInputTickerHighInput ->
            {
                readValue( ticker.getHighInputId(), textContent);
            }

            case SourceInputTickerInit ->
            {
                readValue( ticker.getInit(), textContent);
            }

            case SourceInputTickerStep ->
            {
                readValue( ticker.getStep(), textContent);
            }

            case SourceInputTickerStore ->
            {
                readValue( ticker.getStore(), textContent);
            }

            case SourceInputTickerTop ->
            {
                readValue( ticker.getTop(), textContent);
            }

            case SourceInputTickerBottom ->
            {
                readValue( ticker.getBottom(), textContent);
            }

            default -> super.valueRead( id, textContent);
        }
	}
}
