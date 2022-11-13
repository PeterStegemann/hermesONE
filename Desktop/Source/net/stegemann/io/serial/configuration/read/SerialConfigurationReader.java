package net.stegemann.io.serial.configuration.read;

import java.util.ArrayList;

import net.stegemann.configuration.Battery;
import net.stegemann.configuration.Calibration;
import net.stegemann.configuration.Calibrations;
import net.stegemann.configuration.Channel;
import net.stegemann.configuration.ChannelMappings;
import net.stegemann.configuration.Channels;
import net.stegemann.configuration.Configuration;
import net.stegemann.configuration.Model;
import net.stegemann.configuration.Models;
import net.stegemann.configuration.PPM;
import net.stegemann.configuration.PPMs;
import net.stegemann.configuration.ProxyReferences;
import net.stegemann.configuration.System;
import net.stegemann.configuration.Type;
import net.stegemann.configuration.Types;
import net.stegemann.configuration.Model.StatusSource;
import net.stegemann.configuration.Model.StatusTime;
import net.stegemann.configuration.source.Source;
import net.stegemann.configuration.source.Follower;
import net.stegemann.configuration.source.Map;
import net.stegemann.configuration.source.Mix;
import net.stegemann.configuration.source.Proxy;
import net.stegemann.configuration.source.Store;
import net.stegemann.configuration.source.Timer;
import net.stegemann.configuration.source.Sources;
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
import net.stegemann.configuration.type.ValueOutOfRangeException;
import net.stegemann.io.ReadException;
import net.stegemann.io.Utility;
import net.stegemann.io.serial.base.DesktopConnection;
import net.stegemann.io.serial.base.DesktopConnectionHandler;
import net.stegemann.io.serial.base.DesktopProtocol;
import net.stegemann.io.serial.base.TypedConnectionHandler;
import net.stegemann.io.serial.configuration.ConfigurationProgress;
import net.stegemann.misc.ChangeListener;

public class SerialConfigurationReader
{
	private final ArrayList< TypedConnectionHandler> handlerStack =
			new ArrayList< TypedConnectionHandler>();
	private final ConfigurationProgress configurationProgress = new ConfigurationProgress();

	public void readFromPort( Configuration configuration, String PortName,
			ChangeListener<ConfigurationProgress> configurationListener) 
			throws ReadException
	{
		TypedConnectionForwarder ConnectionForwarder = new TypedConnectionForwarder();
		DesktopConnection Connection = new DesktopConnection();

		if( Connection.open( PortName, ConnectionForwarder) == false)
		{
			throw new ReadException( "Failed to open connection with port " + PortName + ".");
		}

		configuration.clear();
		
		configurationProgress.addChangeListener( configurationListener);

		pushHandler( new StartHandler( configuration));

		try
		{
			Connection.readConfiguration();
		}
		catch( ReadException reason)
		{
			throw new ReadException( "Failed to read configuration!", reason);
		}
		finally
		{
			configurationProgress.removeChangeListener( configurationListener);	

			handlerStack.remove( 0);

			Connection.close();
		}


//		java.lang.System.err.println( "End, stack size: " + handlerStack.size());
  	}

	private class TypedConnectionForwarder implements TypedConnectionHandler
	{
		@Override
		public void complexOpened( byte UseId)
		{
//			java.lang.System.out.print( ".");

			handlerStack.get( 0).complexOpened( UseId);
		}

		@Override
		public void complexClosed()
		{
			handlerStack.get( 0).complexClosed();

			handlerStack.remove( 0);

//			java.lang.System.err.println( "Complex closed, stack size: " + handlerStack.size());
		}

		@Override
		public void valueRead( byte UseId, String value)
		{
			handlerStack.get( 0).valueRead( UseId, value);
		}
	}

	private class UnknownTypeHandler extends DesktopConnectionHandler
	{
		@Override
		public void complexOpened( DesktopProtocol.Id UseId)
		{
			java.lang.System.out.println( "Unknown complex " + UseId);

			pushHandler( new UnknownTypeHandler());
		}

		@Override
		public void complexClosed()
		{
		}

		@Override
		public void valueRead( DesktopProtocol.Id UseId, String TextContent)
		{
			java.lang.System.out.println( "Unknown value " + UseId.GetByteValue() + " {" + TextContent + "}");
 		}

		protected void readValue( Number value, String TextContent)
		{
			try
			{
				value.setConfigurationValue( TextContent);
			}
			catch( NumberFormatException e)
			{
			}
			catch( ValueOutOfRangeException e)
			{
			}		
		}

		protected void readValue( Text value, String TextContent)
		{
			value.setConfigurationValue( TextContent);
		}

		protected void readValue( Bool value, String TextContent)
		{
			value.setConfigurationValue( TextContent);
		}
	}

