package net.stegemann.io.serial.configuration.read.handler.source;

import net.stegemann.configuration.source.*;
import net.stegemann.configuration.source.input.*;
import net.stegemann.io.serial.base.DesktopProtocol;
import net.stegemann.io.serial.configuration.read.handler.DesktopConnectionHandler;
import net.stegemann.io.serial.configuration.read.handler.source.input.*;

class SourceHandler extends DesktopConnectionHandler
{
	private final Sources sources;
	private final int sourceId;

	private Source source = null;
	private String name = null;
	private String modelId = null;

	public SourceHandler( Sources sources, int sourceId)
	{
		this.sources = sources;
		this.sourceId = sourceId;
	}

	@Override
	public void complexOpened( DesktopProtocol.Id id)
	{
        switch( id)
        {
            case SourceInputAnalog ->
            {
                Analog analog = new Analog();
                source = analog;

                pushHandler( new AnalogHandler( analog));
            }

            case SourceInputButton ->
            {
                Button button = new Button();
                source = button;

                pushHandler( new ButtonHandler( button));
            }

            case SourceInputRotary ->
            {
                Rotary rotary = new Rotary();
                source = rotary;

                pushHandler(new RotaryHandler( rotary));
            }

            case SourceInputSwitch ->
            {
                Switch aSwitch = new Switch();
                source = aSwitch;

                pushHandler( new SwitchHandler( aSwitch));
            }

            case SourceInputTicker ->
            {
                Ticker ticker = new Ticker();
                source = ticker;

                pushHandler( new TickerHandler( ticker));
            }

            case SourceMap ->
            {
                Map map = new Map();
                source = map;

                pushHandler( new MapHandler( map));
            }

            case SourceMix ->
            {
                Mix mix = new Mix();
                source = mix;

                pushHandler( new MixHandler( mix));
            }

            case SourceStore ->
            {
                Store store = new Store();
                source = store;

                pushHandler( new StoreHandler( store));
            }

            case SourceFollower ->
            {
                Follower follower = new Follower();
                source = follower;

                pushHandler( new FollowerHandler( follower));
            }

            case SourceTimer ->
            {
                Timer timer = new Timer();
                source = timer;

                pushHandler( new TimerHandler( timer));
            }

            case SourceProxy ->
            {
                Proxy proxy = new Proxy();
                source = proxy;

                pushHandler( new ProxyHandler( proxy));
            }

            case SourceTrimmer ->
            {
                Trimmer trimmer = new Trimmer();
                source = trimmer;

                pushHandler( new TrimmerHandler( trimmer));
            }

            default -> super.complexOpened( id);
        }
	}

	@Override
	public void complexClosed( DesktopProtocol.Id id)
	{
        if( source == null)
        {
            return;
        }

        try
        {
            source.getId().setValue( sourceId);
            readValue( source.getName(), name);
            readValue( source.getModelId(), modelId);

            debug( id + "'" + source.getId().getValue() + "'/'" + source.getModelId().getValue() + "'");

            sources.addSource( source);

            getConfigurationProgress().setSourcesCount( sources.getCount());
        }
        catch( Exception ignored) {}
    }

	@Override
	public void valueRead( DesktopProtocol.Id id, String textContent)
	{
        // Read and store these values to put them onto the concrete source later.
        switch( id)
        {
            case SourceName -> name = textContent;
            case SourceModel -> modelId = textContent;

            default -> super.valueRead( id, textContent);
        }
	}
}
