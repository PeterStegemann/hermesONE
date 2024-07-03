package net.stegemann.io.serial.configuration.write;

import net.stegemann.configuration.*;
import net.stegemann.configuration.Model.StatusSource;
import net.stegemann.configuration.System;
import net.stegemann.configuration.Model.StatusTime;
import net.stegemann.configuration.source.*;
import net.stegemann.configuration.source.input.*;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.*;
import net.stegemann.io.Utility;
import net.stegemann.io.WriteException;
import net.stegemann.io.serial.base.DesktopConnection;
import net.stegemann.io.serial.base.DesktopProtocol;
import net.stegemann.io.serial.base.DesktopProtocol.Id;
import net.stegemann.io.serial.configuration.ConfigurationProgress;
import net.stegemann.misc.ChangeListener;
import net.stegemann.misc.ThrowingFunction;

import static net.stegemann.misc.Utility.forEach;

public class SerialConfigurationWriter
{
    private final ConfigurationProgress configurationProgress;

    private final DesktopConnection connection;

    public SerialConfigurationWriter( ConfigurationProgress configurationProgress, DesktopConnection connection)
    {
        this.configurationProgress = configurationProgress;
        this.connection = connection;
    }

    public void writeToPort
    (
        Configuration configuration, String portName, ChangeListener< ConfigurationProgress> configurationListener
    )
        throws WriteException
    {
        DummyConnectionHandler connectionForwarder = new DummyConnectionHandler();

        if( connection.open( portName, connectionForwarder) == false)
        {
            throw new WriteException( "Failed to open connection with port " + portName + ".");
        }

        configurationProgress.reset();
        configurationProgress.setTypesMaximum( configuration.getTypes().getCount());
        configurationProgress.setModelsMaximum( configuration.getModels().getCount());
        configurationProgress.setSourcesMaximum( configuration.getSources().getCount());

        configurationProgress.addChangeListener( configurationListener);

        try
        {
            connection.writeConfiguration();

            configuration( configuration);
        }
        catch( WriteException reason)
        {
            throw new WriteException( "Failed to write configuration!", reason);
        }
        finally
        {
            configurationProgress.removeChangeListener( configurationListener);

            connection.close();
        }
    }

    private void configuration( Configuration configuration)
        throws WriteException
    {
        complex( Id.Configuration, () ->
        {
            System system = configuration.getSystem();

            system( system);

            types( configuration.getTypes(), system.getStorageTypes().getValue());
            models( configuration.getModels(), system.getStorageModels().getValue());
            sources( configuration.getSources(), system.getStorageSources().getValue());
        });
    }

    private void system( System system)
        throws WriteException
    {
        value( Id.AnalogInputs, system.getAnalogInputs());
        value( Id.DigitalInputs, system.getDigitalInputs());
        value( Id.OutputChannels, system.getOutputChannels());
        value( Id.Outputs, system.getOutputs());

        value( Id.Owner, system.getOwner());
        value( Id.SetupBacklight, system.getSetupBacklight());
        value( Id.SetupBlankTime, system.getSetupBlankTime());
        value( Id.StatusBacklight, system.getStatusBacklight());
        value( Id.StatusContrast, system.getStatusContrast());
        value( Id.StatusBlankTime, system.getStatusBlankTime());
        value( Id.StatusInverted, system.getStatusInverted());
        value( Id.SelectedModel, system.getSelectedModel());

        battery( system.getBattery());
        calibrations( system.getCalibrations());
        ppms( system.getPpms());
    }

    private void battery( Battery battery)
        throws WriteException
    {
        complex( Id.Battery, () ->
        {
            value( Id.BatteryWarnLowVoltage, battery.getWarnLowVoltage());
            value( Id.BatteryWarnCriticalVoltage, battery.getWarnCriticalVoltage());
            value( Id.BatteryMinimumVoltage, battery.getMinimumVoltage());
            value( Id.BatteryMaximumVoltage, battery.getMaximumVoltage());
            value( Id.BatteryCalibrationValue, battery.getCalibrationVoltage());
        });
    }

    private void calibrations( Calibrations calibrations)
        throws WriteException
    {
        complex( Id.Calibrations, () -> forEach( calibrations, this::calibration));
    }

    private void calibration( Calibration calibration)
        throws WriteException
    {
        complex( Id.Calibration, () ->
        {
            value( Id.CalibrationHigh, calibration.getHigh());
            value( Id.CalibrationCenter, calibration.getCenter());
            value( Id.CalibrationLow, calibration.getLow());
        });
    }

