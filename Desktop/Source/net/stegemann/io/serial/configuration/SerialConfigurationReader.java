package net.stegemann.io.serial.configuration;

import net.stegemann.configuration.*;
import net.stegemann.configuration.Model.StatusSource;
import net.stegemann.configuration.System;
import net.stegemann.configuration.Model.StatusTime;
import net.stegemann.configuration.source.*;
import net.stegemann.configuration.source.input.*;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.*;
import net.stegemann.io.ReadException;
import net.stegemann.io.Utility;
import net.stegemann.io.serial.base.DesktopConnection;
import net.stegemann.io.serial.base.DesktopConnectionHandler;
import net.stegemann.io.serial.base.DesktopProtocol;
import net.stegemann.io.serial.base.TypedConnectionHandler;
import net.stegemann.misc.ChangeListener;

import java.util.ArrayList;

public class SerialConfigurationReader
{
	private final ConfigurationProgress configurationProgress;
	private final DesktopConnection connection;

	private final ArrayList< TypedConnectionHandler> handlerStack = new ArrayList<>();

	SerialConfigurationReader( ConfigurationProgress useConfigurationProgress, DesktopConnection useConnection)
	{
		configurationProgress = useConfigurationProgress;
		connection = useConnection;
    }

	public void readFromPort( Configuration configuration, String portName,
							  ChangeListener< ConfigurationProgress> configurationListener)
		throws ReadException
	{
		TypedConnectionForwarder connectionForwarder = new TypedConnectionForwarder();

		if( connection.open( portName, connectionForwarder) == false)
		{
			throw new ReadException( "Failed to open connection with port " + portName + ".");
		}

		configuration.clear();

		configurationProgress.reset();
		configurationProgress.addChangeListener( configurationListener);

		pushHandler( new StartHandler( configuration));

		try
		{
			connection.readConfiguration();
		}
		catch( ReadException reason)
		{
			throw new ReadException( "Failed to read configuration!", reason);
		}
		finally
		{
			configurationProgress.removeChangeListener( configurationListener);	

			handlerStack.remove( 0);

			connection.close();
		}

//		java.lang.System.err.println( "End, stack size: " + handlerStack.size());
  	}

	private class TypedConnectionForwarder
		implements TypedConnectionHandler
	{
		@Override
		public void complexOpened( byte id)
		{
//			java.lang.System.out.print( ".");

			handlerStack.get( 0).complexOpened( id);
		}

		@Override
		public void complexClosed()
		{
			handlerStack.get( 0).complexClosed();

			handlerStack.remove( 0);

//			java.lang.System.err.println( "Complex closed, stack size: " + handlerStack.size());
		}

		@Override
		public void valueRead( byte id, String value)
		{
			handlerStack.get( 0).valueRead( id, value);
		}
	}

	private class UnknownTypeHandler
		extends DesktopConnectionHandler
	{
		@Override
		public void complexOpened( DesktopProtocol.Id id)
		{
			java.lang.System.out.println( "Unknown complex " + id);

			pushHandler( new UnknownTypeHandler());
		}

		@Override
		public void complexClosed()
		{
		}

		@Override
		public void valueRead( DesktopProtocol.Id id, String textContent)
		{
			java.lang.System.out.println( "Unknown value " + id.byteValue() + " {" + textContent + "}");
 		}

		protected void readValue( Number value, String textContent)
		{
			try
			{
				value.setConfigurationValue( textContent);
			}
			catch( NumberFormatException | ValueOutOfRangeException ignored)
			{
			}
		}

		protected void readValue( Text value, String textContent)
		{
			value.setConfigurationValue( textContent);
		}

		protected void readValue( Bool value, String textContent)
		{
			value.setConfigurationValue( textContent);
		}
	}

