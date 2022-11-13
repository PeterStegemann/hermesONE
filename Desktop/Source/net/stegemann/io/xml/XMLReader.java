package net.stegemann.io.xml;

import java.io.IOException;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

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
import net.stegemann.configuration.type.ValueOutOfRangeException;
import net.stegemann.io.ReadException;
import net.stegemann.io.Utility;

import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

public class XMLReader
{
	public enum Mode
	{
		All,
		System,
		Models
	}

	public void readFromFile( Configuration UseConfiguration, String FileName, Mode UseMode)
		throws ReadException
	{
		DocumentBuilderFactory Factory = DocumentBuilderFactory.newInstance();
		DocumentBuilder Builder;

		try
		{
			Builder = Factory.newDocumentBuilder();
		}
		catch( ParserConfigurationException Reason)
		{
			throw new ReadException( "Failed to create xml document builder.", Reason);
		}

		Document ConfigurationDocument;

		try
		{
			ConfigurationDocument = Builder.parse( FileName);
		}
		catch( SAXException Reason)
		{
			throw new ReadException( "Failed to parse xml document '" + FileName + "'.", Reason);
		}
		catch( IOException Reason)
		{
			throw new ReadException( "Failed to read xml document '" + FileName + "'.", Reason);
		}

		if( UseMode == Mode.All)
		{
			UseConfiguration.clear();
		}
		else if( UseMode == Mode.Models)
		{
			UseConfiguration.clearModels();
		}
		else if( UseMode == Mode.System)
		{
			UseConfiguration.clearSystem();
		}

		interpretDocument( UseConfiguration, ConfigurationDocument, UseMode);
	}

	private void interpretDocument( Configuration UseConfiguration, Document ConfigurationDocument,
			Mode UseMode)
		throws ReadException
	{
		NodeList ChildNodes = ConfigurationDocument.getChildNodes();

		Node ConfigurationNode = ChildNodes.item( 0);

		if( ConfigurationNode == null)
		{
			throw new ReadException( "No configuration node found, invalid configuration.");
		}

		importConfiguration( UseConfiguration, getChildNodes( ConfigurationNode), UseMode);
	}

	private void importConfiguration( Configuration UseConfiguration, IterableNodeList ChildNodes,
		Mode UseMode)
	{
		System UseSystem = UseConfiguration.getSystem();

		for( Node ChildNode: ChildNodes)
		{
			String NodeName = ChildNode.getNodeName();
			String TextContent = ChildNode.getTextContent();

			boolean SystemResult = true;

			if(( UseMode == Mode.All) || ( UseMode == Mode.System))
			{
				SystemResult = importSystemNode( UseSystem, NodeName, TextContent, ChildNode);
			}

			boolean ModelResult = true;

			if(( UseMode == Mode.All) || ( UseMode == Mode.Models))
			{
				ModelResult = importModelNode( UseConfiguration, NodeName, ChildNode);
			}

			if(( SystemResult == false) && ( ModelResult == false))
			{
				java.lang.System.out.println( NodeName + " " + TextContent);
			}
		}

		UseConfiguration.fill();
	}

	private boolean importModelNode( Configuration UseConfiguration, String NodeName, Node ChildNode)
	{
		if( Names.MODELS.compareToIgnoreCase( NodeName) == 0)
		{
			importModels( UseConfiguration.getModels(), getChildNodes( ChildNode));
		}
		else if( Names.TYPES.compareToIgnoreCase( NodeName) == 0)
		{
			importTypes( UseConfiguration.getTypes(), getChildNodes( ChildNode));
		}
		else if( Names.SOURCES.compareToIgnoreCase( NodeName) == 0)
		{
			importSources( UseConfiguration.getSources(), getChildNodes( ChildNode));
		}
		else
		{
			return false;
		}

		return true;
	}