	private class StartHandler
		extends UnknownTypeHandler
	{
		final Configuration configuration;

		public StartHandler( Configuration configuration)
		{
			this.configuration = configuration;
		}

		@Override
		public void complexOpened( DesktopProtocol.Id UseId)
		{
			switch( UseId)
			{
				case Configuration :
				{
					pushHandler( new ConfigurationHandler( configuration));
				}
				break;

				default : super.complexOpened( UseId); break;
			}
 		}
	}

	private class ConfigurationHandler
		extends UnknownTypeHandler
	{
		final Configuration configuration;

		public ConfigurationHandler( Configuration configuration)
		{
			this.configuration = configuration;
		}

		@Override
		public void complexOpened( DesktopProtocol.Id UseId)
		{
			System UseSystem = configuration.getSystem();

			switch( UseId)
			{
				case Battery :
				{
					pushHandler( new BatteryHandler( UseSystem.getBattery()));
				}
				break;

				case Calibrations :
				{
					pushHandler( new CalibrationsHandler( UseSystem.getCalibrations()));
				}
				break;

				case Models :
				{
					pushHandler( new ModelsHandler( configuration.getModels(), configuration));
				}
				break;

				case Types :
				{
					pushHandler( new TypesHandler( configuration.getTypes(), configuration));
				}
				break;

				case Sources :
				{
					pushHandler( new SourcesHandler( configuration.getSources(), configuration));
				}
				break;

				case PPMs :
				{
					pushHandler( new PPMsHandler( UseSystem.getPPMs()));
				}
				break;

				default : super.complexOpened( UseId); break;
			}
 		}

		@Override
		public void complexClosed()
		{
			configuration.fill();
		}

		@Override
		public void valueRead( DesktopProtocol.Id UseId, String TextContent)
		{
			System UseSystem = configuration.getSystem();

			switch( UseId)
			{
				case Owner :
				{
					readValue( UseSystem.getOwner(), TextContent);
				}
				break;

				case AnalogInputs :
				{
					readValue( UseSystem.getAnalogInputs(), TextContent);
				}
				break;

				case DigitalInputs :
				{
					readValue( UseSystem.getDigitalInputs(), TextContent);
				}
				break;

				case OutputChannels :
				{
					readValue( UseSystem.getOutputChannels(), TextContent);
				}
				break;

				case Outputs :
				{
					readValue( UseSystem.getOutputs(), TextContent);
				}
				break;

				case SetupBacklight :
				{
					readValue( UseSystem.getSetupBacklight(), TextContent);
				}
				break;

				case SetupBlankTime :
				{
					readValue( UseSystem.getSetupBlankTime(), TextContent);
				}
				break;

				case StatusBacklight :
				{
					readValue( UseSystem.getStatusBacklight(), TextContent);
				}
				break;

				case StatusContrast :
				{
					readValue( UseSystem.getStatusContrast(), TextContent);
				}
				break;

				case StatusBlankTime :
				{
					readValue( UseSystem.getStatusBlankTime(), TextContent);
				}
				break;

				case StatusInverted :
				{
					readValue( UseSystem.getStatusInverted(), TextContent);
				}
				break;

				case SelectedModel :
				{
					readValue( UseSystem.getSelectedModel(), TextContent);
				}
				break;

				default : super.valueRead( UseId, TextContent); break;
			}
		}
	}

	private class BatteryHandler extends UnknownTypeHandler
	{
		private Battery battery;

		public BatteryHandler( Battery UseBattery)
		{
			battery = UseBattery;
		}

		@Override
		public void valueRead( DesktopProtocol.Id UseId, String TextContent)
		{			
			switch( UseId)
			{
				case BatteryWarnLowVoltage :
				{
					readValue( battery.getWarnLowVoltage(), TextContent);
				}
				break;

				case BatteryWarnCriticalVoltage :
				{
					readValue( battery.getWarnCriticalVoltage(), TextContent);
				}
				break;

				case BatteryMinimumVoltage :
				{
					readValue( battery.getMinimumVoltage(), TextContent);
				}
				break;

				case BatteryMaximumVoltage :
				{
					readValue( battery.getMaximumVoltage(), TextContent);
				}
				break;

				case BatteryCalibrationVoltage :
				{
					readValue( battery.getCalibrationVoltage(), TextContent);
				}
				break;

				default : super.valueRead( UseId, TextContent); break;
			}
		}
	}

	private class CalibrationsHandler extends UnknownTypeHandler
	{
		private Calibrations calibrations;
		private int calibrationIndex;

		public CalibrationsHandler( Calibrations UseCalibrations)
		{
			calibrations = UseCalibrations;
			calibrationIndex = 0;
		}

		@Override
		public void complexOpened( DesktopProtocol.Id UseId)
		{
			switch( UseId)
			{
				case Calibration :
				{
					Calibration NewCalibration = new Calibration();

					pushHandler( new CalibrationHandler( NewCalibration));

					calibrations.setCalibration( calibrationIndex, NewCalibration);

					calibrationIndex++;
				}
				break;

				default : super.complexOpened( UseId); break;
			}
 		}
	}

