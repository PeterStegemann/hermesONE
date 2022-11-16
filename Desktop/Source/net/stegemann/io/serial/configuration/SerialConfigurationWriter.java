package net.stegemann.io.serial.configuration;

import net.stegemann.configuration.*;
import net.stegemann.configuration.Model.StatusTime;
import net.stegemann.configuration.Model.StatusSource;
import net.stegemann.configuration.System;
import net.stegemann.configuration.source.Follower;
import net.stegemann.configuration.source.Map;
import net.stegemann.configuration.source.Mix;
import net.stegemann.configuration.source.Proxy;
import net.stegemann.configuration.source.Source;
import net.stegemann.configuration.source.Sources;
import net.stegemann.configuration.source.Store;
import net.stegemann.configuration.source.Timer;
import net.stegemann.configuration.source.Trim;
import net.stegemann.configuration.source.input.Analog;
import net.stegemann.configuration.source.input.Button;
import net.stegemann.configuration.source.input.Rotary;
import net.stegemann.configuration.source.input.Switch;
import net.stegemann.configuration.source.input.Ticker;
import net.stegemann.configuration.type.Bool;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.SourceWithVolume;
import net.stegemann.configuration.type.Text;
import net.stegemann.configuration.type.Volume;
import net.stegemann.io.Utility;
import net.stegemann.io.WriteException;
import net.stegemann.io.serial.base.DesktopConnection;
import net.stegemann.io.serial.base.DesktopProtocol;
import net.stegemann.io.serial.base.DesktopProtocol.Id;
import net.stegemann.misc.ChangeListener;
import net.stegemann.io.serial.base.TypedConnectionHandler;
import net.stegemann.misc.ThrowingFunction;

public class SerialConfigurationWriter
{
	private final ConfigurationProgress configurationProgress;

	private final DesktopConnection connection;

	SerialConfigurationWriter
	(
		ConfigurationProgress useConfigurationProgress,
		DesktopConnection useConnection
	)
	{
		configurationProgress = useConfigurationProgress;
		connection = useConnection;
    }