    private void ppms( PPMs ppms)
        throws WriteException
    {
        complex( Id.PPMs, () -> forEach( ppms, this::ppm ));
    }

    private void ppm( PPM ppm)
        throws WriteException
    {
        complex( Id.PPM, () ->
        {
            value( Id.PPMInverted, ppm.getInverted());
            value( Id.PPMCenter, ppm.getCenter());
            value( Id.PPMName, ppm.getName());

            channelMappings( ppm.getChannelMappings());
        });
    }

    private void channelMappings( ChannelMappings channelMappings)
        throws WriteException
    {
        complex( Id.ChannelMappings, () ->
            forEach( channelMappings,( channelMapping) -> value( Id.ChannelMapping, channelMapping))
        );
    }

    private void types( Types types, int total)
        throws WriteException
    {
        complex( Id.Types, () ->
        {
            // Type ids don't start with zero!
            int typeId = Model.TYPE_START;

            for( Type type: types)
            {
                // Fill with empty types until we reach the current type id.
                typeId = fillTypes( typeId, type.getId().getValue());

                type( type);

                typeId++;

                configurationProgress.setTypesCount( typeId - Model.TYPE_START);
            }

            fillTypes( typeId, Model.TYPE_START + total - 1);
        });
    }

    private int fillTypes( int typeId, int types)
        throws WriteException
    {
        while( typeId < types)
        {
            emptyType();

            typeId++;

            configurationProgress.setTypesCount( typeId - Model.TYPE_START);
        }

        return typeId;
    }

    private void emptyType()
        throws WriteException
    {
        complex( Id.Type, () -> {});
    }

    private void type( Type type)
        throws WriteException
    {
        complex( Id.Type, () ->
        {
            value( Id.TypeName, type.getName());
            value( Id.TypeState, Utility.convertTypeState( type.getState()));
        });
    }

    private void models( Models models, int total)
        throws WriteException
    {
        complex( Id.Models, () ->
        {
            int modelId = 0;

            for( Model model: models)
            {
                // Fill with empty models until we reach the current model id.
                modelId = fillModels( modelId, model.getId().getValue());

                model( model);

                modelId++;

                configurationProgress.setModelsCount( modelId);
            }

            fillModels( modelId, total - 1);
        });
    }

    private int fillModels( int modelId, int models)
        throws WriteException
    {
        while( modelId < models)
        {
            emptyModel();

            modelId++;

            configurationProgress.setTypesCount( modelId);
        }

        return modelId;
    }

    private void emptyModel()
        throws WriteException
    {
        complex( Id.Model, () -> {});
    }

    private void model( Model model)
        throws WriteException
    {
        complex( Id.Model, () ->
        {
            value( Id.ModelName, model.getName());
            value( Id.ModelState, Utility.convertModelState( model.getState()));
            value( Id.ModelRFMode, model.getRfMode());
            value( Id.ModelType, model.getTypeId());

            statusSources( model);
            statusTimes( model);
            channels( model.getChannels());
            proxyReferences( model.getProxyReferences());
        });
    }

    private void sources( Sources sources, int total)
        throws WriteException
    {
        complex( Id.Sources, () ->
        {
            int sourceId = 0;

            for( Source source: sources)
            {
                // Fill with empty sources until we reach the current source id.
                sourceId = fillSources( sourceId, source.getId().getValue());

                source( source);

                sourceId++;

                configurationProgress.setSourcesCount( sourceId);
            }

            fillSources( sourceId, total - 1);
        });
    }

    private int fillSources( int sourceId, int sources)
        throws WriteException
    {
        while( sourceId < sources)
        {
            emptySource();

            sourceId++;

            configurationProgress.setSourcesCount( sourceId);
        }

        return sourceId;
    }

    private void emptySource()
        throws WriteException
    {
        complex( Id.Source, () -> {});
    }

    private void source( Source source)
        throws WriteException
    {
        complex( Id.Source, () ->
        {
            value( Id.SourceName, source.getName());
            value( Id.SourceModel, source.getModelId());

            if( source instanceof Analog)
            {
                analog(( Analog) source);
            }
            else if( source instanceof Button)
            {
                button(( Button) source);
            }
            else if( source instanceof Rotary)
            {
                rotary(( Rotary) source);
            }
            else if( source instanceof Switch)
            {
                aSwitch(( Switch) source);
            }
            else if( source instanceof Ticker)
            {
                ticker(( Ticker) source);
            }
            else if( source instanceof Map)
            {
                map(( Map) source);
            }
            else if( source instanceof Mix)
            {
                mix(( Mix) source);
            }
            else if( source instanceof Store)
            {
                store(( Store) source);
            }
            else if( source instanceof Proxy)
            {
                proxy(( Proxy) source);
            }
            else if( source instanceof Follower)
            {
                follower(( Follower) source);
            }
            else if( source instanceof Timer)
            {
                timer(( Timer) source);
            }
            else if( source instanceof Trimmer)
            {
                trimmer(( Trimmer) source);
            }
        });
    }