	private class CalibrationHandler extends UnknownTypeHandler
	{
		private Calibration calibration;

		public CalibrationHandler( Calibration UseCalibration)
		{
			calibration = UseCalibration;
		}

		@Override
		public void valueRead( DesktopProtocol.Id UseId, String TextContent)
		{			
			switch( UseId)
			{
				case CalibrationHigh :
				{
					readValue( calibration.getHigh(), TextContent);
				}
				break;

				case CalibrationCenter :
				{
					readValue( calibration.getCenter(), TextContent);
				}
				break;

				case CalibrationLow :
				{
					readValue( calibration.getLow(), TextContent);
				}
				break;

				default : super.valueRead( UseId, TextContent); break;
			}
		}
	}

	private class ModelsHandler extends UnknownTypeHandler
	{
		private final Models models;
		private final Configuration configuration;

		private int modelId;

		public ModelsHandler( Models models, Configuration configuration)
		{
			this.models = models;
			this.configuration = configuration;

			modelId = 0;
		}

		@Override
		public void complexOpened( DesktopProtocol.Id UseId)
		{
			switch( UseId)
			{
				case Model :
				{
					Model NewModel = new Model();

					pushHandler( new ModelHandler( models, NewModel, configuration));

					try
					{
						NewModel.getId().setValue( modelId);
					}
					catch( ValueOutOfRangeException e)
					{
					}

					// Always count up model id, it doesn't matter if a model was skipped.
					modelId++;
				}
				break;

				default : super.complexOpened( UseId); break;
			}
 		}
	}

	private class ModelHandler extends UnknownTypeHandler
	{
		private final Models models;
		private final Model model;
		private final Configuration configuration;

		public ModelHandler( Models UseModels, Model UseModel, Configuration configuration)
		{
			models = UseModels;
			model = UseModel;
			this.configuration = configuration;
		}

		@Override
		public void complexOpened( DesktopProtocol.Id UseId)
		{
			switch( UseId)
			{
				case StatusSources :
				{
					pushHandler( new StatusSourcesHandler( model));
				}
				break;

				case StatusTimers :
				{
					pushHandler( new StatusTimesHandler( model));
				}
				break;

				case Channels :
				{
					pushHandler( new ChannelsHandler( model.getChannels()));
				}
				break;

				case ModelProxyReferences :
				{
					pushHandler( new ProxyReferencesHandler( model.getProxyReferences()));
				}
				break;

				default : super.complexOpened( UseId); break;
			}
		}

		@Override
		public void complexClosed()
		{
			if( model.getState() == Model.State.USED)
			{
				models.addModel( model);

				configurationProgress.setModelCount(configuration.getModels().getCount());
			}
		}

		@Override
		public void valueRead( DesktopProtocol.Id UseId, String TextContent)
		{			
			switch( UseId)
			{
				case ModelName :
				{
					readValue( model.getName(), TextContent);
				}
				break;

				case ModelState :
				{
					model.setState( Utility.convertModelState( TextContent));
				}
				break;

				case ModelType :
				{
					readValue( model.getTypeId(), TextContent);
				}
				break;

				case ModelRFMode :
				{
					readValue( model.getRFMode(), TextContent);
				}
				break;

				default : super.valueRead( UseId, TextContent); break;
			}
 		}
	}

	private class StatusSourcesHandler extends UnknownTypeHandler
	{
		private Model model;
		private int statusSourceIndex;

		public StatusSourcesHandler( Model UseModel)
		{
			model = UseModel;
			statusSourceIndex = 0;
		}

		@Override
		public void valueRead( DesktopProtocol.Id UseId, String TextContent)
		{
			switch( UseId)
			{
				case StatusSource :
				{
					readValue( model.getStatusSourceId( StatusSource.values()[ statusSourceIndex]),
						TextContent);

					statusSourceIndex++;
				}
				break;

				default : super.valueRead( UseId, TextContent); break;
			}
 		}
	}

	private class StatusTimesHandler extends UnknownTypeHandler
	{
		private Model model;
		private int statusTimeIndex;

		public StatusTimesHandler( Model UseModel)
		{
			model = UseModel;
			statusTimeIndex = 0;
		}

		@Override
		public void valueRead( DesktopProtocol.Id UseId, String TextContent)
		{
			switch( UseId)
			{
				case StatusTimer :
				{
					readValue( model.getStatusTimeId( StatusTime.values()[ statusTimeIndex]),
						TextContent);

					statusTimeIndex++;
				}
				break;

				default : super.valueRead( UseId, TextContent); break;
			}
 		}
	}

	private class ChannelsHandler extends UnknownTypeHandler
	{
		private Channels channels;
		private int channelIndex;

		public ChannelsHandler( Channels UseChannels)
		{
			channels = UseChannels;
			channelIndex = 0;
		}