	private boolean importSystemNode( System UseSystem, String NodeName, String TextContent,
		Node ChildNode)
	{
		if( Names.SYSTEM_ANALOG_INPUTS.compareToIgnoreCase( NodeName) == 0)
		{
			readValue( UseSystem.getAnalogInputs(), TextContent);
		}
		else if( Names.SYSTEM_DIGITAL_INPUTS.compareToIgnoreCase( NodeName) == 0)
		{
			readValue( UseSystem.getDigitalInputs(), TextContent);
		}
		else if( Names.SYSTEM_OUTPUT_CHANNELS.compareToIgnoreCase( NodeName) == 0)
		{
			readValue( UseSystem.getOutputChannels(), TextContent);
		}
		else if( Names.SYSTEM_OUTPUTS.compareToIgnoreCase( NodeName) == 0)
		{
			readValue( UseSystem.getOutputs(), TextContent);
		}
		else if( Names.SYSTEM_OWNER.compareToIgnoreCase( NodeName) == 0)
		{
			readValue( UseSystem.getOwner(), TextContent);
		}
		else if( Names.SYSTEM_SETUP_BACKLIGHT.compareToIgnoreCase( NodeName) == 0)
		{
			readValue( UseSystem.getSetupBacklight(), TextContent);
		}
		else if( Names.SYSTEM_SETUP_BLANK_TIME.compareToIgnoreCase( NodeName) == 0)
		{
			readValue( UseSystem.getSetupBlankTime(), TextContent);
		}
		else if( Names.SYSTEM_STATUS_BACKLIGHT.compareToIgnoreCase( NodeName) == 0)
		{
			readValue( UseSystem.getStatusBacklight(), TextContent);
		}
		else if( Names.SYSTEM_STATUS_CONTRAST.compareToIgnoreCase( NodeName) == 0)
		{
			readValue( UseSystem.getStatusContrast(), TextContent);
		}
		else if( Names.SYSTEM_STATUS_BLANK_TIME.compareToIgnoreCase( NodeName) == 0)
		{
			readValue( UseSystem.getStatusBlankTime(), TextContent);
		}
		else if( Names.SYSTEM_STATUS_INVERTED.compareToIgnoreCase( NodeName) == 0)
		{
			readValue( UseSystem.getStatusInverted(), TextContent);
		}
		else if( Names.SYSTEM_SELECTED_MODEL.compareToIgnoreCase( NodeName) == 0)
		{
			readValue( UseSystem.getSelectedModel(), TextContent);
		}
		else if( Names.BATTERY.compareToIgnoreCase( NodeName) == 0)
		{
			importBattery( UseSystem.getBattery(), getChildNodes( ChildNode));
		}
		else if( Names.CALIBRATIONS.compareToIgnoreCase( NodeName) == 0)
		{
			importCalibrations( UseSystem.getCalibrations(), getChildNodes( ChildNode));
		}
		else if( Names.PPMS.compareToIgnoreCase( NodeName) == 0)
		{
			importPPMs( UseSystem.getPPMs(), getChildNodes( ChildNode));
		}		
		else
		{
			return false;
		}

		return true;
	}

	private void importPPMs( PPMs UsePPMs, IterableNodeList ChildNodes)
	{
		int PPMId = 0;

		for( Node ChildNode: ChildNodes)
		{
			String NodeName = ChildNode.getNodeName();

			if( Names.PPM.compareToIgnoreCase( NodeName) == 0)
			{
				importPPM( UsePPMs.getPPM( PPMId), getChildNodes( ChildNode));

				PPMId++;
			}
			else
			{
				java.lang.System.out.println( NodeName);
			}
		}
	}

