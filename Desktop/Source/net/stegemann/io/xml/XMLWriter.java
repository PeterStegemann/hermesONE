package net.stegemann.io.xml;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerConfigurationException;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

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
import net.stegemann.configuration.type.Volume;
import net.stegemann.io.Utility;
import net.stegemann.io.WriteException;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;

public class XMLWriter
{
	public void WriteToFile( Configuration UseConfiguration, String FileName) throws WriteException
	{
		DocumentBuilderFactory Factory = DocumentBuilderFactory.newInstance();
		DocumentBuilder Builder;

		try
		{
			Builder = Factory.newDocumentBuilder();
		}
		catch( ParserConfigurationException Reason)
		{
			throw new WriteException( "Failed to create xml document builder.", Reason);
		}

		Document ConfigurationDocument = Builder.newDocument();

		createDocument( UseConfiguration, ConfigurationDocument);

		TransformerFactory UseTransformerFactory = TransformerFactory.newInstance();
		Transformer UseTransformer;

		try
		{
			UseTransformer = UseTransformerFactory.newTransformer();
		}
		catch( TransformerConfigurationException Reason)
		{
			throw new WriteException( "Failed to create xml tranformer.", Reason);
		}

		UseTransformer.setOutputProperty( OutputKeys.OMIT_XML_DECLARATION, "yes");
		UseTransformer.setOutputProperty("{http://xml.apache.org/xslt}indent-amount", "2");
		UseTransformer.setOutputProperty( OutputKeys.INDENT, "yes");

		DOMSource UseSource = new DOMSource( ConfigurationDocument);

		FileOutputStream ResultFile;

		try
		{
			ResultFile = new FileOutputStream( FileName);
		}
		catch( FileNotFoundException Reason)
		{
			throw new WriteException( "Failed to create file '" + FileName + "'.", Reason);
		}

		StreamResult Result = new StreamResult( ResultFile);

		try
		{
			UseTransformer.transform( UseSource, Result);
		}
		catch( TransformerException Reason)
		{
			throw new WriteException( "Failed to transform xml.", Reason);
		}
	}

	private void createDocument( Configuration UseConfiguration, Document ConfigurationDocument)
	{
		Node ConfigurationNode = ConfigurationDocument.createElement( Names.CONFIGURATION);
		ConfigurationDocument.appendChild( ConfigurationNode);

		exportConfiguration( UseConfiguration, ConfigurationDocument, ConfigurationNode);
	}