		@Override
		public void complexOpened( DesktopProtocol.Id UseId)
		{
			switch( UseId)
			{
				case Channel :
				{
					Channel NewChannel = new Channel();

					channels.setChannel( channelIndex++, NewChannel);

					pushHandler( new ChannelHandler( NewChannel));
				}
				break;

				default : super.complexOpened( UseId); break;
			}
 		}
	}

	private class ChannelHandler extends UnknownTypeHandler
	{
		private Channel channel;

		public ChannelHandler( Channel UseChannel)
		{
			channel = UseChannel;
		}

		@Override
		public void complexOpened( DesktopProtocol.Id UseId)
		{
			switch( UseId)
			{
				case ChannelInput :
				{
					pushHandler( new SourceTupelHandler( channel.getInput()));
				}
				break;

				case ChannelTrim :
				{
					pushHandler( new SourceTupelHandler( channel.getTrim()));
				}
				break;

				case ChannelLimit :
				{
					pushHandler( new SourceTupelHandler( channel.getLimit()));
				}
				break;

				case ChannelPoints :
				{
					pushHandler( new ChannelPointsHandler( channel));
				}
				break;

				default : super.complexOpened( UseId); break;
			}
 		}

		@Override
		public void valueRead( DesktopProtocol.Id UseId, String TextContent)
		{			
			switch( UseId)
			{
				case ChannelName :
				{
					readValue( channel.getName(), TextContent);
				}
				break;

				case ChannelReverse :
				{
					readValue( channel.getReverse(), TextContent);
				}
				break;

				case ChannelMode :
				{
					readValue( channel.getMode(), TextContent);
				}
				break;

				default : super.valueRead( UseId, TextContent); break;
			}
 		}
	}

	private class ChannelPointsHandler extends UnknownTypeHandler
	{
		private Channel channel;
		private int channelPointIndex;

		public ChannelPointsHandler( Channel UseChannel)
		{
			channel = UseChannel;
			channelPointIndex = 0;
		}

		@Override
		public void valueRead( DesktopProtocol.Id UseId, String TextContent)
		{			
			switch( UseId)
			{
				case ChannelPoint :
				{
					readValue( channel.getPoint( channelPointIndex++), TextContent);
				}
				break;

				default : super.valueRead( UseId, TextContent); break;
			}
 		}
	}

	private class ProxyReferencesHandler extends UnknownTypeHandler
	{
		private ProxyReferences proxyReferences;
		private int proxyIndex;

		public ProxyReferencesHandler( ProxyReferences UseProxyReferences)
		{
			proxyReferences = UseProxyReferences;
			proxyIndex = 0;
		}

		@Override
		public void complexOpened( DesktopProtocol.Id UseId)
		{
			switch( UseId)
			{
				case ModelProxyReference :
				{
					pushHandler( new SourceTupelHandler( proxyReferences.GetProxyReferenceFromIndex(
						proxyIndex++)));
				}
				break;

				default : super.complexOpened( UseId); break;
			}
 		}
	}

	private class TypesHandler extends UnknownTypeHandler
	{
		private final Types types;
		private final Configuration configuration;

		private int typeId;

		public TypesHandler( Types types, Configuration configuration)
		{
			this.types = types;
			this.configuration = configuration;

			// TypeIds don't start with zero!
			typeId = Model.TYPE_START;
		}

		@Override
		public void complexOpened( DesktopProtocol.Id UseId)
		{
			switch( UseId)
			{
				case Type :
				{
					Type NewType = new Type();

					try
					{
						NewType.getId().setValue( typeId);
					}
					catch( ValueOutOfRangeException e)
					{
					}

					// Always count up type id, it doesn't matter if a type was skipped.
					typeId++;

					pushHandler( new TypeHandler( types, NewType, configuration));
				}
				break;

				default : super.complexOpened( UseId); break;
			}
 		}
	}

	private class TypeHandler extends UnknownTypeHandler
	{
		private final Types types;
		private final Type type;
		private final Configuration configuration;

		public TypeHandler( Types UseTypes, Type UseType, Configuration configuration)
		{
			types = UseTypes;
			type = UseType;
			this.configuration = configuration;
		}

		@Override
		public void complexClosed()
		{
			if( type.getState() == Type.State.USED)
			{
				types.addType( type);

				configurationProgress.setTypeCount( configuration.getTypes().getCount());
			}
		}

		@Override
		public void valueRead( DesktopProtocol.Id UseId, String TextContent)
		{			
			switch( UseId)
			{
				case TypeName :
				{
					readValue( type.getName(), TextContent);
				}
				break;

				case TypeState :
				{
					type.setState( Utility.convertTypeState( TextContent));
				}
				break;

				default : super.valueRead( UseId, TextContent); break;
			}
 		}
	}

	private class SourcesHandler extends UnknownTypeHandler
	{
		private final Sources sources;
		private final Configuration configuration;

		private int sourceId;

		public SourcesHandler( Sources sources, Configuration configuration)
		{
			this.sources = sources;
			this.configuration = configuration;

			sourceId = 0;
		}