	private class StartHandler
		extends UnknownTypeHandler
	{
		final Configuration configuration;

		public StartHandler( Configuration useConfiguration)
		{
			configuration = useConfiguration;
		}

		@Override
		public void complexOpened( DesktopProtocol.Id id)
		{
			switch( id)
			{
				case Configuration :
				{
					pushHandler( new ConfigurationHandler( configuration));
				}
				break;

				default : super.complexOpened( id); break;
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
		public void complexOpened( DesktopProtocol.Id id)
		{
			System UseSystem = configuration.getSystem();

			switch( id)
			{
				case Battery ->
					pushHandler( new BatteryHandler( UseSystem.getBattery()));

				case Calibrations ->
					pushHandler( new CalibrationsHandler( UseSystem.getCalibrations()));

				case Models ->
					pushHandler( new ModelsHandler( configuration.getModels(), configuration));

				case Types ->
					pushHandler( new TypesHandler( configuration.getTypes(), configuration));

				case Sources ->
					pushHandler( new SourcesHandler( configuration.getSources(), configuration));

				case PPMs ->
			 		pushHandler( new PPMsHandler( UseSystem.getPpms()));

				default -> super.complexOpened( id);
			}
 		}

		@Override
		public void complexClosed()
		{
			configuration.fill();
		}

		@Override
		public void valueRead( DesktopProtocol.Id id, String textContent)
		{
			System UseSystem = configuration.getSystem();

			switch( id)
			{
				case Owner :
				{
					readValue( UseSystem.getOwner(), textContent);
				}
				break;

				case AnalogInputs :
				{
					readValue( UseSystem.getAnalogInputs(), textContent);
				}
				break;

				case DigitalInputs :
				{
					readValue( UseSystem.getDigitalInputs(), textContent);
				}
				break;

				case OutputChannels :
				{
					readValue( UseSystem.getOutputChannels(), textContent);
				}
				break;

				case Outputs :
				{
					readValue( UseSystem.getOutputs(), textContent);
				}
				break;

				case SetupBacklight :
				{
					readValue( UseSystem.getSetupBacklight(), textContent);
				}
				break;

				case SetupBlankTime :
				{
					readValue( UseSystem.getSetupBlankTime(), textContent);
				}
				break;

				case StatusBacklight :
				{
					readValue( UseSystem.getStatusBacklight(), textContent);
				}
				break;

				case StatusContrast :
				{
					readValue( UseSystem.getStatusContrast(), textContent);
				}
				break;

				case StatusBlankTime :
				{
					readValue( UseSystem.getStatusBlankTime(), textContent);
				}
				break;

				case StatusInverted :
				{
					readValue( UseSystem.getStatusInverted(), textContent);
				}
				break;

				case SelectedModel :
				{
					readValue( UseSystem.getSelectedModel(), textContent);
				}
				break;

				default : super.valueRead( id, textContent); break;
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
		public void valueRead( DesktopProtocol.Id id, String textContent)
		{			
			switch( id)
			{
				case BatteryWarnLowVoltage :
				{
					readValue( battery.getWarnLowVoltage(), textContent);
				}
				break;

				case BatteryWarnCriticalVoltage :
				{
					readValue( battery.getWarnCriticalVoltage(), textContent);
				}
				break;

				case BatteryMinimumVoltage :
				{
					readValue( battery.getMinimumVoltage(), textContent);
				}
				break;

				case BatteryMaximumVoltage :
				{
					readValue( battery.getMaximumVoltage(), textContent);
				}
				break;

				case BatteryCalibrationVoltage :
				{
					readValue( battery.getCalibrationVoltage(), textContent);
				}
				break;

				default : super.valueRead( id, textContent); break;
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
		public void complexOpened( DesktopProtocol.Id id)
		{
			switch( id)
			{
				case Calibration :
				{
					Calibration NewCalibration = new Calibration();

					pushHandler( new CalibrationHandler( NewCalibration));

					calibrations.setCalibration( calibrationIndex, NewCalibration);

					calibrationIndex++;
				}
				break;

				default : super.complexOpened( id); break;
			}
 		}
	}

	private class CalibrationHandler extends UnknownTypeHandler
	{
		private final Calibration calibration;

		public CalibrationHandler( Calibration useCalibration)
		{
			calibration = useCalibration;
		}

		@Override
		public void valueRead( DesktopProtocol.Id id, String textContent)
		{			
			switch( id)
			{
				case CalibrationHigh :
				{
					readValue( calibration.getHigh(), textContent);
				}
				break;

				case CalibrationCenter :
				{
					readValue( calibration.getCenter(), textContent);
				}
				break;

				case CalibrationLow :
				{
					readValue( calibration.getLow(), textContent);
				}
				break;

				default : super.valueRead( id, textContent); break;
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
		public void complexOpened( DesktopProtocol.Id id)
		{
			switch( id)
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

				default : super.complexOpened( id); break;
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
		public void complexOpened( DesktopProtocol.Id id)
		{
			switch( id)
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

				default : super.complexOpened( id); break;
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
		public void valueRead( DesktopProtocol.Id id, String textContent)
		{			
			switch( id)
			{
				case ModelName :
				{
					readValue( model.getName(), textContent);
				}
				break;

				case ModelState :
				{
					model.setState( Utility.convertModelState( textContent));
				}
				break;

				case ModelType :
				{
					readValue( model.getTypeId(), textContent);
				}
				break;

				case ModelRFMode :
				{
					readValue( model.getRfMode(), textContent);
				}
				break;

				default : super.valueRead( id, textContent); break;
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
		public void valueRead( DesktopProtocol.Id id, String textContent)
		{
			switch( id)
			{
				case StatusSource :
				{
					readValue( model.getStatusSourceId( StatusSource.values()[ statusSourceIndex]),
						textContent);

					statusSourceIndex++;
				}
				break;

				default : super.valueRead( id, textContent); break;
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
		public void valueRead( DesktopProtocol.Id id, String textContent)
		{
			switch( id)
			{
				case StatusTimer :
				{
					readValue( model.getStatusTimeId( StatusTime.values()[ statusTimeIndex]),
						textContent);

					statusTimeIndex++;
				}
				break;

				default : super.valueRead( id, textContent); break;
			}
 		}
	}

	private class ChannelsHandler extends UnknownTypeHandler
	{
		private final Channels channels;
		private int channelIndex;

		public ChannelsHandler( Channels useChannels)
		{
			channels = useChannels;
			channelIndex = 0;
		}

		@Override
		public void complexOpened( DesktopProtocol.Id id)
		{
			switch( id)
			{
				case Channel ->
				{
					Channel channel = channels.addChannel( channelIndex++);

					pushHandler( new ChannelHandler( channel));
				}

				default -> super.complexOpened( id);
			}
 		}
	}

	private class ChannelHandler extends UnknownTypeHandler
	{
		private final Channel channel;

		public ChannelHandler( Channel UseChannel)
		{
			channel = UseChannel;
		}

		@Override
		public void complexOpened( DesktopProtocol.Id id)
		{
			switch( id)
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

				default : super.complexOpened( id); break;
			}
 		}

		@Override
		public void valueRead( DesktopProtocol.Id id, String textContent)
		{			
			switch( id)
			{
				case ChannelName :
				{
					readValue( channel.getName(), textContent);
				}
				break;

				case ChannelReverse :
				{
					readValue( channel.getReverse(), textContent);
				}
				break;

				case ChannelMode :
				{
					readValue( channel.getMode(), textContent);
				}
				break;

				default : super.valueRead( id, textContent); break;
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
		public void valueRead( DesktopProtocol.Id id, String textContent)
		{			
			switch( id)
			{
				case ChannelPoint :
				{
					readValue( channel.getPoint( channelPointIndex++), textContent);
				}
				break;

				default : super.valueRead( id, textContent); break;
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
		public void complexOpened( DesktopProtocol.Id id)
		{
			switch( id)
			{
				case ModelProxyReference :
				{
					pushHandler( new SourceTupelHandler( proxyReferences.GetProxyReferenceFromIndex(
						proxyIndex++)));
				}
				break;

				default : super.complexOpened( id); break;
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
		public void complexOpened( DesktopProtocol.Id id)
		{
			switch( id)
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

				default : super.complexOpened( id); break;
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
		public void valueRead( DesktopProtocol.Id id, String textContent)
		{			
			switch( id)
			{
				case TypeName :
				{
					readValue( type.getName(), textContent);
				}
				break;

				case TypeState :
				{
					type.setState( Utility.convertTypeState( textContent));
				}
				break;

				default : super.valueRead( id, textContent); break;
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
		public void complexOpened( DesktopProtocol.Id id)
		{
			switch( id)
			{
				case Source :
				{
					pushHandler( new SourceHandler( sources, sourceId++, configuration));
				}
				break;

				default : super.complexOpened( id); break;
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
		public void complexOpened( DesktopProtocol.Id id)
		{
			switch( id)
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
					Trimmer NewSource = new Trimmer();
					newSource = NewSource;

					pushHandler( new SourceTrimmerHandler( NewSource));
				}
				break;

				default : super.complexOpened( id); break;
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
		public void valueRead( DesktopProtocol.Id id, String textContent)
		{			
			switch( id)
			{
				case SourceName :
				{
					name = textContent;
				}
				break;

				case SourceModel :
				{
					readValue( modelId, textContent);
				}
				break;

				default : super.valueRead( id, textContent); break;
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
		public void valueRead( DesktopProtocol.Id id, String textContent)
		{			
			switch( id)
			{
				case SourceInputAnalogInput :
				{
					readValue( source.getInputId(), textContent);
				}
				break;

				default : super.valueRead( id, textContent); break;
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
		public void valueRead( DesktopProtocol.Id id, String textContent)
		{			
			switch( id)
			{
				case SourceInputButtonInput :
				{
					readValue( source.getInputId(), textContent);
				}
				break;

				case SourceInputButtonInit :
				{
					readValue( source.getInit(), textContent);
				}
				break;

				case SourceInputButtonStore :
				{
					readValue( source.getStore(), textContent);
				}
				break;

				case SourceInputButtonToggle :
				{
					readValue( source.getToggle(), textContent);
				}
				break;

				case SourceInputButtonTop :
				{
					readValue( source.getTop(), textContent);
				}
				break;

				case SourceInputButtonBottom :
				{
					readValue( source.getBottom(), textContent);
				}
				break;

				default : super.valueRead( id, textContent); break;
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
		public void valueRead( DesktopProtocol.Id id, String textContent)
		{			
			switch( id)
			{
				case SourceInputRotaryAInput :
				{
					readValue( source.getAInputId(), textContent);
				}
				break;

				case SourceInputRotaryBInput :
				{
					readValue( source.getBInputId(), textContent);
				}
				break;

				case SourceInputRotaryStore :
				{
					readValue( source.getStore(), textContent);
				}
				break;

				case SourceInputRotaryInit :
				{
					readValue( source.getInit(), textContent);
				}
				break;

				case SourceInputRotaryStep :
				{
					readValue( source.getStep(), textContent);
				}
				break;

				case SourceInputRotaryTop :
				{
					readValue( source.getTop(), textContent);
				}
				break;

				case SourceInputRotaryBottom :
				{
					readValue( source.getBottom(), textContent);
				}
				break;

				default : super.valueRead( id, textContent); break;
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
		public void valueRead( DesktopProtocol.Id id, String textContent)
		{			
			switch( id)
			{
				case SourceInputSwitchLowInput :
				{
					readValue( source.getLowInputId(), textContent);
				}
				break;

				case SourceInputSwitchHighInput :
				{
					readValue( source.getHighInputId(), textContent);
				}
				break;

				case SourceInputSwitchTop :
				{
					readValue( source.getTop(), textContent);
				}
				break;

				case SourceInputSwitchBottom :
				{
					readValue( source.getBottom(), textContent);
				}
				break;

				default : super.valueRead( id, textContent); break;
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
		public void valueRead( DesktopProtocol.Id id, String textContent)
		{			
			switch( id)
			{
				case SourceInputTickerLowInput :
				{
					readValue( source.getLowInputId(), textContent);
				}
				break;

				case SourceInputTickerHighInput :
				{
					readValue( source.getHighInputId(), textContent);
				}
				break;

				case SourceInputTickerInit :
				{
					readValue( source.getInit(), textContent);
				}
				break;

				case SourceInputTickerStep :
				{
					readValue( source.getStep(), textContent);
				}
				break;

				case SourceInputTickerStore :
				{
					readValue( source.getStore(), textContent);
				}
				break;

				case SourceInputTickerTop :
				{
					readValue( source.getTop(), textContent);
				}
				break;

				case SourceInputTickerBottom :
				{
					readValue( source.getBottom(), textContent);
				}
				break;

				default : super.valueRead( id, textContent); break;
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
		public void complexOpened( DesktopProtocol.Id id)
		{
			switch( id)
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

				default : super.complexOpened( id); break;
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
		public void complexOpened( DesktopProtocol.Id id)
		{
			switch( id)
			{
				case SourceMapPoint :
				{
					pushHandler( new SourceTupelHandler( source.getPoint( pointIndex++)));
				}
				break;

				default : super.complexOpened( id); break;
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
		public void complexOpened( DesktopProtocol.Id id)
		{
			switch( id)
			{
				case SourceMixInputs :
				{
					pushHandler( new SourceMixInputsHandler( source));
				}
				break;

				default : super.complexOpened( id); break;
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
		public void complexOpened( DesktopProtocol.Id id)
		{
			switch( id)
			{
				case SourceMixInput ->
				{
					pushHandler( new SourceTupelHandler( source.getInput( inputIndex++)));
				}

				default -> super.complexOpened( id);
			}
 		}
	}

	private class SourceFollowerHandler extends UnknownTypeHandler
	{
		private final Follower source;

		public SourceFollowerHandler( Follower useSource)
		{
			source = useSource;
		}

		@Override
		public void complexOpened( DesktopProtocol.Id id)
		{
			switch( id)
			{
				case SourceFollowerTarget ->
				{
					pushHandler( new SourceTupelHandler( source.getTarget()));
				}

				case SourceFollowerStep ->
				{
					pushHandler( new SourceTupelHandler( source.getStep()));
				}

				default -> super.complexOpened( id);
			}
 		}

		@Override
		public void valueRead( DesktopProtocol.Id id, String textContent)
		{			
			switch( id)
			{
				case SourceFollowerTrigger ->
				{
					readValue( source.getTrigger(), textContent);
				}

				case SourceFollowerTriggerLowLimit ->
			 	{
					readValue( source.getTriggerLowLimit(), textContent);
				}

				case SourceFollowerTriggerHighLimit ->
				{
					readValue( source.getTriggerHighLimit(), textContent);
				}

				default -> super.valueRead( id, textContent);
			}
 		}
	}

	private class SourceStoreHandler
		extends UnknownTypeHandler
	{
		private final Store source;

		public SourceStoreHandler( Store useSource)
		{
			source = useSource;
		}

		@Override
		public void valueRead( DesktopProtocol.Id id, String textContent)
		{			
			switch( id)
			{
				case SourceStoreInput ->
				{
					readValue( source.getInput(), textContent);
				}

				case SourceStoreInit ->
				{
					readValue( source.getInit(), textContent);
				}

				default -> super.valueRead( id, textContent);
			}
 		}
	}

	private class SourceTimerHandler
		extends UnknownTypeHandler
	{
		private final Timer source;

		public SourceTimerHandler( Timer useSource)
		{
			source = useSource;
		}

		@Override
		public void valueRead( DesktopProtocol.Id id, String textContent)
		{			
			switch( id)
			{
				case SourceTimerInitTime ->
				{
					readValue( source.getInitTime(), textContent);
				}

				case SourceTimerCurrentTime ->
			 	{
					readValue( source.getCurrentTime(), textContent);
				}

				case SourceTimerStore ->
				{
					readValue( source.getStore(), textContent);
				}

				case SourceTimerReverse ->
				{
					readValue( source.getReverse(), textContent);
				}

				case SourceTimerTrigger ->
				{
					readValue( source.getTrigger(), textContent);
				}

				case SourceTimerTriggerHighLimit ->
				{
					readValue( source.getTriggerHighLimit(), textContent);
				}

				case SourceTimerTriggerLowLimit ->
				{
					readValue( source.getTriggerLowLimit(), textContent);
				}

				case SourceTimerWarnLowTime ->
				{
					readValue( source.getWarnLowTime(), textContent);
				}

				case SourceTimerWarnCriticalTime ->
				{
					readValue( source.getWarnCriticalTime(), textContent);
				}

				case SourceTimerWarnPauseTime ->
				{
					readValue( source.getWarnPauseTime(), textContent);
				}

				default -> super.valueRead( id, textContent);
			}
 		}
	}

	private class SourceTrimmerHandler
		extends UnknownTypeHandler
	{
		private final Trimmer trimmer;

		public SourceTrimmerHandler( Trimmer useTrimmer)
		{
			trimmer = useTrimmer;
		}

		@Override
		public void complexOpened( DesktopProtocol.Id id)
		{
			switch( id)
			{
				case SourceTrimmerInput ->
				{
					pushHandler( new SourceTupelHandler( trimmer.getInput()));
				}

				case SourceTrimmerTrim ->
				{
					pushHandler( new SourceTupelHandler( trimmer.getTrim()));
				}

				case SourceTrimmerLimit ->
				{
					pushHandler( new SourceTupelHandler( trimmer.getLimit()));
				}

				case SourceTrimmerPoints ->
				{
					pushHandler( new SourceTrimmerPointsHandler( trimmer));
				}

				default -> super.complexOpened( id);
			}
 		}

		@Override
		public void valueRead( DesktopProtocol.Id id, String textContent)
		{			
			switch( id)
			{
				case SourceTrimmerReverse :
				{
					readValue( trimmer.getReverse(), textContent);
				}
				break;

				default : super.valueRead( id, textContent); break;
			}
 		}
	}

	private class SourceTrimmerPointsHandler extends UnknownTypeHandler
	{
		private Trimmer trimmer;
		private int pointIndex;

		public SourceTrimmerPointsHandler( Trimmer UseTrimmer)
		{
			trimmer = UseTrimmer;
			pointIndex = 0;
		}

		@Override
		public void valueRead( DesktopProtocol.Id id, String textContent)
		{			
			switch( id)
			{
				case SourceTrimmerPoint :
				{
					readValue( trimmer.getPoint( pointIndex++), textContent);
				}
				break;

				default : super.valueRead( id, textContent); break;
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
		public void valueRead( DesktopProtocol.Id id, String textContent)
		{			
			switch( id)
			{
				case SourceProxySlot :
				{
					readValue( source.getSlot(), textContent);
				}
				break;

				default : super.valueRead( id, textContent); break;
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
		public void complexOpened( DesktopProtocol.Id id)
		{
			switch( id)
			{
				case PPM :
				{
					pushHandler( new PPMHandler( ppms.getPPM( ppmIndex++)));
				}
				break;

				default : super.complexOpened( id); break;
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
		public void complexOpened( DesktopProtocol.Id id)
		{
			switch( id)
			{
				case ChannelMappings :
				{
					pushHandler( new ChannelMappingsHandler( ppm.getChannelMappings()));
				}
				break;

				default : super.complexOpened( id); break;
			}
 		}

		@Override
		public void valueRead( DesktopProtocol.Id id, String textContent)
		{			
			switch( id)
			{
				case PPMInverted :
				{
					readValue( ppm.getInverted(), textContent);
				}
				break;

				case PPMCenter :
				{
					readValue( ppm.getCenter(), textContent);
				}
				break;

				case PPMName :
				{
					readValue( ppm.getName(), textContent);
				}
				break;

				default : super.valueRead( id, textContent); break;
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
		public void valueRead( DesktopProtocol.Id id, String textContent)
		{			
			switch( id)
			{
				case ChannelMapping :
				{
					readValue( channelMappings.getChannelMapping( channelIndex++), textContent);
				}
				break;

				default : super.valueRead( id, textContent); break;
			}
 		}
	}

	private class SourceTupelHandler extends UnknownTypeHandler
	{
		private final SourceWithVolume sourceTupel;

		public SourceTupelHandler( SourceWithVolume useSourceTupel)
		{
			sourceTupel = useSourceTupel;
		}

		@Override
		public void valueRead( DesktopProtocol.Id id, String textContent)
		{
			switch( id)
			{
				case SourceTupelSource :
				{
					readValue( sourceTupel.getSourceId(), textContent);
				}
				break;

				case SourceTupelVolume :
				{
					readValue( sourceTupel.getVolume(), textContent);
				}
				break;

				default : super.valueRead( id, textContent); break;
			}
 		}
	}

	@SuppressWarnings("UnusedReturnValue")
 	private TypedConnectionHandler pushHandler( TypedConnectionHandler connectionHandler)
	{
		handlerStack.add( 0, connectionHandler);
		
		return connectionHandler;
	}
}