    private void analog( Analog analog)
        throws WriteException
    {
        complex( Id.SourceInputAnalog, () -> value( Id.SourceInputAnalogInput, analog.getInputId()));
    }

    private void button( Button button)
        throws WriteException
    {
        complex( Id.SourceInputButton, () ->
        {
            value( Id.SourceInputButtonInput, button.getInputId());
            value( Id.SourceInputButtonInit, button.getInit());
            value( Id.SourceInputButtonStore, button.getStore());
            value( Id.SourceInputButtonToggle, button.getToggle());
            value( Id.SourceInputButtonTop, button.getTop());
            value( Id.SourceInputButtonBottom, button.getBottom());
        });
    }

    private void rotary( Rotary rotary)
        throws WriteException
    {
        complex( Id.SourceInputRotary, () ->
        {
            value( Id.SourceInputRotaryAInput, rotary.getAInputId());
            value( Id.SourceInputRotaryBInput, rotary.getBInputId());
            value( Id.SourceInputRotaryStore, rotary.getStore());
            value( Id.SourceInputRotaryInit, rotary.getInit());
            value( Id.SourceInputRotaryStep, rotary.getStep());
            value( Id.SourceInputRotaryTop, rotary.getTop());
            value( Id.SourceInputRotaryBottom, rotary.getBottom());
        });
    }

    private void aSwitch( Switch aSwitch)
        throws WriteException
    {
        complex( Id.SourceInputSwitch, () ->
        {
            value( Id.SourceInputSwitchLowInput, aSwitch.getLowInputId());
            value( Id.SourceInputSwitchHighInput, aSwitch.getHighInputId());
            value( Id.SourceInputSwitchTop, aSwitch.getTop());
            value( Id.SourceInputSwitchBottom, aSwitch.getBottom());
        });
    }

    private void ticker( Ticker ticker)
        throws WriteException
    {
        complex( Id.SourceInputTicker, () ->
        {
            value( Id.SourceInputTickerLowInput, ticker.getLowInputId());
            value( Id.SourceInputTickerHighInput, ticker.getHighInputId());
            value( Id.SourceInputTickerInit, ticker.getInit());
            value( Id.SourceInputTickerStep, ticker.getStep());
            value( Id.SourceInputTickerStore, ticker.getStore());
            value( Id.SourceInputTickerTop, ticker.getTop());
            value( Id.SourceInputTickerBottom, ticker.getBottom());
        });
    }

    private void map( Map map)
        throws WriteException
    {
        complex( Id.SourceMap, () ->
        {
            sourceTuple( Id.SourceMapInput, map.getInput());

            complex( Id.SourceMapPoints, () ->
                forEach( map.getPoints(),( point) -> sourceTuple( Id.SourceMapPoint, point))
            );
        });
    }

    private void mix( Mix mix)
        throws WriteException
    {
        complex( Id.SourceMix, () ->
            complex( Id.SourceMixInputs, () ->
                forEach( mix.getInputs(),( input) -> sourceTuple( Id.SourceMixInput, input))
            )
        );
    }

    private void store( Store store)
        throws WriteException
    {
        complex( Id.SourceStore, () ->
        {
            value( Id.SourceStoreInput, store.getInput());
            value( Id.SourceStoreInit, store.getInit());
        });
    }

    private void follower( Follower follower)
        throws WriteException
    {
        complex( Id.SourceFollower, () ->
        {
            sourceTuple( Id.SourceFollowerTarget, follower.getTarget());
            sourceTuple( Id.SourceFollowerStep, follower.getStep());

            value( Id.SourceFollowerTrigger, follower.getTrigger());
            value( Id.SourceFollowerTriggerLowLimit, follower.getTriggerLowLimit());
            value( Id.SourceFollowerTriggerHighLimit, follower.getTriggerHighLimit());
        });
    }