		@Override
		public void complexOpened( DesktopProtocol.Id UseId)
		{
			switch( UseId)
			{
				case Source :
				{
					pushHandler( new SourceHandler( sources, sourceId++, configuration));
				}
				break;

				default : super.complexOpened( UseId); break;
			}
 		}
	}

	private class SourceHandler extends UnknownTypeHandler
	{
		private final Sources sources;
		private final int sourceId;
		private final Configuration configuration;

		private Source newSource = null;
		private String name = null;
		private Number modelId = new Number( Model.MODEL_START, Model.MODEL_NONE);

		public SourceHandler( Sources UseSources, int UseSourceId, Configuration configuration)
		{
			sources = UseSources;
			sourceId = UseSourceId;
			this.configuration = configuration;
		}

		@Override
		public void complexOpened( DesktopProtocol.Id UseId)
		{
			switch( UseId)
			{
				case SourceInputAnalog :
				{
					Analog NewSource = new Analog();
					newSource = NewSource;

					pushHandler( new SourceInputAnalogHandler( NewSource));
				}
				break;

				case SourceInputButton :
				{
					Button NewSource = new Button();
					newSource = NewSource;

					pushHandler( new SourceInputButtonHandler( NewSource));
				}
				break;

				case SourceInputRotary :
				{
					Rotary NewSource = new Rotary();
					newSource = NewSource;

					pushHandler( new SourceInputRotaryHandler( NewSource));
				}
				break;

				case SourceInputSwitch :
				{
					Switch NewSource = new Switch();
					newSource = NewSource;

					pushHandler( new SourceInputSwitchHandler( NewSource));
				}
				break;

				case SourceInputTicker :
				{
					Ticker NewSource = new Ticker();
					newSource = NewSource;

					pushHandler( new SourceInputTickerHandler( NewSource));
				}
				break;

				case SourceMap :
				{
					Map NewSource = new Map();
					newSource = NewSource;

					pushHandler( new SourceMapHandler( NewSource));
				}
				break;

				case SourceMix :
				{
					Mix NewSource = new Mix();
					newSource = NewSource;

					pushHandler( new SourceMixHandler( NewSource));
				}
				break;

				case SourceStore :
				{
					Store NewSource = new Store();
					newSource = NewSource;

					pushHandler( new SourceStoreHandler( NewSource));
				}
				break;

				case SourceFollower :
				{
					Follower NewSource = new Follower();
					newSource = NewSource;

					pushHandler( new SourceFollowerHandler( NewSource));
				}
				break;

				case SourceTimer :
				{
					Timer NewSource = new Timer();
					newSource = NewSource;

					pushHandler( new SourceTimerHandler( NewSource));
				}
				break;

				case SourceProxy :
				{
					Proxy NewSource = new Proxy();
					newSource = NewSource;

					pushHandler( new SourceProxyHandler( NewSource));
				}
				break;

				case SourceTrimmer :
				{
					Trim NewSource = new Trim();
					newSource = NewSource;

					pushHandler( new SourceTrimmerHandler( NewSource));
				}
				break;

				default : super.complexOpened( UseId); break;
			}
 		}

		@Override
		public void complexClosed()
		{
			if(( newSource != null) && ( name != null)/*&& ( NewSource.GetType() != Source.Type.EMPTY)*/)
			{
				try
				{
					newSource.getId().setValue( sourceId);
					readValue( newSource.getName(), name);
					newSource.setModel( modelId);

					sources.addSource( newSource);

					configurationProgress.setSourceCount(configuration.getSources().getCount());
				}
				catch( Exception Reason) {}
			}
		}

		@Override
		public void valueRead( DesktopProtocol.Id UseId, String TextContent)
		{			
			switch( UseId)
			{
				case SourceName :
				{
					name = TextContent;
				}
				break;

				case SourceModel :
				{
					readValue( modelId, TextContent);
				}
				break;

				default : super.valueRead( UseId, TextContent); break;
			}
 		}
	}

	private class SourceInputAnalogHandler extends UnknownTypeHandler
	{
		private Analog source;

		public SourceInputAnalogHandler( Analog UseSource)
		{
			source = UseSource;
		}

		@Override
		public void valueRead( DesktopProtocol.Id UseId, String TextContent)
		{			
			switch( UseId)
			{
				case SourceInputAnalogInput :
				{
					readValue( source.getInputId(), TextContent);
				}
				break;

				default : super.valueRead( UseId, TextContent); break;
			}
 		}
	}

	private class SourceInputButtonHandler extends UnknownTypeHandler
	{
		private Button source;

		public SourceInputButtonHandler( Button UseSource)
		{
			source = UseSource;
		}