	private void importPPM( PPM UsePPM, IterableNodeList ChildNodes)
	{
		for( Node ChildNode: ChildNodes)
		{
			String NodeName = ChildNode.getNodeName();
			String TextContent = ChildNode.getTextContent();

			if( Names.PPM_INVERTED.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UsePPM.getPPMInverted(), TextContent);
			}
			else if( Names.PPM_CENTER.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UsePPM.getPPMCenter(), TextContent);
			}
			else if( Names.PPM_NAME.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UsePPM.getName(), TextContent);
			}
			else if( Names.PPM_CHANNEL_MAPPINGS.compareToIgnoreCase( NodeName) == 0)
			{
				importChannelMappings( UsePPM.getChannelMappings(), getChildNodes( ChildNode));
			}
			else
			{
				java.lang.System.out.println( NodeName);
			}
		}
	}

	private void importChannelMappings( ChannelMappings UseChannelMappings, IterableNodeList ChildNodes)
	{
		int ChannelIndex = 0;

		for( Node ChildNode: ChildNodes)
		{
			String NodeName = ChildNode.getNodeName();
			String TextContent = ChildNode.getTextContent();

			if( Names.PPM_CHANNEL_MAPPING.compareToIgnoreCase( NodeName) == 0)
			{
				Number NewChannelMapping = new Number( 0, 16);

				readValue( NewChannelMapping, TextContent);

				UseChannelMappings.setChannelMapping( ChannelIndex, NewChannelMapping);

				ChannelIndex++;
			}
			else
			{
				java.lang.System.out.println( NodeName);
			}
		}
	}


	private void importBattery( Battery UseBattery, IterableNodeList ChildNodes)
	{
		for( Node ChildNode: ChildNodes)
		{
			String NodeName = ChildNode.getNodeName();
			String TextContent = ChildNode.getTextContent();

			if( Names.BATTERY_WARN_LOW_VOLTAGE.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseBattery.getWarnLowVoltage(), TextContent);
			}
			else if( Names.BATTERY_WARN_CRITICAL_VOLTAGE.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseBattery.getWarnCriticalVoltage(), TextContent);
			}
			else if( Names.BATTERY_MINIMUM_VOLTAGE.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseBattery.getMinimumVoltage(), TextContent);
			}
			else if( Names.BATTERY_MAXIMUM_VOLTAGE.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseBattery.getMaximumVoltage(), TextContent);
			}
			else if( Names.BATTERY_CALIBRATION_VOLTAGE.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseBattery.getCalibrationVoltage(), TextContent);
			}
			else
			{
				java.lang.System.out.println( NodeName + " " + TextContent);
			}
		}
	}

	private void importCalibrations( Calibrations UseCalibrations, IterableNodeList ChildNodes)
	{
		int CalibrationIndex = 0;

		for( Node ChildNode: ChildNodes)
		{
			String NodeName = ChildNode.getNodeName();

			if( Names.CALIBRATION.compareToIgnoreCase( NodeName) == 0)
			{
				Calibration NewCalibration = new Calibration();

				importCalibration( NewCalibration, getChildNodes( ChildNode));

				UseCalibrations.setCalibration( CalibrationIndex, NewCalibration);

				CalibrationIndex++;
			}
			else
			{
				java.lang.System.out.println( NodeName);
			}
		}
	}

	private void importCalibration( Calibration UseCalibration, IterableNodeList ChildNodes)
	{
		for( Node ChildNode: ChildNodes)
		{
			String NodeName = ChildNode.getNodeName();
			String TextContent = ChildNode.getTextContent();

			if( Names.CALIBRATION_HIGH.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseCalibration.getHigh(), TextContent);
			}
			else if( Names.CALIBRATION_CENTER.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseCalibration.getCenter(), TextContent);
			}
			else if( Names.CALIBRATION_LOW.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseCalibration.getLow(), TextContent);
			}
			else
			{
				java.lang.System.out.println( NodeName + " " + TextContent);
			}
		}
	}

	private void importModels( Models UseModels, IterableNodeList ChildNodes)
	{
		int ModelId = 0;

		for( Node ChildNode: ChildNodes)
		{
			String NodeName = ChildNode.getNodeName();

			if( Names.MODEL.compareToIgnoreCase( NodeName) == 0)
			{
				Model NewModel = new Model();

				importModel( NewModel, getChildNodes( ChildNode));

				// Don't add empty models to the array.
				if( NewModel.getState() == Model.State.USED)
				{
					try
					{
						NewModel.getId().setValue( ModelId);

						UseModels.addModel( NewModel);
					}
					catch( Exception Reason)
					{
						java.lang.System.err.println( "Error inserting model: " + Reason.getMessage());
					}
				}

				// Always count up model id, it doesn't matter if a model was skipped.
				ModelId++;
			}
			else
			{
				java.lang.System.out.println( NodeName);
			}
		}
	}

	private void importModel( Model UseModel, IterableNodeList ChildNodes)
	{
		for( Node ChildNode: ChildNodes)
		{
			String NodeName = ChildNode.getNodeName();
			String TextContent = ChildNode.getTextContent();

			if( Names.MODEL_NAME.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseModel.getName(), TextContent);
			}
			else if( Names.MODEL_STATE.compareToIgnoreCase( NodeName) == 0)
			{
				UseModel.setState( Utility.convertModelState( TextContent));
			}
			else if( Names.MODEL_RF_MODE.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseModel.getRFMode(), TextContent);
			}
			else if( Names.MODEL_TYPE.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseModel.getTypeId(), TextContent);
			}
			else if( Names.MODEL_STATUS_SOURCES.compareToIgnoreCase( NodeName) == 0)
			{
				importStatusSources( UseModel, getChildNodes( ChildNode));
			}
			else if( Names.MODEL_STATUS_TIMES.compareToIgnoreCase( NodeName) == 0)
			{
				importStatusTimes( UseModel, getChildNodes( ChildNode));
			}
			else if( Names.CHANNELS.compareToIgnoreCase( NodeName) == 0)
			{
				importChannels( UseModel.getChannels(), getChildNodes( ChildNode));
			}
			else if( Names.MODEL_PROXY_REFERENCES.compareToIgnoreCase( NodeName) == 0)
			{
				importProxySources( UseModel.getProxyReferences(), getChildNodes( ChildNode));
			}
			else
			{
				java.lang.System.out.println( NodeName + " " + TextContent);
			}
		}
	}

	private void importStatusSources( Model UseModel, IterableNodeList ChildNodes)
	{
		int StatusSourceIndex = 0;

		for( Node ChildNode: ChildNodes)
		{
			String TextContent = ChildNode.getTextContent();
			String NodeName = ChildNode.getNodeName();

			if( Names.MODEL_STATUS_SOURCE.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseModel.getStatusSourceId( StatusSource.values()[ StatusSourceIndex]),
						TextContent);

				StatusSourceIndex++;
			}
			else
			{
				java.lang.System.out.println( NodeName + " " + TextContent);
			}
		}		
	}

	private void importStatusTimes( Model UseModel, IterableNodeList ChildNodes)
	{
		int StatusTimeIndex = 0;

		for( Node ChildNode: ChildNodes)
		{
			String TextContent = ChildNode.getTextContent();

			String NodeName = ChildNode.getNodeName();

			if( Names.MODEL_STATUS_TIME.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseModel.getStatusTimeId( StatusTime.values()[ StatusTimeIndex]),
						TextContent);

				StatusTimeIndex++;
			}
			else
			{
				java.lang.System.out.println( NodeName + " " + TextContent);
			}
		}		
	}

	private void importChannels( Channels UseChannels, IterableNodeList ChildNodes)
	{
		int ChannelIndex = 0;

		for( Node ChildNode: ChildNodes)
		{
			String NodeName = ChildNode.getNodeName();

			if( Names.CHANNEL.compareToIgnoreCase( NodeName) == 0)
			{
				Channel NewChannel = new Channel();

				importChannel( NewChannel, getChildNodes( ChildNode));

				UseChannels.setChannel( ChannelIndex, NewChannel);

				ChannelIndex++;
			}
			else
			{
				java.lang.System.out.println( NodeName);
			}
		}
	}		

	private void importChannel( Channel UseChannel, IterableNodeList ChildNodes)
	{
		for( Node ChildNode: ChildNodes)
		{
			String NodeName = ChildNode.getNodeName();
			String TextContent = ChildNode.getTextContent();

			if( Names.CHANNEL_NAME.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseChannel.getName(), TextContent);
			}
			else if( Names.CHANNEL_INPUT.compareToIgnoreCase( NodeName) == 0)
			{
				importSourceTupel( UseChannel.getInput(), getChildNodes( ChildNode));
			}
			else if( Names.CHANNEL_TRIM.compareToIgnoreCase( NodeName) == 0)
			{
				importSourceTupel( UseChannel.getTrim(), getChildNodes( ChildNode));
			}
			else if( Names.CHANNEL_LIMIT.compareToIgnoreCase( NodeName) == 0)
			{
				importSourceTupel( UseChannel.getLimit(), getChildNodes( ChildNode));
			}
			else if( Names.CHANNEL_REVERSE.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseChannel.getReverse(), TextContent);
			}
			else if( Names.CHANNEL_MODE.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseChannel.getMode(), TextContent);
			}
			else if( Names.CHANNEL_POINTS.compareToIgnoreCase( NodeName) == 0)
			{
				importChannelPoints( UseChannel, getChildNodes( ChildNode));
			}
			else
			{
				java.lang.System.out.println( NodeName + " " + TextContent);
			}
		}
	}

	private void importChannelPoints( Channel UseChannel, IterableNodeList ChildNodes)
	{
		int ChannelPointIndex = 0;

		for( Node ChildNode: ChildNodes)
		{
			String NodeName = ChildNode.getNodeName();
			String TextContent = ChildNode.getTextContent();

			if( Names.CHANNEL_POINT.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseChannel.getPoint( ChannelPointIndex), TextContent);

				ChannelPointIndex++;
			}
			else
			{
				java.lang.System.out.println( NodeName + " " + TextContent);
			}
		}		
	}

	private void importProxySources( ProxyReferences UseProxySources, IterableNodeList ChildNodes)
	{
		int ProxyReferenceIndex = 0;

		for( Node ChildNode: ChildNodes)
		{
			String NodeName = ChildNode.getNodeName();
			String TextContent = ChildNode.getTextContent();

			if( Names.MODEL_PROXY_REFERENCE.compareToIgnoreCase( NodeName) == 0)
			{
				importSourceTupel( UseProxySources.GetProxyReferenceFromIndex( ProxyReferenceIndex),
						getChildNodes( ChildNode));

				ProxyReferenceIndex++;
			}
			else
			{
				java.lang.System.out.println( NodeName + " " + TextContent);
			}
		}		
	}

	private void importSourceTupel( SourceWithVolume UseSourceTupel, IterableNodeList ChildNodes)
	{
		for( Node ChildNode: ChildNodes)
		{
			String NodeName = ChildNode.getNodeName();
			String TextContent = ChildNode.getTextContent();

			if( Names.SOURCE_TUPEL_SOURCE_ID.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseSourceTupel.getSourceId(), TextContent);
			}
			else if( Names.SOURCE_TUPEL_VOLUME.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseSourceTupel.getVolume(), TextContent);
			}
			else
			{
				java.lang.System.out.println( NodeName + " " + TextContent);
			}
		}
	}

	private void importTypes( Types UseTypes, IterableNodeList ChildNodes)
	{
		// TypeIds don't start with zero!
		int TypeId = Model.TYPE_START;

		for( Node ChildNode: ChildNodes)
		{
			String NodeName = ChildNode.getNodeName();

			if( Names.TYPE.compareToIgnoreCase( NodeName) == 0)
			{
				Type NewType = new Type();

				importType( NewType, getChildNodes( ChildNode));

				if( NewType.getState() == Type.State.USED)
				{
					try
					{
						NewType.getId().setValue( TypeId);

						UseTypes.addType( NewType);
					}
					catch( Exception Reason)
					{
					}
				}

				// Always count up type id, it doesn't matter if a type was skipped.
				TypeId++;
			}
			else
			{
				java.lang.System.out.println( NodeName);
			}
		}
	}

	private void importType( Type UseType, IterableNodeList ChildNodes)
	{
		for( Node ChildNode: ChildNodes)
		{
			String NodeName = ChildNode.getNodeName();
			String TextContent = ChildNode.getTextContent();

			if( Names.TYPE_NAME.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseType.getName(), TextContent);
			}
			else if( Names.TYPE_STATE.compareToIgnoreCase( NodeName) == 0)
			{
				UseType.setState( Utility.convertTypeState( TextContent));
			}
			else
			{
				java.lang.System.out.println( NodeName + " " + TextContent);
			}
		}
	}

	private void importSources( Sources UseSources, IterableNodeList ChildNodes)
	{
		int SourceId = 0;

		for( Node ChildNode: ChildNodes)
		{
			String NodeName = ChildNode.getNodeName();

			if( Names.SOURCE.compareToIgnoreCase( NodeName) == 0)
			{
				Source NewSource = importSource( getChildNodes( ChildNode));

				if(( NewSource != null) /*&& ( NewSource.GetType() != Source.Type.EMPTY)*/)
				{
					try
					{
						NewSource.getId().setValue( SourceId);

						UseSources.addSource( NewSource);
					}
					catch( Exception Reason)
					{
						java.lang.System.err.println( "Error reading type id for type: " +
								Reason.getMessage());
					}
				}

				// Always count up source id, it doesn't matter if a source was skipped.
				SourceId++;
			}
			else
			{
				java.lang.System.out.println( NodeName);
			}
		}
	}

	private Source importSource( IterableNodeList ChildNodes)
	{
		Source UseSource = null;

		String Name = null;
		Number ModelId = new Number( Model.MODEL_START, Model.MODEL_NONE);

		for( Node ChildNode: ChildNodes)
		{
			String NodeName = ChildNode.getNodeName();
			String TextContent = ChildNode.getTextContent();

			if( Names.SOURCE_NAME.compareToIgnoreCase( NodeName) == 0)
			{
				Name = TextContent;
			}
			else if( Names.SOURCE_MODEL.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( ModelId, TextContent);
			}
			else if( Names.SOURCE_INPUT_ANALOG.compareToIgnoreCase( NodeName) == 0)
			{
				UseSource = importSourceInputAnalog( new Analog(), getChildNodes( ChildNode));
			}
			else if( Names.SOURCE_INPUT_BUTTON.compareToIgnoreCase( NodeName) == 0)
			{
				UseSource = importSourceInputButton( new Button(), getChildNodes( ChildNode));
			}
			else if( Names.SOURCE_INPUT_ROTARY.compareToIgnoreCase( NodeName) == 0)
			{
				UseSource = importSourceInputRotary( new Rotary(), getChildNodes( ChildNode));
			}
			else if( Names.SOURCE_INPUT_SWITCH.compareToIgnoreCase( NodeName) == 0)
			{
				UseSource = importSourceInputSwitch( new Switch(), getChildNodes( ChildNode));
			}
			else if( Names.SOURCE_INPUT_TICKER.compareToIgnoreCase( NodeName) == 0)
			{
				UseSource = importSourceInputTicker( new Ticker(), getChildNodes( ChildNode));
			}
			else if( Names.SOURCE_MAP.compareToIgnoreCase( NodeName) == 0)
			{
				UseSource = importSourceMap( new Map(), getChildNodes( ChildNode));
			}
			else if( Names.SOURCE_MIX.compareToIgnoreCase( NodeName) == 0)
			{
				UseSource = importSourceMix( new Mix(), getChildNodes( ChildNode));
			}
			else if( Names.SOURCE_STORE.compareToIgnoreCase( NodeName) == 0)
			{
				UseSource = importSourceStore( new Store(), getChildNodes( ChildNode));
			}
			else if( Names.SOURCE_FOLLOWER.compareToIgnoreCase( NodeName) == 0)
			{
				UseSource =	importSourceFollower( new Follower(), getChildNodes( ChildNode));
			}
			else if( Names.SOURCE_TIMER.compareToIgnoreCase( NodeName) == 0)
			{
				UseSource =	importSourceTimer( new Timer(), getChildNodes( ChildNode));
			}
			else if( Names.SOURCE_TRIMMER.compareToIgnoreCase( NodeName) == 0)
			{
				UseSource = importSourceTrimmer( new Trim(), getChildNodes( ChildNode));
			}
			else if( Names.SOURCE_PROXY.compareToIgnoreCase( NodeName) == 0)
			{
				UseSource = importSourceProxy( new Proxy(), getChildNodes( ChildNode));
			}
			else
			{
				java.lang.System.out.println( NodeName + " " + TextContent);
			}
		}

		if(( UseSource != null) && ( Name != null))
		{
			readValue( UseSource.getName(), Name);
			UseSource.setModel( ModelId);
		}

		return UseSource;
	}

	private Source importSourceInputAnalog( Analog UseSource, IterableNodeList ChildNodes)
	{
		for( Node ChildNode: ChildNodes)
		{
			String NodeName = ChildNode.getNodeName();
			String TextContent = ChildNode.getTextContent();

			if( Names.SOURCE_INPUT_ANALOG_INPUT.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseSource.getInputId(), TextContent);
			}
			else
			{
				java.lang.System.out.println( NodeName + " " + TextContent);
			}
		}

		return UseSource;		
	}

	private Source importSourceInputButton( Button UseSource, IterableNodeList ChildNodes)
	{
		for( Node ChildNode: ChildNodes)
		{
			String NodeName = ChildNode.getNodeName();
			String TextContent = ChildNode.getTextContent();

			if( Names.SOURCE_INPUT_BUTTON_INPUT.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseSource.getInputId(), TextContent);
			}
			else if( Names.SOURCE_INPUT_BUTTON_INIT.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseSource.getInit(), TextContent);
			}
			else if( Names.SOURCE_INPUT_BUTTON_STORE.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseSource.getStore(), TextContent);
			}
			else if( Names.SOURCE_INPUT_BUTTON_TOGGLE.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseSource.getToggle(), TextContent);
			}
			else if( Names.SOURCE_INPUT_BUTTON_TOP.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseSource.getTop(), TextContent);
			}
			else if( Names.SOURCE_INPUT_BUTTON_BOTTOM.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseSource.getBottom(), TextContent);
			}
			else
			{
				java.lang.System.out.println( NodeName + " " + TextContent);
			}
		}

		return UseSource;		
	}

	private Source importSourceInputRotary( Rotary UseSource, IterableNodeList ChildNodes)
	{
		for( Node ChildNode: ChildNodes)
		{
			String NodeName = ChildNode.getNodeName();
			String TextContent = ChildNode.getTextContent();

			if( Names.SOURCE_INPUT_ROTARY_A_INPUT.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseSource.getAInputId(), TextContent);
			}
			else if( Names.SOURCE_INPUT_ROTARY_B_INPUT.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseSource.getBInputId(), TextContent);
			}
			else if( Names.SOURCE_INPUT_ROTARY_STORE.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseSource.getStore(), TextContent);
			}
			else if( Names.SOURCE_INPUT_ROTARY_INIT.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseSource.getInit(), TextContent);
			}
			else if( Names.SOURCE_INPUT_ROTARY_STEP.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseSource.getStep(), TextContent);
			}
			else if( Names.SOURCE_INPUT_ROTARY_TOP.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseSource.getTop(), TextContent);
			}
			else if( Names.SOURCE_INPUT_ROTARY_BOTTOM.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseSource.getBottom(), TextContent);
			}
			else
			{
				java.lang.System.out.println( NodeName + " " + TextContent);
			}
		}

		return UseSource;		
	}

	private Source importSourceInputSwitch( Switch UseSource, IterableNodeList ChildNodes)
	{
		for( Node ChildNode: ChildNodes)
		{
			String NodeName = ChildNode.getNodeName();
			String TextContent = ChildNode.getTextContent();

			if( Names.SOURCE_INPUT_SWITCH_LOW_INPUT.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseSource.getLowInputId(), TextContent);
			}
			else if( Names.SOURCE_INPUT_SWITCH_HIGH_INPUT.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseSource.getHighInputId(), TextContent);
			}
			else if( Names.SOURCE_INPUT_SWITCH_TOP.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseSource.getTop(), TextContent);
			}
			else if( Names.SOURCE_INPUT_SWITCH_BOTTOM.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseSource.getBottom(), TextContent);
			}
			else
			{
				java.lang.System.out.println( NodeName + " " + TextContent);
			}
		}

		return UseSource;		
	}

	private Source importSourceInputTicker( Ticker UseSource, IterableNodeList ChildNodes)
	{
		for( Node ChildNode: ChildNodes)
		{
			String NodeName = ChildNode.getNodeName();
			String TextContent = ChildNode.getTextContent();

			if( Names.SOURCE_INPUT_TICKER_LOW_INPUT.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseSource.getLowInputId(), TextContent);
			}
			else if( Names.SOURCE_INPUT_TICKER_HIGH_INPUT.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseSource.getHighInputId(), TextContent);
			}
			else if( Names.SOURCE_INPUT_TICKER_INIT.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseSource.getInit(), TextContent);
			}
			else if( Names.SOURCE_INPUT_TICKER_STEP.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseSource.getStep(), TextContent);
			}
			else if( Names.SOURCE_INPUT_TICKER_STORE.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseSource.getStore(), TextContent);
			}
			else if( Names.SOURCE_INPUT_TICKER_TOP.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseSource.getTop(), TextContent);
			}
			else if( Names.SOURCE_INPUT_TICKER_BOTTOM.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseSource.getBottom(), TextContent);
			}
			else
			{
				java.lang.System.out.println( NodeName + " " + TextContent);
			}
		}

		return UseSource;		
	}

	private Source importSourceMap( Map UseSource, IterableNodeList ChildNodes)
	{
		for( Node ChildNode: ChildNodes)
		{
			String NodeName = ChildNode.getNodeName();
			String TextContent = ChildNode.getTextContent();

			if( Names.SOURCE_MAP_INPUT.compareToIgnoreCase( NodeName) == 0)
			{
				importSourceTupel( UseSource.getInput(), getChildNodes( ChildNode));
			}
			else if( Names.SOURCE_MAP_POINTS.compareToIgnoreCase( NodeName) == 0)
			{
				importSourceMapPoints( UseSource, getChildNodes( ChildNode));
			}
			else
			{
				java.lang.System.out.println( NodeName + " " + TextContent);
			}
		}

		return UseSource;		
	}

	private void importSourceMapPoints( Map UseSource, IterableNodeList ChildNodes)
	{
		int PointIndex = 0;

		for( Node ChildNode: ChildNodes)
		{
			String TextContent = ChildNode.getTextContent();
			String NodeName = ChildNode.getNodeName();

			if( Names.SOURCE_MAP_POINT.compareToIgnoreCase( NodeName) == 0)
			{
				importSourceTupel( UseSource.getPoint( PointIndex), getChildNodes( ChildNode));

				PointIndex++;
			}
			else
			{
				java.lang.System.out.println( NodeName + " " + TextContent);
			}
		}
	}

	private Source importSourceMix( Mix UseSource, IterableNodeList ChildNodes)
	{
		for( Node ChildNode: ChildNodes)
		{
			String NodeName = ChildNode.getNodeName();
			String TextContent = ChildNode.getTextContent();

			if( Names.SOURCE_MIX_INPUTS.compareToIgnoreCase( NodeName) == 0)
			{
				importSourceMixInputs( UseSource, getChildNodes( ChildNode));
			}
			else
			{
				java.lang.System.out.println( NodeName + " " + TextContent);
			}
		}

		return UseSource;		
	}

	private void importSourceMixInputs( Mix UseSource, IterableNodeList ChildNodes)
	{
		int InputIndex = 0;

		for( Node ChildNode: ChildNodes)
		{
			String TextContent = ChildNode.getTextContent();
			String NodeName = ChildNode.getNodeName();

			if( Names.SOURCE_MIX_INPUT.compareToIgnoreCase( NodeName) == 0)
			{
				importSourceTupel( UseSource.getInput( InputIndex), getChildNodes( ChildNode));

				InputIndex++;
			}
			else
			{
				java.lang.System.out.println( NodeName + " " + TextContent);
			}
		}
	}

	private Source importSourceStore( Store UseSource, IterableNodeList ChildNodes)
	{
		for( Node ChildNode: ChildNodes)
		{
			String NodeName = ChildNode.getNodeName();
			String TextContent = ChildNode.getTextContent();

			if( Names.SOURCE_STORE_INPUT.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseSource.getInput(), TextContent);
			}
			else if( Names.SOURCE_STORE_INIT.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseSource.getInit(), TextContent);
			}
			else
			{
				java.lang.System.out.println( NodeName + " " + TextContent);
			}
		}

		return UseSource;		
	}

	private Source importSourceFollower( Follower UseSource, IterableNodeList ChildNodes)
	{
		for( Node ChildNode: ChildNodes)
		{
			String NodeName = ChildNode.getNodeName();
			String TextContent = ChildNode.getTextContent();

			if( Names.SOURCE_FOLLOWER_TARGET.compareToIgnoreCase( NodeName) == 0)
			{
				importSourceTupel( UseSource.getTarget(), getChildNodes( ChildNode));
			}
			else if( Names.SOURCE_FOLLOWER_STEP.compareToIgnoreCase( NodeName) == 0)
			{
				importSourceTupel( UseSource.getStep(), getChildNodes( ChildNode));
			}
			else if( Names.SOURCE_FOLLOWER_TRIGGER.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseSource.getTriggerId(), TextContent);
			}
			else if( Names.SOURCE_FOLLOWER_TRIGGER_LOW_LIMIT.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseSource.getTriggerLowLimit(), TextContent);
			}
			else if( Names.SOURCE_FOLLOWER_TRIGGER_HIGH_LIMIT.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseSource.getTriggerHighLimit(), TextContent);
			}
			else
			{
				java.lang.System.out.println( NodeName + " " + TextContent);
			}
		}

		return UseSource;		
	}

	private Source importSourceTimer( Timer UseSource, IterableNodeList ChildNodes)
	{
		for( Node ChildNode: ChildNodes)
		{
			String NodeName = ChildNode.getNodeName();
			String TextContent = ChildNode.getTextContent();

			if( Names.SOURCE_TIMER_INIT_TIME.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseSource.getInitTime(), TextContent);
			}
			else if( Names.SOURCE_TIMER_CURRENT_TIME.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseSource.getCurrentTime(), TextContent);
			}
			else if( Names.SOURCE_TIMER_STORE.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseSource.getStore(), TextContent);
			}
			else if( Names.SOURCE_TIMER_REVERSE.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseSource.getReverse(), TextContent);
			}
			else if( Names.SOURCE_TIMER_STORE.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseSource.getStore(), TextContent);
			}
			else if( Names.SOURCE_TIMER_TRIGGER.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseSource.getTrigger(), TextContent);
			}
			else if( Names.SOURCE_TIMER_TRIGGER_LOW_LIMIT.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseSource.getTriggerLowLimit(), TextContent);
			}
			else if( Names.SOURCE_TIMER_TRIGGER_HIGH_LIMIT.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseSource.getTriggerHighLimit(), TextContent);
			}
			else if( Names.SOURCE_TIMER_WARN_LOW_TIME.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseSource.getWarnLowTime(), TextContent);
			}
			else if( Names.SOURCE_TIMER_WARN_CRITICAL_TIME.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseSource.getWarnCriticalTime(), TextContent);
			}
			else if( Names.SOURCE_TIMER_WARN_PAUSE_TIME.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseSource.getWarnPauseTime(), TextContent);
			}
			else
			{
				java.lang.System.out.println( NodeName + " " + TextContent);
			}
		}

		return UseSource;		
	}

	private Source importSourceTrimmer( Trim UseSource, IterableNodeList ChildNodes)
	{
		for( Node ChildNode: ChildNodes)
		{
			String NodeName = ChildNode.getNodeName();
			String TextContent = ChildNode.getTextContent();

			if( Names.SOURCE_TRIMMER_INPUT_SOURCE.compareToIgnoreCase( NodeName) == 0)
			{
				importSourceTupel( UseSource.getInput(), getChildNodes( ChildNode));
			}
			else if( Names.SOURCE_TRIMMER_TRIM_SOURCE.compareToIgnoreCase( NodeName) == 0)
			{
				importSourceTupel( UseSource.getTrim(), getChildNodes( ChildNode));
			}
			else if( Names.SOURCE_TRIMMER_LIMIT_SOURCE.compareToIgnoreCase( NodeName) == 0)
			{
				importSourceTupel( UseSource.getLimit(), getChildNodes( ChildNode));
			}
			else if( Names.SOURCE_TRIMMER_REVERSE.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseSource.getReverse(), TextContent);
			}
			else if( Names.SOURCE_TRIMMER_POINTS.compareToIgnoreCase( NodeName) == 0)
			{
				importSourceTrimmerPoints( UseSource, getChildNodes( ChildNode));
			}
			else
			{
				java.lang.System.out.println( NodeName + " " + TextContent);
			}
		}

		return UseSource;		
	}

	private void importSourceTrimmerPoints( Trim UseSource, IterableNodeList ChildNodes)
	{
		int PointIndex = 0;

		for( Node ChildNode: ChildNodes)
		{
			String TextContent = ChildNode.getTextContent();
			String NodeName = ChildNode.getNodeName();

			if( Names.SOURCE_TRIMMER_POINT.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseSource.getPoint( PointIndex), TextContent);

				PointIndex++;
			}
			else
			{
				java.lang.System.out.println( NodeName + " " + TextContent);
			}
		}
	}

	private Source importSourceProxy( Proxy UseSource, IterableNodeList ChildNodes)
	{
		for( Node ChildNode: ChildNodes)
		{
			String NodeName = ChildNode.getNodeName();
			String TextContent = ChildNode.getTextContent();

			if( Names.SOURCE_PROXY_SLOT.compareToIgnoreCase( NodeName) == 0)
			{
				readValue( UseSource.getSlot(), TextContent);
			}
			else
			{
				java.lang.System.out.println( NodeName + " " + TextContent);
			}
		}

		return UseSource;		
	}

	private IterableNodeList getChildNodes( Node UseNode)
	{
		return new IterableNodeList( UseNode.getChildNodes());
	}

	private void readValue( Bool Value, String TextContent)
	{
		Value.setConfigurationValue( TextContent);		
	}

	private void readValue( Text Value, String TextContent)
	{
		Value.setConfigurationValue( TextContent);
	}

	private void readValue( Number Value, String TextContent)
	{
		try
		{
			Value.setConfigurationValue( TextContent);
		}
		catch( NumberFormatException e)
		{
		}
		catch( ValueOutOfRangeException e)
		{
		}
	}
}