	private void exportConfiguration( Configuration UseConfiguration, Document ConfigurationDocument,
		Node ConfigurationNode)
	{
		System UseSystem = UseConfiguration.getSystem();

		appendNode( ConfigurationDocument, ConfigurationNode, Names.SYSTEM_ANALOG_INPUTS,
			UseSystem.getAnalogInputs());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SYSTEM_DIGITAL_INPUTS,
			UseSystem.getDigitalInputs());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SYSTEM_OUTPUT_CHANNELS,
			UseSystem.getOutputChannels());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SYSTEM_OUTPUTS,
			UseSystem.getOutputs());

		appendNode( ConfigurationDocument, ConfigurationNode, Names.SYSTEM_OWNER,
			UseSystem.getOwner());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SYSTEM_SETUP_BACKLIGHT,
			UseSystem.getSetupBacklight());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SYSTEM_SETUP_BLANK_TIME,
			UseSystem.getSetupBlankTime());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SYSTEM_STATUS_BACKLIGHT,
			UseSystem.getStatusBacklight());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SYSTEM_STATUS_CONTRAST,
			UseSystem.getStatusContrast());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SYSTEM_STATUS_BLANK_TIME,
			UseSystem.getStatusBlankTime());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SYSTEM_STATUS_INVERTED,
			UseSystem.getStatusInverted());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SYSTEM_SELECTED_MODEL,
			UseSystem.getSelectedModel());

		Node NewNode;

		NewNode = ConfigurationDocument.createElement( Names.BATTERY);
		ConfigurationNode.appendChild( NewNode);

		exportBattery( UseSystem.getBattery(), ConfigurationDocument, NewNode);

		NewNode = ConfigurationDocument.createElement( Names.CALIBRATIONS);
		ConfigurationNode.appendChild( NewNode);

		exportCalibrations( UseSystem.getCalibrations(), ConfigurationDocument, NewNode);

		NewNode = ConfigurationDocument.createElement( Names.MODELS);
		ConfigurationNode.appendChild( NewNode);

		exportModels( UseConfiguration.getModels(), ConfigurationDocument, NewNode);

		NewNode = ConfigurationDocument.createElement( Names.TYPES);
		ConfigurationNode.appendChild( NewNode);

		exportTypes( UseConfiguration, ConfigurationDocument, NewNode);

		NewNode = ConfigurationDocument.createElement( Names.SOURCES);
		ConfigurationNode.appendChild( NewNode);

		exportSources( UseConfiguration.getSources(), ConfigurationDocument, NewNode);

		NewNode = ConfigurationDocument.createElement( Names.PPMS);
		ConfigurationNode.appendChild( NewNode);

		exportPPMs( UseSystem.getPPMs(), ConfigurationDocument, NewNode);
	}

	private void exportPPMs( PPMs UsePPMs, Document ConfigurationDocument, Node ConfigurationNode)
	{
		int PPMId = 0;

		for( PPM CurrentPPM: UsePPMs)
		{
			ConfigurationNode.appendChild( ConfigurationDocument.createComment(
				Names.PPM + " " + PPMId));
			Node NewNode = ConfigurationDocument.createElement( Names.PPM);
			ConfigurationNode.appendChild( NewNode);

			exportPPM( CurrentPPM, ConfigurationDocument, NewNode);

			PPMId++;
		}
	}

	private void exportPPM( PPM UsePPM, Document ConfigurationDocument, Node ConfigurationNode)
	{
		appendNode( ConfigurationDocument, ConfigurationNode, Names.PPM_INVERTED,
			UsePPM.getPPMInverted());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.PPM_CENTER,
			UsePPM.getPPMCenter());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.PPM_NAME,
			UsePPM.getName());

		Element NewNode = ConfigurationDocument.createElement( Names.PPM_CHANNEL_MAPPINGS);
		ConfigurationNode.appendChild( NewNode);

		exportChannelMappings( UsePPM.getChannelMappings(), ConfigurationDocument, NewNode);
	}

	private void exportChannelMappings( ChannelMappings UseChannelMappings,
		Document ConfigurationDocument, Node ConfigurationNode)
	{
		for( Number CurrentChannelMapping: UseChannelMappings)
		{
			appendNode( ConfigurationDocument, ConfigurationNode, Names.PPM_CHANNEL_MAPPING,
				CurrentChannelMapping);
		}
	}

	private void exportSources( Sources UseSources, Document ConfigurationDocument,
		Node ConfigurationNode)
	{
		int SourceId = 0;

		for( Source CurrentSource: UseSources)
		{
			// Fill XML with empty sources until we reach the current source id.
			while( SourceId < CurrentSource.getId().getValue())
			{
				ConfigurationNode.appendChild( ConfigurationDocument.createComment(
					Names.SOURCE + " " + SourceId));
				Node NewNode = ConfigurationDocument.createElement( Names.SOURCE);
				ConfigurationNode.appendChild( NewNode);

				SourceId++;
			}

			ConfigurationNode.appendChild( ConfigurationDocument.createComment(
					Names.SOURCE + " " + SourceId));
			Node NewNode = ConfigurationDocument.createElement( Names.SOURCE);
			ConfigurationNode.appendChild( NewNode);

			exportSource( CurrentSource, ConfigurationDocument, NewNode);

			SourceId++;
		}
	}

	private void exportSource( Source UseSource, Document ConfigurationDocument,
		Node ConfigurationNode)
	{
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SOURCE_NAME,
			UseSource.getName());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SOURCE_MODEL,
			UseSource.getModel());

		Node NewNode = null;

		if( UseSource instanceof Analog)
		{
			NewNode = ConfigurationDocument.createElement( Names.SOURCE_INPUT_ANALOG);

			exportSourceInputAnalog(( Analog) UseSource, ConfigurationDocument, NewNode);
		}
		else if( UseSource instanceof Button)
		{
			NewNode = ConfigurationDocument.createElement( Names.SOURCE_INPUT_BUTTON);

			exportSourceInputButton(( Button) UseSource, ConfigurationDocument, NewNode);
		}
		else if( UseSource instanceof Rotary)
		{
			NewNode = ConfigurationDocument.createElement( Names.SOURCE_INPUT_ROTARY);

			exportSourceInputRotary(( Rotary) UseSource, ConfigurationDocument, NewNode);
		}
		else if( UseSource instanceof Switch)
		{
			NewNode = ConfigurationDocument.createElement( Names.SOURCE_INPUT_SWITCH);

			exportSourceInputSwitch(( Switch) UseSource, ConfigurationDocument, NewNode);
		}
		else if( UseSource instanceof Ticker)
		{
			NewNode = ConfigurationDocument.createElement( Names.SOURCE_INPUT_TICKER);

			exportSourceInputTicker(( Ticker) UseSource, ConfigurationDocument, NewNode);
		}
		else if( UseSource instanceof Map)
		{
			NewNode = ConfigurationDocument.createElement( Names.SOURCE_MAP);

			exportSourceMap(( Map) UseSource, ConfigurationDocument, NewNode);
		}
		else if( UseSource instanceof Mix)
		{
			NewNode = ConfigurationDocument.createElement( Names.SOURCE_MIX);

			exportSourceMix(( Mix) UseSource, ConfigurationDocument, NewNode);
		}
		else if( UseSource instanceof Store)
		{
			NewNode = ConfigurationDocument.createElement( Names.SOURCE_STORE);

			exportSourceStore(( Store) UseSource, ConfigurationDocument, NewNode);
		}
		else if( UseSource instanceof Follower)
		{
			NewNode = ConfigurationDocument.createElement( Names.SOURCE_FOLLOWER);

			exportSourceFollower(( Follower) UseSource, ConfigurationDocument, NewNode);
		}
		else if( UseSource instanceof Timer)
		{
			NewNode = ConfigurationDocument.createElement( Names.SOURCE_TIMER);

			exportSourceTimer(( Timer) UseSource, ConfigurationDocument, NewNode);
		}
		else if( UseSource instanceof Trim)
		{
			NewNode = ConfigurationDocument.createElement( Names.SOURCE_TRIMMER);

			exportSourceTrimmer(( Trim) UseSource, ConfigurationDocument, NewNode);
		}
		else if( UseSource instanceof Proxy)
		{
			NewNode = ConfigurationDocument.createElement( Names.SOURCE_PROXY);

			exportSourceProxy(( Proxy) UseSource, ConfigurationDocument, NewNode);
		}

		if( NewNode != null)
		{
			ConfigurationNode.appendChild( NewNode);
		}
	}

	private void exportSourceInputAnalog( Analog UseSource, Document ConfigurationDocument,
		Node ConfigurationNode)
	{
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SOURCE_INPUT_ANALOG_INPUT,
			UseSource.getInputId());
	}

	private void exportSourceInputButton( Button UseSource, Document ConfigurationDocument,
		Node ConfigurationNode)
	{
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SOURCE_INPUT_BUTTON_INPUT,
			UseSource.getInputId());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SOURCE_INPUT_BUTTON_INIT,
			UseSource.getInit());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SOURCE_INPUT_BUTTON_STORE,
			UseSource.getStore());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SOURCE_INPUT_BUTTON_TOGGLE,
			UseSource.getToggle());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SOURCE_INPUT_BUTTON_TOP,
			UseSource.getTop());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SOURCE_INPUT_BUTTON_BOTTOM,
			UseSource.getBottom());
	}

	private void exportSourceInputRotary( Rotary UseSource, Document ConfigurationDocument,
		Node ConfigurationNode)
	{
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SOURCE_INPUT_ROTARY_A_INPUT,
			UseSource.getAInputId());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SOURCE_INPUT_ROTARY_B_INPUT,
			UseSource.getBInputId());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SOURCE_INPUT_ROTARY_STORE,
			UseSource.getStore());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SOURCE_INPUT_ROTARY_INIT,
			UseSource.getInit());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SOURCE_INPUT_ROTARY_STEP,
			UseSource.getStep());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SOURCE_INPUT_ROTARY_TOP,
			UseSource.getTop());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SOURCE_INPUT_ROTARY_BOTTOM,
			UseSource.getBottom());
	}

	private void exportSourceInputSwitch( Switch UseSource, Document ConfigurationDocument,
		Node ConfigurationNode)
	{
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SOURCE_INPUT_SWITCH_LOW_INPUT,
					UseSource.getLowInputId());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SOURCE_INPUT_SWITCH_HIGH_INPUT,
					UseSource.getHighInputId());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SOURCE_INPUT_SWITCH_TOP,
					UseSource.getTop());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SOURCE_INPUT_SWITCH_BOTTOM,
					UseSource.getBottom());
	}

	private void exportSourceInputTicker( Ticker UseSource, Document ConfigurationDocument,
		Node ConfigurationNode)
	{
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SOURCE_INPUT_TICKER_LOW_INPUT,
			UseSource.getLowInputId());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SOURCE_INPUT_TICKER_HIGH_INPUT,
			UseSource.getHighInputId());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SOURCE_INPUT_TICKER_INIT,
			UseSource.getInit());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SOURCE_INPUT_TICKER_STEP,
			UseSource.getStep());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SOURCE_INPUT_TICKER_STORE,
			UseSource.getStore());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SOURCE_INPUT_TICKER_TOP,
			UseSource.getTop());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SOURCE_INPUT_TICKER_BOTTOM,
			UseSource.getBottom());
	}

	private void exportSourceMap( Map UseSource, Document ConfigurationDocument,
		Node ConfigurationNode)
	{
		exportSourceTupel( UseSource.getInput(), ConfigurationDocument, ConfigurationNode,
		   Names.SOURCE_MAP_INPUT);

		Node NewNode = ConfigurationDocument.createElement( Names.SOURCE_MAP_POINTS);
		ConfigurationNode.appendChild( NewNode);

		for( SourceWithVolume Input: UseSource.getPoints())
		{
			exportSourceTupel( Input, ConfigurationDocument, NewNode, Names.SOURCE_MAP_POINT);
		}
	}

	private void exportSourceMix( Mix UseSource, Document ConfigurationDocument,
		Node ConfigurationNode)
	{
		Node NewNode = ConfigurationDocument.createElement( Names.SOURCE_MIX_INPUTS);
		ConfigurationNode.appendChild( NewNode);

		for( SourceWithVolume Input: UseSource.getInputs())
		{
			exportSourceTupel( Input, ConfigurationDocument, NewNode, Names.SOURCE_MIX_INPUT);
		}
	}

	private void exportSourceStore( Store UseSource, Document ConfigurationDocument,
		Node ConfigurationNode)
	{
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SOURCE_STORE_INPUT,
			UseSource.getInput());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SOURCE_STORE_INIT,
			UseSource.getInit());
	}

	private void exportSourceFollower( Follower UseSource, Document ConfigurationDocument,
		Node ConfigurationNode)
	{
		exportSourceTupel( UseSource.getTarget(), ConfigurationDocument, ConfigurationNode,
		   Names.SOURCE_FOLLOWER_TARGET);
		exportSourceTupel( UseSource.getStep(), ConfigurationDocument, ConfigurationNode,
			Names.SOURCE_FOLLOWER_STEP);

		appendNode( ConfigurationDocument, ConfigurationNode, Names.SOURCE_FOLLOWER_TRIGGER,
			UseSource.getTriggerId());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SOURCE_FOLLOWER_TRIGGER_LOW_LIMIT,
			UseSource.getTriggerLowLimit());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SOURCE_FOLLOWER_TRIGGER_HIGH_LIMIT,
			UseSource.getTriggerHighLimit());
	}

	private void exportSourceTimer( Timer UseSource, Document ConfigurationDocument,
		Node ConfigurationNode)
	{
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SOURCE_TIMER_INIT_TIME,
			UseSource.getInitTime());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SOURCE_TIMER_CURRENT_TIME,
			UseSource.getCurrentTime());

		appendNode( ConfigurationDocument, ConfigurationNode, Names.SOURCE_TIMER_STORE,
			UseSource.getStore());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SOURCE_TIMER_REVERSE,
			UseSource.getReverse());

		appendNode( ConfigurationDocument, ConfigurationNode, Names.SOURCE_TIMER_TRIGGER,
			UseSource.getTrigger());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SOURCE_TIMER_TRIGGER_LOW_LIMIT,
			UseSource.getTriggerLowLimit());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SOURCE_TIMER_TRIGGER_HIGH_LIMIT,
			UseSource.getTriggerHighLimit());

		appendNode( ConfigurationDocument, ConfigurationNode, Names.SOURCE_TIMER_WARN_LOW_TIME,
			UseSource.getWarnLowTime());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SOURCE_TIMER_WARN_CRITICAL_TIME,
			UseSource.getWarnCriticalTime());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SOURCE_TIMER_WARN_PAUSE_TIME,
			UseSource.getWarnPauseTime());
	}

	private void exportSourceTrimmer( Trim UseSource, Document ConfigurationDocument,
			Node ConfigurationNode)
	{
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SOURCE_TRIMMER_REVERSE,
			UseSource.getReverse());

		exportSourceTupel( UseSource.getInput(), ConfigurationDocument, ConfigurationNode,
		   Names.SOURCE_TRIMMER_INPUT_SOURCE);

		exportSourceTupel( UseSource.getTrim(), ConfigurationDocument, ConfigurationNode,
		   Names.SOURCE_TRIMMER_TRIM_SOURCE);

		exportSourceTupel( UseSource.getLimit(), ConfigurationDocument, ConfigurationNode,
		   Names.SOURCE_TRIMMER_LIMIT_SOURCE);

		Node NewNode = ConfigurationDocument.createElement( Names.SOURCE_TRIMMER_POINTS);
		ConfigurationNode.appendChild( NewNode);

		exportSourceTrimmerPoints( UseSource, ConfigurationDocument, NewNode);
	}

	private void exportSourceProxy( Proxy UseSource, Document ConfigurationDocument,
		Node ConfigurationNode)
	{
		appendNode( ConfigurationDocument, ConfigurationNode, Names.SOURCE_PROXY_SLOT,
			UseSource.getSlot());
	}

	private void exportSourceTrimmerPoints( Trim UseSource, Document ConfigurationDocument,
		Node ConfigurationNode)
	{
		for( int PointIndex = 0; PointIndex < Trim.POINTS; PointIndex++)
		{
			Volume Point = UseSource.getPoint( PointIndex);

			appendNode( ConfigurationDocument, ConfigurationNode, Names.SOURCE_TRIMMER_POINT, Point);
		}
	}

	private void exportTypes( Configuration UseConfiguration, Document ConfigurationDocument,
		Node ConfigurationNode)
	{
		// TypeIds don't start with zero!
		int TypeId = Model.TYPE_START;

		Types UseTypes = UseConfiguration.getTypes();

		for( Type CurrentType: UseTypes)
		{
			// fill XML with empty types until we reach the current type id.
			while( TypeId < CurrentType.getId().getValue())
			{
				ConfigurationNode.appendChild( ConfigurationDocument.createComment(
					Names.TYPE + " " + TypeId));
				Node NewNode = ConfigurationDocument.createElement( Names.TYPE);
				ConfigurationNode.appendChild( NewNode);

				TypeId++;
			}

			ConfigurationNode.appendChild( ConfigurationDocument.createComment(
				Names.TYPE + " " + TypeId));
			Node NewNode = ConfigurationDocument.createElement( Names.TYPE);
			ConfigurationNode.appendChild( NewNode);

			exportType( CurrentType, ConfigurationDocument, NewNode);

			TypeId++;
		}
	}

	private void exportType( Type UseType, Document ConfigurationDocument, Node ConfigurationNode)
	{
		appendNode( ConfigurationDocument, ConfigurationNode, Names.TYPE_NAME, UseType.getName());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.TYPE_STATE,
			Utility.ConvertTypeState( UseType.getState()));
	}

	private void exportModels( Models UseModels, Document ConfigurationDocument,
		Node ConfigurationNode)
	{
		int ModelId = 0;

		for( Model CurrentModel: UseModels)
		{
			// fill XML with empty models until we reach the current model id.
			while( ModelId < CurrentModel.getId().getValue())
			{
				ConfigurationNode.appendChild( ConfigurationDocument.createComment(
					Names.MODEL + " " + ModelId));
				Node NewNode = ConfigurationDocument.createElement( Names.MODEL);
				ConfigurationNode.appendChild( NewNode);

				ModelId++;
			}

			ConfigurationNode.appendChild( ConfigurationDocument.createComment(
				Names.MODEL + " " + ModelId));
			Node NewNode = ConfigurationDocument.createElement( Names.MODEL);
			ConfigurationNode.appendChild( NewNode);

			exportModel( CurrentModel, ConfigurationDocument, NewNode);

			ModelId++;
		}
	}

	private void exportModel( Model UseModel, Document ConfigurationDocument, Node ConfigurationNode)
	{
		appendNode( ConfigurationDocument, ConfigurationNode, Names.MODEL_NAME, UseModel.getName());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.MODEL_STATE,
			Utility.ConvertModelState( UseModel.getState()));
		appendNode( ConfigurationDocument, ConfigurationNode, Names.MODEL_RF_MODE,
			UseModel.getRFMode());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.MODEL_TYPE, UseModel.getTypeId());

		Node NewNode;

		NewNode = ConfigurationDocument.createElement( Names.MODEL_STATUS_SOURCES);
		ConfigurationNode.appendChild( NewNode);

		exportStatusSources( UseModel, ConfigurationDocument, NewNode);

		NewNode = ConfigurationDocument.createElement( Names.MODEL_STATUS_TIMES);
		ConfigurationNode.appendChild( NewNode);

		exportStatusTimes( UseModel, ConfigurationDocument, NewNode);

		NewNode = ConfigurationDocument.createElement( Names.CHANNELS);
		ConfigurationNode.appendChild( NewNode);

		exportChannels( UseModel.getChannels(), ConfigurationDocument, NewNode);

		NewNode = ConfigurationDocument.createElement( Names.MODEL_PROXY_REFERENCES);
		ConfigurationNode.appendChild( NewNode);

		exportProxyReferences( UseModel.getProxyReferences(), ConfigurationDocument, NewNode);
	}

	private void exportStatusSources( Model UseModel, Document ConfigurationDocument,
		Node ConfigurationNode)
	{
		appendNode( ConfigurationDocument, ConfigurationNode, Names.MODEL_STATUS_SOURCE,
			UseModel.getStatusSourceId( StatusSource.LEFT_SIDE));
		appendNode( ConfigurationDocument, ConfigurationNode, Names.MODEL_STATUS_SOURCE,
			UseModel.getStatusSourceId( StatusSource.LEFT_BOTTOM));
		appendNode( ConfigurationDocument, ConfigurationNode, Names.MODEL_STATUS_SOURCE,
			UseModel.getStatusSourceId( StatusSource.RIGHT_SIDE));
		appendNode( ConfigurationDocument, ConfigurationNode, Names.MODEL_STATUS_SOURCE,
			UseModel.getStatusSourceId( StatusSource.RIGHT_BOTTOM));
	}

	private void exportStatusTimes( Model UseModel, Document ConfigurationDocument,
									  		  Node ConfigurationNode)
	{
		appendNode( ConfigurationDocument, ConfigurationNode, Names.MODEL_STATUS_TIME,
			UseModel.getStatusTimeId( StatusTime.TOP));
		appendNode( ConfigurationDocument, ConfigurationNode, Names.MODEL_STATUS_TIME,
			UseModel.getStatusTimeId( StatusTime.BOTTOM));
	}

	private void exportChannels( Channels UseChannels, Document ConfigurationDocument,
		Node ConfigurationNode)
	{
		int ChannelId = 0;

		for( Channel CurrentChannel: UseChannels)
		{
			ConfigurationNode.appendChild( ConfigurationDocument.createComment(
				Names.CHANNEL + " " + ChannelId));
			Node NewNode = ConfigurationDocument.createElement( Names.CHANNEL);
			ConfigurationNode.appendChild( NewNode);

			exportChannel( CurrentChannel, ConfigurationDocument, NewNode);

			ChannelId++;
		}
	}

	private void exportChannel( Channel UseChannel, Document ConfigurationDocument,
		Node ConfigurationNode)
	{
		appendNode( ConfigurationDocument, ConfigurationNode, Names.CHANNEL_NAME,
			UseChannel.getName());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.CHANNEL_REVERSE,
			UseChannel.getReverse());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.CHANNEL_MODE,
			UseChannel.getMode());

		exportSourceTupel( UseChannel.getInput(), ConfigurationDocument, ConfigurationNode,
			Names.CHANNEL_INPUT);

		exportSourceTupel( UseChannel.getTrim(), ConfigurationDocument, ConfigurationNode,
			Names.CHANNEL_TRIM);

		exportSourceTupel( UseChannel.getLimit(), ConfigurationDocument, ConfigurationNode,
			Names.CHANNEL_LIMIT);

		Node NewNode = ConfigurationDocument.createElement( Names.CHANNEL_POINTS);
		ConfigurationNode.appendChild( NewNode);

		exportChannelPoints( UseChannel, ConfigurationDocument, NewNode);
	}

	private void exportChannelPoints( Channel UseChannel, Document ConfigurationDocument,
		Node ConfigurationNode)
	{
		for( int ChannelPointIndex = 0; ChannelPointIndex < Channel.POINTS; ChannelPointIndex++)
		{
			Volume ChannelPoint = UseChannel.getPoint( ChannelPointIndex);

			appendNode( ConfigurationDocument, ConfigurationNode, Names.CHANNEL_POINT, ChannelPoint);
		}
	}

	private void exportProxyReferences( ProxyReferences UseProxyReferences,
		Document ConfigurationDocument, Node ConfigurationNode)
	{
		for( SourceWithVolume CurrentProxyReference: UseProxyReferences)
		{
			exportSourceTupel( CurrentProxyReference, ConfigurationDocument, ConfigurationNode,
				Names.MODEL_PROXY_REFERENCE);
		}
	}

	private void exportSourceTupel( SourceWithVolume UseSourceTupel, Document ConfigurationDocument,
		Node ConfigurationNode, String NodeName)
	{
		Node NewNode;

		NewNode = ConfigurationDocument.createElement( NodeName);
		ConfigurationNode.appendChild( NewNode);

		appendNode( ConfigurationDocument, NewNode, Names.SOURCE_TUPEL_SOURCE_ID,
			UseSourceTupel.getSourceId());
		appendNode( ConfigurationDocument, NewNode, Names.SOURCE_TUPEL_VOLUME,
			UseSourceTupel.getVolume());
	}

	private void exportBattery( Battery UseBattery, Document ConfigurationDocument,
		Node ConfigurationNode)
	{
		appendNode( ConfigurationDocument, ConfigurationNode, Names.BATTERY_WARN_LOW_VOLTAGE,
			UseBattery.getWarnLowVoltage());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.BATTERY_WARN_CRITICAL_VOLTAGE,
			UseBattery.getWarnCriticalVoltage());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.BATTERY_MINIMUM_VOLTAGE,
			UseBattery.getMinimumVoltage());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.BATTERY_MAXIMUM_VOLTAGE,
			UseBattery.getMaximumVoltage());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.BATTERY_CALIBRATION_VOLTAGE,
			UseBattery.getCalibrationVoltage());
	}

	private void exportCalibrations( Calibrations UseCalibrations, Document ConfigurationDocument,
		Node ConfigurationNode)
	{
		int CalibrationId = 0;

		for( Calibration CurrentCalibration: UseCalibrations)
		{
			ConfigurationNode.appendChild( ConfigurationDocument.createComment( Names.CALIBRATION + " " + CalibrationId));
			Node NewNode = ConfigurationDocument.createElement( Names.CALIBRATION);
			ConfigurationNode.appendChild( NewNode);

			exportCalibration( CurrentCalibration, ConfigurationDocument, NewNode);

			CalibrationId++;
		}
	}

	private void exportCalibration( Calibration UseCalibration, Document ConfigurationDocument,
		Node ConfigurationNode)
	{
		appendNode( ConfigurationDocument, ConfigurationNode, Names.CALIBRATION_HIGH,
			UseCalibration.getHigh());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.CALIBRATION_CENTER,
			UseCalibration.getCenter());
		appendNode( ConfigurationDocument, ConfigurationNode, Names.CALIBRATION_LOW,
			UseCalibration.getLow());
	}

	private void appendNode( Document ConfigurationDocument, Node ConfigurationNode, String NodeTag,
		Bool Value)
	{
		appendNode( ConfigurationDocument, ConfigurationNode, NodeTag, Value.getConfigurationValue());		
	}

	private void appendNode( Document ConfigurationDocument, Node ConfigurationNode, String NodeTag,
		Text Value)
	{
		appendNode( ConfigurationDocument, ConfigurationNode, NodeTag, Value.getConfigurationValue());
	}

	private void appendNode( Document ConfigurationDocument, Node ConfigurationNode, String NodeTag,
		Number Value)
	{
		appendNode( ConfigurationDocument, ConfigurationNode, NodeTag, Value.getConfigurationValue());
	}

	private void appendNode( Document ConfigurationDocument, Node ConfigurationNode, String NodeTag,
		int Value)
	{
		appendNode( ConfigurationDocument, ConfigurationNode, NodeTag,
			java.lang.Integer.toString( Value));
	}

	private void appendNode( Document ConfigurationDocument, Node ConfigurationNode, String NodeTag,
		String Value)
	{
		Node NewNode = ConfigurationDocument.createElement( NodeTag);
		ConfigurationNode.appendChild( NewNode);
		NewNode.setTextContent( Value);
	}
}
