package net.stegemann.io.serial.configuration.write;

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
import net.stegemann.io.serial.configuration.ConfigurationProgress;
import net.stegemann.misc.ChangeListener;
import net.stegemann.io.serial.base.TypedConnectionHandler;
import net.stegemann.misc.ThrowingFunction;

public class SerialConfigurationWriter
{
	private final ConfigurationProgress configurationProgress = new ConfigurationProgress();

	private DesktopConnection connection;

	public void writeToPort
	(
		Configuration configuration,
		String portName,
		ChangeListener< ConfigurationProgress> configurationListener
	)
		throws WriteException
	{
		TypedConnectionForwarder connectionForwarder = new TypedConnectionForwarder();
		connection = new DesktopConnection();

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
			connection = null;
		}
	}

	private static class TypedConnectionForwarder implements TypedConnectionHandler
	{
		@Override
		public void complexOpened( byte Id)
		{
		}

		@Override
		public void complexClosed()
		{
		}

		@Override
		public void valueRead( byte Id, String Value)
		{
		}
	}

	private void exportConfiguration( Configuration UseConfiguration)
		throws WriteException
	{
		connection.openComplex( Id.Configuration);

			System UseSystem = UseConfiguration.getSystem();

 			writeValue( Id.AnalogInputs, UseSystem.getAnalogInputs());
 			writeValue( Id.DigitalInputs, UseSystem.getDigitalInputs());
 			writeValue( Id.OutputChannels, UseSystem.getOutputChannels());
 			writeValue( Id.Outputs, UseSystem.getOutputs());

 			writeValue( Id.Owner, UseSystem.getOwner());
 			writeValue( Id.SetupBacklight, UseSystem.getSetupBacklight());
 			writeValue( Id.SetupBlankTime, UseSystem.getSetupBlankTime());
 			writeValue( Id.StatusBacklight, UseSystem.getStatusBacklight());
 			writeValue( Id.StatusContrast, UseSystem.getStatusContrast());
 			writeValue( Id.StatusBlankTime, UseSystem.getStatusBlankTime());
 			writeValue( Id.StatusInverted, UseSystem.getStatusInverted());
 			writeValue( Id.SelectedModel, UseSystem.getSelectedModel());

 			exportBattery( UseSystem.getBattery());
 			exportCalibrations( UseSystem.getCalibrations());
 			exportModels( UseConfiguration.getModels());
 			exportTypes( UseConfiguration);
 			exportSources( UseConfiguration.getSources());
 			exportPPMs( UseSystem.getPPMs());

		connection.closeComplex();
	}

	private void exportPPMs( PPMs UsePPMs) throws WriteException
	{
		connection.openComplex( Id.PPMs);

			for( PPM UsePPM: UsePPMs)
			{
				exportPPM( UsePPM);
			}

		connection.closeComplex();
	}

	private void exportPPM( PPM UsePPM) throws WriteException
	{
		connection.openComplex( Id.PPM);

			writeValue( Id.PPMInverted, UsePPM.getPPMInverted());
			writeValue( Id.PPMCenter, UsePPM.getPPMCenter());
			writeValue( Id.PPMName, UsePPM.getName());

			exportChannelMappings( UsePPM.getChannelMappings());

		connection.closeComplex();
	}

	private void exportChannelMappings( ChannelMappings UseChannelMappings) throws WriteException
	{
		connection.openComplex( Id.ChannelMappings);

			for( Number CurrentChannelMapping: UseChannelMappings)
			{
				writeValue( Id.ChannelMapping, CurrentChannelMapping);
			}

		connection.closeComplex();
	}

	private void exportSources( Sources UseSources) throws WriteException
	{
		connection.openComplex( Id.Sources);

			int SourceId = 0;

			for( Source CurrentSource: UseSources)
			{
				// Fill with empty Sources until we reach the current Source id.
				while( SourceId < CurrentSource.getId().getValue())
				{
					connection.openComplex( Id.Source);
					connection.closeComplex();

					SourceId++;
				}

				exportSource( CurrentSource);

				SourceId++;

				configurationProgress.setSourceCount( SourceId);
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

	private void exportSourceInputAnalog( Analog UseSource) throws WriteException
	{
		connection.openComplex( Id.SourceInputAnalog);

			writeValue( Id.SourceInputAnalogInput, UseSource.getInputId());

		connection.closeComplex();
	}

	private void exportSourceInputButton( Button UseSource) throws WriteException
	{
		connection.openComplex( Id.SourceInputButton);

			writeValue( Id.SourceInputButtonInput, UseSource.getInputId());
			writeValue( Id.SourceInputButtonInit, UseSource.getInit());
			writeValue( Id.SourceInputButtonStore, UseSource.getStore());
			writeValue( Id.SourceInputButtonToggle, UseSource.getToggle());
			writeValue( Id.SourceInputButtonTop, UseSource.getTop());
			writeValue( Id.SourceInputButtonBottom, UseSource.getBottom());

		connection.closeComplex();
	}

	private void exportSourceInputRotary( Rotary UseSource) throws WriteException
	{
		connection.openComplex( Id.SourceInputRotary);

			writeValue( Id.SourceInputRotaryAInput, UseSource.getAInputId());
			writeValue( Id.SourceInputRotaryBInput, UseSource.getBInputId());
			writeValue( Id.SourceInputRotaryStore, UseSource.getStore());
			writeValue( Id.SourceInputRotaryInit, UseSource.getInit());
			writeValue( Id.SourceInputRotaryStep, UseSource.getStep());
			writeValue( Id.SourceInputRotaryTop, UseSource.getTop());
			writeValue( Id.SourceInputRotaryBottom, UseSource.getBottom());

		connection.closeComplex();
	}

	private void exportSourceInputSwitch( Switch UseSource) throws WriteException
	{
		connection.openComplex( Id.SourceInputSwitch);

			writeValue( Id.SourceInputSwitchLowInput, UseSource.getLowInputId());
			writeValue( Id.SourceInputSwitchHighInput, UseSource.getHighInputId());
			writeValue( Id.SourceInputSwitchTop, UseSource.getTop());
			writeValue( Id.SourceInputSwitchBottom, UseSource.getBottom());

		connection.closeComplex();
	}

	private void exportSourceInputTicker( Ticker UseSource) throws WriteException
	{
		connection.openComplex( Id.SourceInputTicker);

			writeValue( Id.SourceInputTickerLowInput, UseSource.getLowInputId());
			writeValue( Id.SourceInputTickerHighInput, UseSource.getHighInputId());
			writeValue( Id.SourceInputTickerInit, UseSource.getInit());
			writeValue( Id.SourceInputTickerStep, UseSource.getStep());
			writeValue( Id.SourceInputTickerStore, UseSource.getStore());
			writeValue( Id.SourceInputTickerTop, UseSource.getTop());
			writeValue( Id.SourceInputTickerBottom, UseSource.getBottom());

		connection.closeComplex();
	}

	private void exportSourceMap( Map UseSource) throws WriteException
	{
		connection.openComplex( Id.SourceMap);

			exportSourceTupel( UseSource.getInput(), Id.SourceMapInput);

			connection.openComplex( Id.SourceMapPoints);

				for( SourceWithVolume Input: UseSource.getPoints())
				{
					exportSourceTupel( Input, Id.SourceMapPoint);
				}

			connection.closeComplex();

		connection.closeComplex();
	}

	private void exportSourceMix( Mix UseSource) throws WriteException
	{
		connection.openComplex( Id.SourceMix);

			connection.openComplex( Id.SourceMixInputs);

			for( SourceWithVolume Input: UseSource.getInputs())
			{
				exportSourceTupel( Input, Id.SourceMixInput);
			}

			connection.closeComplex();

		connection.closeComplex();
	}

	private void exportSourceStore( Store UseSource) throws WriteException
	{
		connection.openComplex( Id.SourceStore);

			writeValue( Id.SourceStoreInput, UseSource.getInput());
			writeValue( Id.SourceStoreInit, UseSource.getInit());

		connection.closeComplex();
	}

	private void exportSourceFollower( Follower UseSource) throws WriteException
	{
		connection.openComplex( Id.SourceFollower);

			exportSourceTupel( UseSource.getTarget(), Id.SourceFollowerTarget);
			exportSourceTupel( UseSource.getStep(), Id.SourceFollowerStep);

			writeValue( Id.SourceFollowerTrigger, UseSource.getTriggerId());
			writeValue( Id.SourceFollowerTriggerLowLimit, UseSource.getTriggerLowLimit());
			writeValue( Id.SourceFollowerTriggerHighLimit, UseSource.getTriggerHighLimit());

		connection.closeComplex();
	}

	private void exportSourceTimer( Timer UseSource) throws WriteException
	{
		connection.openComplex( Id.SourceTimer);

			writeValue( Id.SourceTimerInitTime, UseSource.getInitTime());
			writeValue( Id.SourceTimerCurrentTime, UseSource.getCurrentTime());

			writeValue( Id.SourceTimerStore, UseSource.getStore());
			writeValue( Id.SourceTimerReverse, UseSource.getReverse());

			writeValue( Id.SourceTimerTrigger, UseSource.getTrigger());
			writeValue( Id.SourceTimerTriggerLowLimit, UseSource.getTriggerLowLimit());
			writeValue( Id.SourceTimerTriggerHighLimit, UseSource.getTriggerHighLimit());

			writeValue( Id.SourceTimerWarnLowTime, UseSource.getWarnLowTime());
			writeValue( Id.SourceTimerWarnCriticalTime, UseSource.getWarnCriticalTime());
			writeValue( Id.SourceTimerWarnPauseTime, UseSource.getWarnPauseTime());

		connection.closeComplex();
	}

	private void exportSourceTrimmer( Trim UseSource) throws WriteException
	{
		connection.openComplex( Id.SourceTrimmer);

			writeValue( Id.SourceTrimmerReverse, UseSource.getReverse());

			exportSourceTupel( UseSource.getInput(), Id.SourceTrimmerInput);
			exportSourceTupel( UseSource.getTrim(), Id.SourceTrimmerTrim);
			exportSourceTupel( UseSource.getLimit(), Id.SourceTrimmerLimit);

			exportSourceTrimmerPoints( UseSource);

		connection.closeComplex();
	}

	private void exportSourceTrimmerPoints( Trim UseSource) throws WriteException
	{
		connection.openComplex( Id.SourceTrimmerPoints);

			for( int PointIndex = 0; PointIndex < Channel.POINTS; PointIndex++)
			{
				Volume Point = UseSource.getPoint( PointIndex);

				writeValue( Id.SourceTrimmerPoint, Point);
			}

		connection.closeComplex();
	}

	private void exportSourceProxy( Proxy UseSource) throws WriteException
	{
		connection.openComplex( Id.SourceProxy);

			writeValue( Id.SourceProxySlot, UseSource.getSlot());

		connection.closeComplex();
	}

	private void exportTypes( Configuration UseConfiguration) throws WriteException
	{
		connection.openComplex( Id.Types);

			// TypeIds don't start with zero!
			int TypeId = Model.TYPE_START;

			Types UseTypes = UseConfiguration.getTypes();

			for( Type CurrentType: UseTypes)
			{
				// Fill with empty types until we reach the current type id.
				while( TypeId < CurrentType.getId().getValue())
				{
					connection.openComplex( Id.Type);
					connection.closeComplex();

					TypeId++;
				}

				exportType( CurrentType);

				TypeId++;

				configurationProgress.setTypeCount( TypeId);
			}

		connection.closeComplex();
	}

	private void exportType( Type UseType)
		throws WriteException
	{
		connection.openComplex( Id.Type);

			writeValue( Id.TypeName, UseType.getName());
			writeValue( Id.TypeState, Utility.convertTypeState( UseType.getState()));

		connection.closeComplex();
	}

	private void exportModels( Models UseModels)
		throws WriteException
	{
		connection.openComplex( Id.Models);

			int ModelId = 0;

			for( Model CurrentModel: UseModels)
			{
				// Fill with empty models until we reach the current model id.
				while( ModelId < CurrentModel.getId().getValue())
				{
					connection.openComplex( Id.Model);
					connection.closeComplex();

					ModelId++;
				}

				exportModel( CurrentModel);

				ModelId++;

				configurationProgress.setModelCount( ModelId);
			}

		connection.closeComplex();
	}

	private void exportModel( Model UseModel)
		throws WriteException
	{
		connection.openComplex( Id.Model);

			writeValue( Id.ModelName, UseModel.getName());
			writeValue( Id.ModelState, Utility.convertModelState( UseModel.getState()));
			writeValue( Id.ModelRFMode, UseModel.getRFMode());
			writeValue( Id.ModelType, UseModel.getTypeId());

			exportStatusSources( UseModel);
			exportStatusTimes( UseModel);
			exportChannels( UseModel.getChannels());
			exportProxyReferences( UseModel.getProxyReferences());

		connection.closeComplex();
	}

	private void exportProxyReferences( ProxyReferences UseProxyReferences)
		throws WriteException
	{
		connection.openComplex( Id.ModelProxyReferences);

			for( SourceWithVolume CurrentProxyReference: UseProxyReferences)
			{
				exportSourceTupel( CurrentProxyReference, Id.ModelProxyReference);
			}

		connection.closeComplex();
	}

	private void exportStatusSources( Model UseModel)
		throws WriteException
	{
		connection.openComplex( Id.StatusSources);

			writeValue( Id.StatusSource, UseModel.getStatusSourceId( StatusSource.LEFT_SIDE));
			writeValue( Id.StatusSource, UseModel.getStatusSourceId( StatusSource.LEFT_BOTTOM));
			writeValue( Id.StatusSource, UseModel.getStatusSourceId( StatusSource.RIGHT_SIDE));
			writeValue( Id.StatusSource, UseModel.getStatusSourceId( StatusSource.RIGHT_BOTTOM));

		connection.closeComplex();
	}

	private void exportStatusTimes( Model UseModel)
		throws WriteException
	{
		connection.openComplex( Id.StatusTimers);

			writeValue( Id.StatusTimer, UseModel.getStatusTimeId( StatusTime.TOP));
			writeValue( Id.StatusTimer, UseModel.getStatusTimeId( StatusTime.BOTTOM));

		connection.closeComplex();
	}

	private void exportChannels( Channels UseChannels)
		throws WriteException
	{
		connection.openComplex( Id.Channels);

			for( Channel CurrentChannel: UseChannels)
			{
				exportChannel( CurrentChannel);
			}

		connection.closeComplex();
	}

	private void exportChannel( Channel UseChannel)
		throws WriteException
	{
		connection.openComplex( Id.Channel);

			writeValue( Id.ChannelName, UseChannel.getName());
			writeValue( Id.ChannelReverse, UseChannel.getReverse());
			writeValue( Id.ChannelMode, UseChannel.getMode());

			exportSourceTupel( UseChannel.getInput(), Id.ChannelInput);
			exportSourceTupel( UseChannel.getTrim(), Id.ChannelTrim);
			exportSourceTupel( UseChannel.getLimit(), Id.ChannelLimit);

			exportChannelPoints( UseChannel);

		connection.closeComplex();
	}

	private void exportChannelPoints( Channel UseChannel)
		throws WriteException
	{
		connection.openComplex( Id.ChannelPoints);

			for( int PointIndex = 0; PointIndex < Channel.POINTS; PointIndex++)
			{
				Volume Point = UseChannel.getPoint( PointIndex);

				writeValue( Id.ChannelPoint, Point);
			}

		connection.closeComplex();
	}

	private void exportSourceTupel( SourceWithVolume UseSourceTupel, DesktopProtocol.Id id)
		throws WriteException
	{
		connection.openComplex( id);

			writeValue( Id.SourceTupelSource, UseSourceTupel.getSourceId());
			writeValue( Id.SourceTupelVolume, UseSourceTupel.getVolume());

		connection.closeComplex();
	}

	private void exportBattery( Battery UseBattery) throws WriteException
	{
		connection.openComplex( Id.Battery);

			writeValue( Id.BatteryWarnLowVoltage, UseBattery.getWarnLowVoltage());
			writeValue( Id.BatteryWarnCriticalVoltage, UseBattery.getWarnCriticalVoltage());
			writeValue( Id.BatteryMinimumVoltage, UseBattery.getMinimumVoltage());
			writeValue( Id.BatteryMaximumVoltage, UseBattery.getMaximumVoltage());
			writeValue( Id.BatteryCalibrationVoltage, UseBattery.getCalibrationVoltage());

		connection.closeComplex();
	}

	private void exportCalibrations( Calibrations UseCalibrations) throws WriteException
	{
		connection.openComplex( Id.Calibrations);

			for( Calibration CurrentCalibration: UseCalibrations)
			{
				exportCalibration( CurrentCalibration);
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

	private void complex( Id id, ThrowingFunction<WriteException> content)
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