		@Override
		public void valueRead( DesktopProtocol.Id UseId, String TextContent)
		{			
			switch( UseId)
			{
				case SourceInputButtonInput :
				{
					readValue( source.getInputId(), TextContent);
				}
				break;

				case SourceInputButtonInit :
				{
					readValue( source.getInit(), TextContent);
				}
				break;

				case SourceInputButtonStore :
				{
					readValue( source.getStore(), TextContent);
				}
				break;

				case SourceInputButtonToggle :
				{
					readValue( source.getToggle(), TextContent);
				}
				break;

				case SourceInputButtonTop :
				{
					readValue( source.getTop(), TextContent);
				}
				break;

				case SourceInputButtonBottom :
				{
					readValue( source.getBottom(), TextContent);
				}
				break;

				default : super.valueRead( UseId, TextContent); break;
			}
 		}
	}

	private class SourceInputRotaryHandler extends UnknownTypeHandler
	{
		private Rotary source;

		public SourceInputRotaryHandler( Rotary UseSource)
		{
			source = UseSource;
		}

		@Override
		public void valueRead( DesktopProtocol.Id UseId, String TextContent)
		{			
			switch( UseId)
			{
				case SourceInputRotaryAInput :
				{
					readValue( source.getAInputId(), TextContent);
				}
				break;

				case SourceInputRotaryBInput :
				{
					readValue( source.getBInputId(), TextContent);
				}
				break;

				case SourceInputRotaryStore :
				{
					readValue( source.getStore(), TextContent);
				}
				break;

				case SourceInputRotaryInit :
				{
					readValue( source.getInit(), TextContent);
				}
				break;

				case SourceInputRotaryStep :
				{
					readValue( source.getStep(), TextContent);
				}
				break;

				case SourceInputRotaryTop :
				{
					readValue( source.getTop(), TextContent);
				}
				break;

				case SourceInputRotaryBottom :
				{
					readValue( source.getBottom(), TextContent);
				}
				break;

				default : super.valueRead( UseId, TextContent); break;
			}
 		}
	}

	private class SourceInputSwitchHandler extends UnknownTypeHandler
	{
		private Switch source;

		public SourceInputSwitchHandler( Switch UseSource)
		{
			source = UseSource;
		}

		@Override
		public void valueRead( DesktopProtocol.Id UseId, String TextContent)
		{			
			switch( UseId)
			{
				case SourceInputSwitchLowInput :
				{
					readValue( source.getLowInputId(), TextContent);
				}
				break;

				case SourceInputSwitchHighInput :
				{
					readValue( source.getHighInputId(), TextContent);
				}
				break;

				case SourceInputSwitchTop :
				{
					readValue( source.getTop(), TextContent);
				}
				break;

				case SourceInputSwitchBottom :
				{
					readValue( source.getBottom(), TextContent);
				}
				break;

				default : super.valueRead( UseId, TextContent); break;
			}
 		}
	}

	private class SourceInputTickerHandler extends UnknownTypeHandler
	{
		private Ticker source;

		public SourceInputTickerHandler( Ticker UseSource)
		{
			source = UseSource;
		}

		@Override
		public void valueRead( DesktopProtocol.Id UseId, String TextContent)
		{			
			switch( UseId)
			{
				case SourceInputTickerLowInput :
				{
					readValue( source.getLowInputId(), TextContent);
				}
				break;

				case SourceInputTickerHighInput :
				{
					readValue( source.getHighInputId(), TextContent);
				}
				break;

				case SourceInputTickerInit :
				{
					readValue( source.getInit(), TextContent);
				}
				break;

				case SourceInputTickerStep :
				{
					readValue( source.getStep(), TextContent);
				}
				break;

				case SourceInputTickerStore :
				{
					readValue( source.getStore(), TextContent);
				}
				break;

				case SourceInputTickerTop :
				{
					readValue( source.getTop(), TextContent);
				}
				break;

				case SourceInputTickerBottom :
				{
					readValue( source.getBottom(), TextContent);
				}
				break;

				default : super.valueRead( UseId, TextContent); break;
			}
 		}
	}

	private class SourceMapHandler extends UnknownTypeHandler
	{
		private Map source;

		public SourceMapHandler( Map UseSource)
		{
			source = UseSource;
		}

		@Override
		public void complexOpened( DesktopProtocol.Id UseId)
		{
			switch( UseId)
			{
				case SourceMapInput :
				{
					pushHandler( new SourceTupelHandler( source.getInput()));
				}
				break;

				case SourceMapPoints :
				{
					pushHandler( new SourceMapPointsHandler( source));
				}
				break;

				default : super.complexOpened( UseId); break;
			}
 		}
	}

	private class SourceMapPointsHandler extends UnknownTypeHandler
	{
		private Map source;
		private int pointIndex;

		public SourceMapPointsHandler( Map UseSource)
		{
			source = UseSource;
			pointIndex = 0;
		}

		@Override
		public void complexOpened( DesktopProtocol.Id UseId)
		{
			switch( UseId)
			{
				case SourceMapPoint :
				{
					pushHandler( new SourceTupelHandler( source.getPoint( pointIndex++)));
				}
				break;

				default : super.complexOpened( UseId); break;
			}
 		}
	}

	private class SourceMixHandler extends UnknownTypeHandler
	{
		private Mix source;