    private void timer( Timer timer)
        throws WriteException
    {
        complex( Id.SourceTimer, () ->
        {
            value( Id.SourceTimerInitTime, timer.getInitTime());
            value( Id.SourceTimerCurrentTime, timer.getCurrentTime());

            value( Id.SourceTimerStore, timer.getStore());
            value( Id.SourceTimerReverse, timer.getReverse());

            value( Id.SourceTimerTrigger, timer.getTrigger());
            value( Id.SourceTimerTriggerLowLimit, timer.getTriggerLowLimit());
            value( Id.SourceTimerTriggerHighLimit, timer.getTriggerHighLimit());

            value( Id.SourceTimerWarnLowTime, timer.getWarnLowTime());
            value( Id.SourceTimerWarnCriticalTime, timer.getWarnCriticalTime());
            value( Id.SourceTimerWarnPauseTime, timer.getWarnPauseTime());
        });
    }

    private void trimmer( Trimmer trimmer)
        throws WriteException
    {
        complex( Id.SourceTrimmer, () ->
        {
            value( Id.SourceTrimmerReverse, trimmer.getReverse());

            sourceTuple( Id.SourceTrimmerInput, trimmer.getInput());
            sourceTuple( Id.SourceTrimmerTrim, trimmer.getTrim());
            sourceTuple( Id.SourceTrimmerLimit, trimmer.getLimit());

            trimmerPoints( trimmer);
        });
    }

    private void trimmerPoints( Trimmer trimmer)
        throws WriteException
    {
        complex( Id.SourceTrimmerPoints, () ->
            forEach( trimmer.getPoints(),( point) -> value( Id.SourceTrimmerPoint, point))
        );
    }

    private void proxy( Proxy proxy)
        throws WriteException
    {
        complex( Id.SourceProxy, () -> value( Id.SourceProxySlot, proxy.getSlot()));
    }

    private void proxyReferences( ProxyReferences proxyReferences)
        throws WriteException
    {
        complex( Id.ModelProxyReferences, () ->
            forEach( proxyReferences,( proxyReference) -> sourceTuple( Id.ModelProxyReference, proxyReference))
        );
    }

    private void statusSources( Model model)
        throws WriteException
    {
        complex( Id.StatusSources, () ->
        {
            value( Id.StatusSource, model.getStatusSourceId( StatusSource.LEFT_SIDE));
            value( Id.StatusSource, model.getStatusSourceId( StatusSource.LEFT_BOTTOM));
            value( Id.StatusSource, model.getStatusSourceId( StatusSource.RIGHT_SIDE));
            value( Id.StatusSource, model.getStatusSourceId( StatusSource.RIGHT_BOTTOM));
        });
    }

    private void statusTimes( Model model)
        throws WriteException
    {
        complex( Id.StatusTimers, () ->
        {
            value( Id.StatusTimer, model.getStatusTimeId( StatusTime.TOP));
            value( Id.StatusTimer, model.getStatusTimeId( StatusTime.BOTTOM));
        });
    }

    private void channels( Channels channels)
        throws WriteException
    {
        complex( Id.Channels, () -> forEach( channels, this::channel));
    }

    private void channel( Channel channel)
        throws WriteException
    {
        complex( Id.Channel, () ->
        {
            value( Id.ChannelName, channel.getName());
            value( Id.ChannelReverse, channel.getReverse());
            value( Id.ChannelMode, channel.getMode());

            sourceTuple( Id.ChannelInput, channel.getInput());
            sourceTuple( Id.ChannelTrim, channel.getTrim());
            sourceTuple( Id.ChannelLimit, channel.getLimit());

            channelPoints( channel);
        });
    }

    private void channelPoints( Channel channel)
        throws WriteException
    {
        complex( Id.ChannelPoints, () ->
            forEach( channel.getPoints(),( point) -> value( Id.ChannelPoint, point))
        );
    }

    private void sourceTuple( Id id, SourceWithVolume sourceTuple)
        throws WriteException
    {
        complex( id, () ->
        {
            value( Id.SourceTupleSource, sourceTuple.getSourceId());
            value( Id.SourceTupleVolume, sourceTuple.getVolume());
        });
   }

    private void complex( Id id, ThrowingFunction< WriteException> content)
        throws WriteException
    {
        connection.openComplex( id);

            content.apply();

        connection.closeComplex();
    }

    private void value( DesktopProtocol.Id id, Bool value)
        throws WriteException
    {
        value( id, value.getStringValue());
    }

    private void value( DesktopProtocol.Id id, Text value)
        throws WriteException
    {
        value( id, value.getStringValue());
    }

    private void value( DesktopProtocol.Id id, Number value)
        throws WriteException
    {
        value( id, value.getStringValue());
    }

    private void value( DesktopProtocol.Id id, int value)
        throws WriteException
    {
        value( id, java.lang.Integer.toString( value));
    }

    private void value( DesktopProtocol.Id id, String value)
        throws WriteException
    {
        connection.writeValue( id, value);
    }
}