	public void writeToPort
	(
		Configuration configuration,
		String portName,
		ChangeListener< ConfigurationProgress> configurationListener
	)
		throws WriteException
	{
		TypedConnectionForwarder connectionForwarder = new TypedConnectionForwarder();

		if( connection.open( portName, connectionForwarder) == false)
		{
			throw new WriteException( "Failed to open connection with port " + portName + ".");
		}

		configurationProgress.addChangeListener( configurationListener);

		try
		{
			connection.writeConfiguration();

			exportConfiguration( configuration);
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

	private static class TypedConnectionForwarder
		implements TypedConnectionHandler
	{
		@Override
		public void complexOpened( byte id)
		{
		}

		@Override
		public void complexClosed()
		{
		}

		@Override
		public void valueRead( byte id, String value)
		{
		}
	}

	private void exportConfiguration( Configuration useConfiguration)
		throws WriteException
	{
		connection.openComplex( Id.Configuration);

			System system = useConfiguration.getSystem();

 			writeValue( Id.AnalogInputs, system.getAnalogInputs());
 			writeValue( Id.DigitalInputs, system.getDigitalInputs());
 			writeValue( Id.OutputChannels, system.getOutputChannels());
 			writeValue( Id.Outputs, system.getOutputs());

 			writeValue( Id.Owner, system.getOwner());
 			writeValue( Id.SetupBacklight, system.getSetupBacklight());
 			writeValue( Id.SetupBlankTime, system.getSetupBlankTime());
 			writeValue( Id.StatusBacklight, system.getStatusBacklight());
 			writeValue( Id.StatusContrast, system.getStatusContrast());
 			writeValue( Id.StatusBlankTime, system.getStatusBlankTime());
 			writeValue( Id.StatusInverted, system.getStatusInverted());
 			writeValue( Id.SelectedModel, system.getSelectedModel());

 			exportBattery( system.getBattery());
 			exportCalibrations( system.getCalibrations());
 			exportModels( useConfiguration.getModels());
 			exportTypes( useConfiguration);
 			exportSources( useConfiguration.getSources());
 			exportPPMs( system.getPPMs());

		connection.closeComplex();
	}

	private void exportPPMs( PPMs ppms)
	 	throws WriteException
	{
		connection.openComplex( Id.PPMs);

			for( PPM ppm: ppms)
			{
				exportPPM( ppm);
			}

		connection.closeComplex();
	}

	private void exportPPM( PPM ppm)
	 	throws WriteException
	{
		connection.openComplex( Id.PPM);

			writeValue( Id.PPMInverted, ppm.getPPMInverted());
			writeValue( Id.PPMCenter, ppm.getPPMCenter());
			writeValue( Id.PPMName, ppm.getName());

			exportChannelMappings( ppm.getChannelMappings());

		connection.closeComplex();
	}

	private void exportChannelMappings( ChannelMappings channelMappings)
	 	throws WriteException
	{
		connection.openComplex( Id.ChannelMappings);

			for( Number channelMapping: channelMappings)
			{
				writeValue( Id.ChannelMapping, channelMapping);
			}

		connection.closeComplex();
	}

	private void exportSources( Sources sources)
		throws WriteException
	{
		connection.openComplex( Id.Sources);

			int sourceId = 0;

			for( Source source: sources)
			{
				// Fill with empty sources until we reach the current source id.
				while( sourceId < source.getId().getValue())
				{
					connection.openComplex( Id.Source);
					connection.closeComplex();

					sourceId++;
				}

				exportSource( source);

				sourceId++;

				configurationProgress.setSourceCount( sourceId);
			}

		connection.closeComplex();
	}

	private void exportSource( Source source)
		throws WriteException
	{
		connection.openComplex( Id.Source);

			writeValue( Id.SourceName, source.getName());
			writeValue( Id.SourceModel, source.getModel());

			if( source instanceof Analog)
			{
				exportSourceInputAnalog(( Analog) source);
			}
			else if( source instanceof Button)
			{
				exportSourceInputButton(( Button) source);
			}
			else if( source instanceof Rotary)
			{
				exportSourceInputRotary(( Rotary) source);
			}
			else if( source instanceof Switch)
			{
				exportSourceInputSwitch(( Switch) source);
			}
			else if( source instanceof Ticker)
			{
				exportSourceInputTicker(( Ticker) source);
			}
			else if( source instanceof Map)
			{
				exportSourceMap(( Map) source);
			}
			else if( source instanceof Mix)
			{
				exportSourceMix(( Mix) source);
			}
			else if( source instanceof Store)
			{
				exportSourceStore(( Store) source);
			}
			else if( source instanceof Proxy)
			{
				exportSourceProxy(( Proxy) source);
			}
			else if( source instanceof Follower)
			{
				exportSourceFollower(( Follower) source);
			}
			else if( source instanceof Timer)
			{
				exportSourceTimer(( Timer) source);
			}
			else if( source instanceof Trim)
			{
				exportSourceTrimmer(( Trim) source);
			}

		connection.closeComplex();
	}

	private void exportSourceInputAnalog( Analog source)
	 	throws WriteException
	{
		connection.openComplex( Id.SourceInputAnalog);

			writeValue( Id.SourceInputAnalogInput, source.getInputId());

		connection.closeComplex();
	}

	private void exportSourceInputButton( Button source)
	 	throws WriteException
	{
		connection.openComplex( Id.SourceInputButton);

			writeValue( Id.SourceInputButtonInput, source.getInputId());
			writeValue( Id.SourceInputButtonInit, source.getInit());
			writeValue( Id.SourceInputButtonStore, source.getStore());
			writeValue( Id.SourceInputButtonToggle, source.getToggle());
			writeValue( Id.SourceInputButtonTop, source.getTop());
			writeValue( Id.SourceInputButtonBottom, source.getBottom());

		connection.closeComplex();
	}

	private void exportSourceInputRotary( Rotary source)
		throws WriteException
	{
		connection.openComplex( Id.SourceInputRotary);

			writeValue( Id.SourceInputRotaryAInput, source.getAInputId());
			writeValue( Id.SourceInputRotaryBInput, source.getBInputId());
			writeValue( Id.SourceInputRotaryStore, source.getStore());
			writeValue( Id.SourceInputRotaryInit, source.getInit());
			writeValue( Id.SourceInputRotaryStep, source.getStep());
			writeValue( Id.SourceInputRotaryTop, source.getTop());
			writeValue( Id.SourceInputRotaryBottom, source.getBottom());

		connection.closeComplex();
	}

	private void exportSourceInputSwitch( Switch source)
	 	throws WriteException
	{
		connection.openComplex( Id.SourceInputSwitch);

			writeValue( Id.SourceInputSwitchLowInput, source.getLowInputId());
			writeValue( Id.SourceInputSwitchHighInput, source.getHighInputId());
			writeValue( Id.SourceInputSwitchTop, source.getTop());
			writeValue( Id.SourceInputSwitchBottom, source.getBottom());

		connection.closeComplex();
	}

	private void exportSourceInputTicker( Ticker source)
		throws WriteException
	{
		connection.openComplex( Id.SourceInputTicker);

			writeValue( Id.SourceInputTickerLowInput, source.getLowInputId());
			writeValue( Id.SourceInputTickerHighInput, source.getHighInputId());
			writeValue( Id.SourceInputTickerInit, source.getInit());
			writeValue( Id.SourceInputTickerStep, source.getStep());
			writeValue( Id.SourceInputTickerStore, source.getStore());
			writeValue( Id.SourceInputTickerTop, source.getTop());
			writeValue( Id.SourceInputTickerBottom, source.getBottom());

		connection.closeComplex();
	}

	private void exportSourceMap( Map source)
	 	throws WriteException
	{
		connection.openComplex( Id.SourceMap);

			exportSourceTupel( source.getInput(), Id.SourceMapInput);

			connection.openComplex( Id.SourceMapPoints);

				for( SourceWithVolume input: source.getPoints())
				{
					exportSourceTupel( input, Id.SourceMapPoint);
				}

			connection.closeComplex();

		connection.closeComplex();
	}

	private void exportSourceMix( Mix source)
	 	throws WriteException
	{
		connection.openComplex( Id.SourceMix);

			connection.openComplex( Id.SourceMixInputs);

			for( SourceWithVolume input: source.getInputs())
			{
				exportSourceTupel( input, Id.SourceMixInput);
			}

			connection.closeComplex();

		connection.closeComplex();
	}

	private void exportSourceStore( Store source)
	 	throws WriteException
	{
		connection.openComplex( Id.SourceStore);

			writeValue( Id.SourceStoreInput, source.getInput());
			writeValue( Id.SourceStoreInit, source.getInit());

		connection.closeComplex();
	}

	private void exportSourceFollower( Follower source)
	 	throws WriteException
	{
		connection.openComplex( Id.SourceFollower);

			exportSourceTupel( source.getTarget(), Id.SourceFollowerTarget);
			exportSourceTupel( source.getStep(), Id.SourceFollowerStep);

			writeValue( Id.SourceFollowerTrigger, source.getTriggerId());
			writeValue( Id.SourceFollowerTriggerLowLimit, source.getTriggerLowLimit());
			writeValue( Id.SourceFollowerTriggerHighLimit, source.getTriggerHighLimit());

		connection.closeComplex();
	}

	private void exportSourceTimer( Timer source)
	 	throws WriteException
	{
		connection.openComplex( Id.SourceTimer);

			writeValue( Id.SourceTimerInitTime, source.getInitTime());
			writeValue( Id.SourceTimerCurrentTime, source.getCurrentTime());

			writeValue( Id.SourceTimerStore, source.getStore());
			writeValue( Id.SourceTimerReverse, source.getReverse());

			writeValue( Id.SourceTimerTrigger, source.getTrigger());
			writeValue( Id.SourceTimerTriggerLowLimit, source.getTriggerLowLimit());
			writeValue( Id.SourceTimerTriggerHighLimit, source.getTriggerHighLimit());

			writeValue( Id.SourceTimerWarnLowTime, source.getWarnLowTime());
			writeValue( Id.SourceTimerWarnCriticalTime, source.getWarnCriticalTime());
			writeValue( Id.SourceTimerWarnPauseTime, source.getWarnPauseTime());

		connection.closeComplex();
	}

	private void exportSourceTrimmer( Trim source)
	 	throws WriteException
	{
		connection.openComplex( Id.SourceTrimmer);

			writeValue( Id.SourceTrimmerReverse, source.getReverse());

			exportSourceTupel( source.getInput(), Id.SourceTrimmerInput);
			exportSourceTupel( source.getTrim(), Id.SourceTrimmerTrim);
			exportSourceTupel( source.getLimit(), Id.SourceTrimmerLimit);

			exportSourceTrimmerPoints( source);

		connection.closeComplex();
	}

	private void exportSourceTrimmerPoints( Trim source)
	 	throws WriteException
	{
		connection.openComplex( Id.SourceTrimmerPoints);

			for( int pointIndex = 0; pointIndex < Channel.POINTS; pointIndex++)
			{
				Volume Point = source.getPoint( pointIndex);

				writeValue( Id.SourceTrimmerPoint, Point);
			}

		connection.closeComplex();
	}

	private void exportSourceProxy( Proxy source)
	 	throws WriteException
	{
		connection.openComplex( Id.SourceProxy);

			writeValue( Id.SourceProxySlot, source.getSlot());

		connection.closeComplex();
	}

	private void exportTypes( Configuration configuration)
		throws WriteException
	{
		connection.openComplex( Id.Types);

			// TypeIds don't start with zero!
			int typeId = Model.TYPE_START;

			Types types = configuration.getTypes();

			for( Type type: types)
			{
				// Fill with empty types until we reach the current type id.
				while( typeId < type.getId().getValue())
				{
					connection.openComplex( Id.Type);
					connection.closeComplex();

					typeId++;
				}

				exportType( type);

				typeId++;

				configurationProgress.setTypeCount( typeId);
			}

		connection.closeComplex();
	}

	private void exportType( Type type)
		throws WriteException
	{
		connection.openComplex( Id.Type);

			writeValue( Id.TypeName, type.getName());
			writeValue( Id.TypeState, Utility.convertTypeState( type.getState()));

		connection.closeComplex();
	}

	private void exportModels( Models models)
		throws WriteException
	{
		connection.openComplex( Id.Models);

			int modelId = 0;

			for( Model model: models)
			{
				// Fill with empty models until we reach the current model id.
				while( modelId < model.getId().getValue())
				{
					connection.openComplex( Id.Model);
					connection.closeComplex();

					modelId++;
				}

				exportModel( model);

				modelId++;

				configurationProgress.setModelCount( modelId);
			}

		connection.closeComplex();
	}

	private void exportModel( Model model)
		throws WriteException
	{
		connection.openComplex( Id.Model);

			writeValue( Id.ModelName, model.getName());
			writeValue( Id.ModelState, Utility.convertModelState( model.getState()));
			writeValue( Id.ModelRFMode, model.getRFMode());
			writeValue( Id.ModelType, model.getTypeId());

			exportStatusSources( model);
			exportStatusTimes( model);
			exportChannels( model.getChannels());
			exportProxyReferences( model.getProxyReferences());

		connection.closeComplex();
	}

	private void exportProxyReferences( ProxyReferences proxyReferences)
		throws WriteException
	{
		connection.openComplex( Id.ModelProxyReferences);

			for( SourceWithVolume proxyReference: proxyReferences)
			{
				exportSourceTupel( proxyReference, Id.ModelProxyReference);
			}

		connection.closeComplex();
	}

	private void exportStatusSources( Model model)
		throws WriteException
	{
		connection.openComplex( Id.StatusSources);

			writeValue( Id.StatusSource, model.getStatusSourceId( StatusSource.LEFT_SIDE));
			writeValue( Id.StatusSource, model.getStatusSourceId( StatusSource.LEFT_BOTTOM));
			writeValue( Id.StatusSource, model.getStatusSourceId( StatusSource.RIGHT_SIDE));
			writeValue( Id.StatusSource, model.getStatusSourceId( StatusSource.RIGHT_BOTTOM));

		connection.closeComplex();
	}

	private void exportStatusTimes( Model model)
		throws WriteException
	{
		connection.openComplex( Id.StatusTimers);

			writeValue( Id.StatusTimer, model.getStatusTimeId( StatusTime.TOP));
			writeValue( Id.StatusTimer, model.getStatusTimeId( StatusTime.BOTTOM));

		connection.closeComplex();
	}

	private void exportChannels( Channels channels)
		throws WriteException
	{
		connection.openComplex( Id.Channels);

			for( Channel channel: channels)
			{
				exportChannel( channel);
			}

		connection.closeComplex();
	}

	private void exportChannel( Channel channel)
		throws WriteException
	{
		connection.openComplex( Id.Channel);

			writeValue( Id.ChannelName, channel.getName());
			writeValue( Id.ChannelReverse, channel.getReverse());
			writeValue( Id.ChannelMode, channel.getMode());

			exportSourceTupel( channel.getInput(), Id.ChannelInput);
			exportSourceTupel( channel.getTrim(), Id.ChannelTrim);
			exportSourceTupel( channel.getLimit(), Id.ChannelLimit);

			exportChannelPoints( channel);

		connection.closeComplex();
	}

	private void exportChannelPoints( Channel channel)
		throws WriteException
	{
		connection.openComplex( Id.ChannelPoints);

			for( int pointIndex = 0; pointIndex < Channel.POINTS; pointIndex++)
			{
				Volume point = channel.getPoint( pointIndex);

				writeValue( Id.ChannelPoint, point);
			}

		connection.closeComplex();
	}

	private void exportSourceTupel( SourceWithVolume sourceTupel, DesktopProtocol.Id id)
		throws WriteException
	{
		connection.openComplex( id);

			writeValue( Id.SourceTupelSource, sourceTupel.getSourceId());
			writeValue( Id.SourceTupelVolume, sourceTupel.getVolume());

		connection.closeComplex();
	}

	private void exportBattery( Battery battery)
		throws WriteException
	{
		connection.openComplex( Id.Battery);

			writeValue( Id.BatteryWarnLowVoltage, battery.getWarnLowVoltage());
			writeValue( Id.BatteryWarnCriticalVoltage, battery.getWarnCriticalVoltage());
			writeValue( Id.BatteryMinimumVoltage, battery.getMinimumVoltage());
			writeValue( Id.BatteryMaximumVoltage, battery.getMaximumVoltage());
			writeValue( Id.BatteryCalibrationVoltage, battery.getCalibrationVoltage());

		connection.closeComplex();
	}

	private void exportCalibrations( Calibrations calibrations)
		throws WriteException
	{
		connection.openComplex( Id.Calibrations);

			for( Calibration calibration: calibrations)
			{
				exportCalibration( calibration);
			}

		connection.closeComplex();
	}

	private void exportCalibration( Calibration calibration)
		throws WriteException
	{
		complex( Id.Calibration, () ->
		{
			writeValue( Id.CalibrationHigh, calibration.getHigh());
			writeValue( Id.CalibrationCenter, calibration.getCenter());
			writeValue( Id.CalibrationLow, calibration.getLow());
		});
	}

	private void complex( Id id, ThrowingFunction< WriteException> content)
		throws WriteException
	{
		connection.openComplex( id);

			content.apply();

		connection.closeComplex();
	}

	private void writeValue( DesktopProtocol.Id id, Bool value)
			throws WriteException
	{
		writeValue( id, value.getConfigurationValue());
	}

	private void writeValue( DesktopProtocol.Id id, Text value)
			throws WriteException
	{
		writeValue( id, value.getConfigurationValue());
	}

	private void writeValue( DesktopProtocol.Id id, Number value)
			throws WriteException
	{
		writeValue( id, value.getConfigurationValue());
	}

	private void writeValue( DesktopProtocol.Id id, int value)
		throws WriteException
	{
		writeValue( id, java.lang.Integer.toString( value));
	}

	private void writeValue( DesktopProtocol.Id id, String value)
		throws WriteException
	{
		connection.writeValue( id, value);
	}
}