		public SourceMixHandler( Mix UseSource)
		{
			source = UseSource;
		}

		@Override
		public void complexOpened( DesktopProtocol.Id UseId)
		{
			switch( UseId)
			{
				case SourceMixInputs :
				{
					pushHandler( new SourceMixInputsHandler( source));
				}
				break;

				default : super.complexOpened( UseId); break;
			}
 		}
	}

	private class SourceMixInputsHandler extends UnknownTypeHandler
	{
		private Mix source;
		private int inputIndex;

		public SourceMixInputsHandler( Mix UseSource)
		{
			source = UseSource;
			inputIndex = 0;
		}

		@Override
		public void complexOpened( DesktopProtocol.Id UseId)
		{
			switch( UseId)
			{
				case SourceMixInput :
				{
					pushHandler( new SourceTupelHandler( source.getInput( inputIndex++)));
				}
				break;

				default : super.complexOpened( UseId); break;
			}
 		}
	}

	private class SourceFollowerHandler extends UnknownTypeHandler
	{
		private Follower source;

		public SourceFollowerHandler( Follower UseSource)
		{
			source = UseSource;
		}

		@Override
		public void complexOpened( DesktopProtocol.Id UseId)
		{
			switch( UseId)
			{
				case SourceFollowerTarget :
				{
					pushHandler( new SourceTupelHandler( source.getTarget()));
				}
				break;

				case SourceFollowerStep :
				{
					pushHandler( new SourceTupelHandler( source.getStep()));
				}
				break;

				default : super.complexOpened( UseId); break;
			}
 		}

		@Override
		public void valueRead( DesktopProtocol.Id UseId, String TextContent)
		{			
			switch( UseId)
			{
				case SourceFollowerTrigger :
				{
					readValue( source.getTriggerId(), TextContent);
				}
				break;

				case SourceFollowerTriggerLowLimit :
				{
					readValue( source.getTriggerLowLimit(), TextContent);
				}
				break;

				case SourceFollowerTriggerHighLimit :
				{
					readValue( source.getTriggerHighLimit(), TextContent);
				}
				break;

				default : super.valueRead( UseId, TextContent); break;
			}
 		}
	}

	private class SourceStoreHandler extends UnknownTypeHandler
	{
		private Store source;

		public SourceStoreHandler( Store UseSource)
		{
			source = UseSource;
		}

		@Override
		public void valueRead( DesktopProtocol.Id UseId, String TextContent)
		{			
			switch( UseId)
			{
				case SourceStoreInput :
				{
					readValue( source.getInput(), TextContent);
				}
				break;

				case SourceStoreInit :
				{
					readValue( source.getInit(), TextContent);
				}
				break;

				default : super.valueRead( UseId, TextContent); break;
			}
 		}
	}

	private class SourceTimerHandler extends UnknownTypeHandler
	{
		private Timer source;

		public SourceTimerHandler( Timer UseSource)
		{
			source = UseSource;
		}

		@Override
		public void valueRead( DesktopProtocol.Id UseId, String TextContent)
		{			
			switch( UseId)
			{
				case SourceTimerInitTime :
				{
					readValue( source.getInitTime(), TextContent);
				}
				break;

				case SourceTimerCurrentTime :
				{
					readValue( source.getCurrentTime(), TextContent);
				}
				break;

				case SourceTimerStore :
				{
					readValue( source.getStore(), TextContent);
				}
				break;

				case SourceTimerReverse :
				{
					readValue( source.getReverse(), TextContent);
				}
				break;

				case SourceTimerTrigger :
				{
					readValue( source.getTrigger(), TextContent);
				}
				break;

				case SourceTimerTriggerHighLimit :
				{
					readValue( source.getTriggerHighLimit(), TextContent);
				}
				break;

				case SourceTimerTriggerLowLimit :
				{
					readValue( source.getTriggerLowLimit(), TextContent);
				}
				break;

				case SourceTimerWarnLowTime :
				{
					readValue( source.getWarnLowTime(), TextContent);
				}
				break;

				case SourceTimerWarnCriticalTime :
				{
					readValue( source.getWarnCriticalTime(), TextContent);
				}
				break;

				case SourceTimerWarnPauseTime :
				{
					readValue( source.getWarnPauseTime(), TextContent);
				}
				break;

				default : super.valueRead( UseId, TextContent); break;
			}
 		}
	}

	private class SourceTrimmerHandler extends UnknownTypeHandler
	{
		private Trim trimmer;

		public SourceTrimmerHandler( Trim UseTrimmer)
		{
			trimmer = UseTrimmer;
		}

		@Override
		public void complexOpened( DesktopProtocol.Id UseId)
		{
			switch( UseId)
			{
				case SourceTrimmerInput :
				{
					pushHandler( new SourceTupelHandler( trimmer.getInput()));
				}
				break;

				case SourceTrimmerTrim :
				{
					pushHandler( new SourceTupelHandler( trimmer.getTrim()));
				}
				break;

				case SourceTrimmerLimit :
				{
					pushHandler( new SourceTupelHandler( trimmer.getLimit()));
				}
				break;

				case SourceTrimmerPoints :
				{
					pushHandler( new SourceTrimmerPointsHandler( trimmer));
				}
				break;

				default : super.complexOpened( UseId); break;
			}
 		}

