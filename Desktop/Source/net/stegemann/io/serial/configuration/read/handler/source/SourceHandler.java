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

	private Source newSource = null;
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
                Analog NewSource = new Analog();
                newSource = NewSource;

                pushHandler( new InputAnalogHandler( NewSource));
            }

            case SourceInputButton ->
            {
                Button NewSource = new Button();
                newSource = NewSource;

                pushHandler( new InputButtonHandler( NewSource));
            }

            case SourceInputRotary ->
            {
                Rotary NewSource = new Rotary();
                newSource = NewSource;

                pushHandler(new InputRotaryHandler( NewSource));
            }

            case SourceInputSwitch ->
            {
                Switch NewSource = new Switch();
                newSource = NewSource;

                pushHandler( new InputSwitchHandler( NewSource));
            }

            case SourceInputTicker ->
            {
                Ticker NewSource = new Ticker();
                newSource = NewSource;

                pushHandler( new InputTickerHandler( NewSource));
            }

            case SourceMap ->
            {
                Map NewSource = new Map();
                newSource = NewSource;

                pushHandler( new MapHandler( NewSource));
            }
            case SourceMix -> {
                Mix NewSource = new Mix();
                newSource = NewSource;

                pushHandler( new MixHandler( NewSource));
            }

            case SourceStore ->
            {
                Store NewSource = new Store();
                newSource = NewSource;

                pushHandler( new StoreHandler( NewSource));
            }

            case SourceFollower ->
            {
                Follower NewSource = new Follower();
                newSource = NewSource;

                pushHandler( new FollowerHandler( NewSource));
            }

            case SourceTimer ->
            {
                Timer NewSource = new Timer();
                newSource = NewSource;

                pushHandler( new TimerHandler( NewSource));
            }

            case SourceProxy ->
            {
                Proxy NewSource = new Proxy();
                newSource = NewSource;

                pushHandler( new ProxyHandler( NewSource));
            }

            case SourceTrimmer ->
            {
                Trimmer NewSource = new Trimmer();
                newSource = NewSource;

                pushHandler( new TrimmerHandler( NewSource));
            }

            default -> super.complexOpened( id);
        }
	}

	@Override
	public void complexClosed( DesktopProtocol.Id id)
	{
        if(( newSource == null)/*|| ( NewSource.GetType() == Source.Type.EMPTY)*/)
        {
            return;
        }

        try
        {
            newSource.getId().setValue( sourceId);
            readValue( newSource.getName(), name);
            readValue( newSource.getModel(), modelId);

            debug( id + "'" + newSource.getId().getValue() + "'/'" + newSource.getModel().getValue() + "'");

            sources.addSource( newSource);

            getConfigurationProgress().setSourceCount( sources.getCount());
        }
        catch( Exception ignored) {}
    }

	@Override
	public void valueRead( DesktopProtocol.Id id, String textContent)
	{
        // Read and store these values to put them onto the concrete source later.
        switch( id)
        {
            case SourceName ->
            {
                name = textContent;
            }

            case SourceModel ->
            {
                modelId = textContent;
            }

            default -> super.valueRead( id, textContent);
        }
	}
}