		@Override
		public void valueRead( DesktopProtocol.Id UseId, String TextContent)
		{			
			switch( UseId)
			{
				case SourceTrimmerReverse :
				{
					readValue( trimmer.getReverse(), TextContent);
				}
				break;

				default : super.valueRead( UseId, TextContent); break;
			}
 		}
	}

	private class SourceTrimmerPointsHandler extends UnknownTypeHandler
	{
		private Trim trimmer;
		private int pointIndex;

		public SourceTrimmerPointsHandler( Trim UseTrimmer)
		{
			trimmer = UseTrimmer;
			pointIndex = 0;
		}

		@Override
		public void valueRead( DesktopProtocol.Id UseId, String TextContent)
		{			
			switch( UseId)
			{
				case SourceTrimmerPoint :
				{
					readValue( trimmer.getPoint( pointIndex++), TextContent);
				}
				break;

				default : super.valueRead( UseId, TextContent); break;
			}
 		}
	}

	private class SourceProxyHandler extends UnknownTypeHandler
	{
		private Proxy source;

		public SourceProxyHandler( Proxy UseSource)
		{
			source = UseSource;
		}

		@Override
		public void valueRead( DesktopProtocol.Id UseId, String TextContent)
		{			
			switch( UseId)
			{
				case SourceProxySlot :
				{
					readValue( source.getSlot(), TextContent);
				}
				break;

				default : super.valueRead( UseId, TextContent); break;
			}
 		}
	}

	private class PPMsHandler extends UnknownTypeHandler
	{
		private PPMs ppms;
		private int ppmIndex;

		public PPMsHandler( PPMs UsePPMs)
		{
			ppms = UsePPMs;
			ppmIndex = 0;
		}

		@Override
		public void complexOpened( DesktopProtocol.Id UseId)
		{
			switch( UseId)
			{
				case PPM :
				{
					pushHandler( new PPMHandler( ppms.getPPM( ppmIndex++)));
				}
				break;

				default : super.complexOpened( UseId); break;
			}
 		}

		@Override
		public void complexClosed()
		{
		}
	}


	private class PPMHandler extends UnknownTypeHandler
	{
		private PPM ppm;

		public PPMHandler( PPM UsePPM)
		{
			ppm = UsePPM;
		}

		@Override
		public void complexOpened( DesktopProtocol.Id UseId)
		{
			switch( UseId)
			{
				case ChannelMappings :
				{
					pushHandler( new ChannelMappingsHandler( ppm.getChannelMappings()));
				}
				break;

				default : super.complexOpened( UseId); break;
			}
 		}

		@Override
		public void valueRead( DesktopProtocol.Id UseId, String TextContent)
		{			
			switch( UseId)
			{
				case PPMInverted :
				{
					readValue( ppm.getPPMInverted(), TextContent);
				}
				break;

				case PPMCenter :
				{
					readValue( ppm.getPPMCenter(), TextContent);
				}
				break;

				case PPMName :
				{
					readValue( ppm.getName(), TextContent);
				}
				break;

				default : super.valueRead( UseId, TextContent); break;
			}
		}
	}

	private class ChannelMappingsHandler extends UnknownTypeHandler
	{
		private ChannelMappings channelMappings;
		private int channelIndex;

		public ChannelMappingsHandler( ChannelMappings UseChannelMappings)
		{
			channelMappings = UseChannelMappings;
			channelIndex = 0;
		}

		@Override
		public void valueRead( DesktopProtocol.Id UseId, String TextContent)
		{			
			switch( UseId)
			{
				case ChannelMapping :
				{
					readValue( channelMappings.getChannelMapping( channelIndex++), TextContent);
				}
				break;

				default : super.valueRead( UseId, TextContent); break;
			}
 		}
	}

	private class SourceTupelHandler extends UnknownTypeHandler
	{
		private SourceWithVolume sourceTupel;

		public SourceTupelHandler( SourceWithVolume UseSourceTupel)
		{
			sourceTupel = UseSourceTupel;
		}

		@Override
		public void valueRead( DesktopProtocol.Id UseId, String TextContent)
		{
			switch( UseId)
			{
				case SourceTupelSource :
				{
					readValue( sourceTupel.getSourceId(), TextContent);
				}
				break;

				case SourceTupelVolume :
				{
					readValue( sourceTupel.getVolume(), TextContent);
				}
				break;

				default : super.valueRead( UseId, TextContent); break;
			}
 		}
	}

	private TypedConnectionHandler pushHandler( TypedConnectionHandler connectionHandler)
	{
		handlerStack.add( 0, connectionHandler);
		
		return connectionHandler;
	}
}
