package net.stegemann.io.xml;

import net.stegemann.configuration.*;
import net.stegemann.configuration.Model.StatusSource;
import net.stegemann.configuration.System;
import net.stegemann.configuration.Model.StatusTime;
import net.stegemann.configuration.source.*;
import net.stegemann.configuration.source.input.*;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.*;
import net.stegemann.io.DocumentException;
import net.stegemann.io.ReadException;
import net.stegemann.io.Utility;
import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

public class XMLReader
{
    private static final boolean debug = false;

    private final DocumentGenerator documentGenerator;

    public enum Mode
    {
        All,
        System,
        Models
    }

    XMLReader( DocumentGenerator documentGenerator)
    {
        this.documentGenerator = documentGenerator;
    }

    public void readFromFile( Configuration configuration, String fileName, Mode mode)
        throws DocumentException
    {
        Document document = documentGenerator.readDocument( fileName);

        switch( mode)
        {
            case All -> configuration.clear();
            case Models -> configuration.clearModels();
            case System -> configuration.clearSystem();
        }

        interpretDocument( configuration, document, mode);
    }

    private void interpretDocument( Configuration configuration, Document document, Mode mode)
        throws ReadException
    {
        NodeList childNodes = document.getChildNodes();

        Node node = childNodes.item( 0);

        if( node == null)
        {
            throw new ReadException( "No configuration node found, invalid configuration.");
        }

        importConfiguration( configuration, getChildNodes( node), mode);
    }

    private void importConfiguration( Configuration configuration, IterableNodeList childNodes, Mode mode)
    {
        for( Node childNode: childNodes)
        {
            String nodeName = childNode.getNodeName();
            String textContent = childNode.getTextContent();

            boolean systemResult = true;

            if(( mode == Mode.All) || ( mode == Mode.System))
            {
                switch( nodeName)
                {
                    case Names.SYSTEM -> importSystem( configuration.getSystem(), getChildNodes( childNode));

                    default -> systemResult = false;
                }
            }

            boolean modelResult = true;

            if(( mode == Mode.All) || ( mode == Mode.Models))
            {
                if( Names.MODELS.compareToIgnoreCase( nodeName) == 0)
                {
                    importModels( configuration.getModels(), getChildNodes( childNode));
                }
                else if( Names.TYPES.compareToIgnoreCase( nodeName) == 0)
                {
                    importTypes( configuration.getTypes(), getChildNodes( childNode));
                }
                else if( Names.SOURCES.compareToIgnoreCase( nodeName) == 0)
                {
                    importSources( configuration.getSources(), getChildNodes( childNode));
                }
                else
                {
                    modelResult = false;
                }
            }

            if(( systemResult == false) && ( modelResult == false))
            {
                if( debug) java.lang.System.out.println( nodeName + " " + textContent);
            }
        }

        configuration.fill();
    }

    private void importSystem( System system, IterableNodeList childNodes)
    {
        for( Node childNode: childNodes)
        {
            String nodeName = childNode.getNodeName();
            String textContent = childNode.getTextContent();

            switch( nodeName)
            {
                case Names.SYSTEM_ANALOG_INPUTS -> readValue( system.getAnalogInputs(), textContent);
                case Names.SYSTEM_DIGITAL_INPUTS -> readValue( system.getDigitalInputs(), textContent);
                case Names.SYSTEM_OUTPUT_CHANNELS -> readValue( system.getOutputChannels(), textContent);
                case Names.SYSTEM_OUTPUTS -> readValue( system.getOutputs(), textContent);
                case Names.SYSTEM_OWNER -> readValue( system.getOwner(), textContent);
                case Names.SYSTEM_SETUP_BACKLIGHT -> readValue( system.getSetupBacklight(), textContent);
                case Names.SYSTEM_SETUP_BLANK_TIME -> readValue( system.getSetupBlankTime(), textContent);
                case Names.SYSTEM_STATUS_BACKLIGHT -> readValue( system.getStatusBacklight(), textContent);
                case Names.SYSTEM_STATUS_CONTRAST -> readValue( system.getStatusContrast(), textContent);
                case Names.SYSTEM_STATUS_BLANK_TIME -> readValue( system.getStatusBlankTime(), textContent);
                case Names.SYSTEM_STATUS_INVERTED -> readValue( system.getStatusInverted(), textContent);
                case Names.SYSTEM_SELECTED_MODEL -> readValue( system.getSelectedModel(), textContent);
                case Names.BATTERY -> importBattery( system.getBattery(), getChildNodes( childNode));
                case Names.CALIBRATIONS -> importCalibrations( system.getCalibrations(), getChildNodes( childNode));
                case Names.PPMS -> importPPMs( system.getPpms(), getChildNodes( childNode));

                default ->
                {
                    if( debug) java.lang.System.out.println( nodeName + " " + textContent);
                }
            }
        }
    }

    private void importPPMs( PPMs ppms, IterableNodeList childNodes)
    {
        int ppmId = 0;

        for( Node childNode: childNodes)
        {
            String nodeName = childNode.getNodeName();

            if( Names.PPM.compareToIgnoreCase( nodeName) == 0)
            {
                importPPM( ppms.getPPM( ppmId++), getChildNodes( childNode));
            }
            else
            {
                if( debug) java.lang.System.out.println( nodeName);
            }
        }
    }

    private void importPPM( PPM ppm, IterableNodeList childNodes)
    {
        for( Node childNode: childNodes)
        {
            String nodeName = childNode.getNodeName();
            String textContent = childNode.getTextContent();

            if( Names.PPM_INVERTED.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( ppm.getInverted(), textContent);
            }
            else if( Names.PPM_CENTER.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( ppm.getCenter(), textContent);
            }
            else if( Names.PPM_NAME.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( ppm.getName(), textContent);
            }
            else if( Names.PPM_CHANNEL_MAPPINGS.compareToIgnoreCase( nodeName) == 0)
            {
                importChannelMappings( ppm.getChannelMappings(), getChildNodes( childNode));
            }
            else
            {
                if( debug) java.lang.System.out.println( nodeName);
            }
        }
    }

    private void importChannelMappings( ChannelMappings channelMappings, IterableNodeList childNodes)
    {
        int channelIndex = 0;

        for( Node childNode: childNodes)
        {
            String nodeName = childNode.getNodeName();
            String textContent = childNode.getTextContent();

            if( Names.PPM_CHANNEL_MAPPING.compareToIgnoreCase( nodeName) == 0)
            {
                Number channelMapping = new Number( ChannelMappings.MINIMUM_VALUE, ChannelMappings.MAXIMUM_VALUE);

                readValue( channelMapping, textContent);

                channelMappings.setChannelMapping( channelIndex++, channelMapping);
            }
            else
            {
                if( debug) java.lang.System.out.println( nodeName);
            }
        }
    }

    private void importBattery( Battery battery, IterableNodeList childNodes)
    {
        for( Node childNode: childNodes)
        {
            String nodeName = childNode.getNodeName();
            String textContent = childNode.getTextContent();

            if( Names.BATTERY_WARN_LOW_VOLTAGE.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( battery.getWarnLowVoltage(), textContent);
            }
            else if( Names.BATTERY_WARN_CRITICAL_VOLTAGE.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( battery.getWarnCriticalVoltage(), textContent);
            }
            else if( Names.BATTERY_MINIMUM_VOLTAGE.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( battery.getMinimumVoltage(), textContent);
            }
            else if( Names.BATTERY_MAXIMUM_VOLTAGE.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( battery.getMaximumVoltage(), textContent);
            }
            else if( Names.BATTERY_CALIBRATION_VOLTAGE.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( battery.getCalibrationVoltage(), textContent);
            }
            else
            {
                if( debug) java.lang.System.out.println( nodeName + " " + textContent);
            }
        }
    }

    private void importCalibrations( Calibrations UseCalibrations, IterableNodeList childNodes)
    {
        int CalibrationIndex = 0;

        for( Node childNode: childNodes)
        {
            String nodeName = childNode.getNodeName();

            if( Names.CALIBRATION.compareToIgnoreCase( nodeName) == 0)
            {
                Calibration NewCalibration = new Calibration();

                importCalibration( NewCalibration, getChildNodes( childNode));

                UseCalibrations.setCalibration( CalibrationIndex++, NewCalibration);
            }
            else
            {
                if( debug) java.lang.System.out.println( nodeName);
            }
        }
    }

    private void importCalibration( Calibration UseCalibration, IterableNodeList childNodes)
    {
        for( Node childNode: childNodes)
        {
            String nodeName = childNode.getNodeName();
            String textContent = childNode.getTextContent();

            if( Names.CALIBRATION_HIGH.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( UseCalibration.getHigh(), textContent);
            }
            else if( Names.CALIBRATION_CENTER.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( UseCalibration.getCenter(), textContent);
            }
            else if( Names.CALIBRATION_LOW.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( UseCalibration.getLow(), textContent);
            }
            else
            {
                if( debug) java.lang.System.out.println( nodeName + " " + textContent);
            }
        }
    }

    private void importModels( Models models, IterableNodeList childNodes)
    {
        for( Node childNode: childNodes)
        {
            String nodeName = childNode.getNodeName();

            if( Names.MODEL.compareToIgnoreCase( nodeName) == 0)
            {
                Model model = new Model();

                importModel( model, getChildNodes( childNode));

                try
                {
                    models.addModel( model);
                }
                catch( Exception reason)
                {
                    java.lang.System.err.println( "Error adding model: " + reason.getMessage());
                }
            }
            else
            {
                if( debug) java.lang.System.out.println( nodeName);
            }
        }
    }

    private void importModel( Model model, IterableNodeList childNodes)
    {
        for( Node childNode: childNodes)
        {
            String nodeName = childNode.getNodeName();
            String textContent = childNode.getTextContent();

            if( Names.MODEL_ID.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( model.getId(), textContent);
            }
            else if( Names.MODEL_NAME.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( model.getName(), textContent);
            }
            else if( Names.MODEL_STATE.compareToIgnoreCase( nodeName) == 0)
            {
                model.setState( Utility.convertModelState( textContent));
            }
            else if( Names.MODEL_RF_MODE.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( model.getRfMode(), textContent);
            }
            else if( Names.MODEL_TYPE.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( model.getTypeId(), textContent);
            }
            else if( Names.MODEL_STATUS_SOURCES.compareToIgnoreCase( nodeName) == 0)
            {
                importStatusSources( model, getChildNodes( childNode));
            }
            else if( Names.MODEL_STATUS_TIMES.compareToIgnoreCase( nodeName) == 0)
            {
                importStatusTimes( model, getChildNodes( childNode));
            }
            else if( Names.CHANNELS.compareToIgnoreCase( nodeName) == 0)
            {
                importChannels( model.getChannels(), getChildNodes( childNode));
            }
            else if( Names.MODEL_PROXY_REFERENCES.compareToIgnoreCase( nodeName) == 0)
            {
                importProxySources( model.getProxyReferences(), getChildNodes( childNode));
            }
            else
            {
                if( debug) java.lang.System.out.println( nodeName + " " + textContent);
            }
        }
    }

    private void importStatusSources( Model model, IterableNodeList childNodes)
    {
        int statusSourceIndex = 0;

        for( Node childNode: childNodes)
        {
            String textContent = childNode.getTextContent();
            String nodeName = childNode.getNodeName();

            if( Names.MODEL_STATUS_SOURCE.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( model.getStatusSourceId( StatusSource.values()[ statusSourceIndex++]), textContent);
            }
            else
            {
                if( debug) java.lang.System.out.println( nodeName + " " + textContent);
            }
        }
    }

    private void importStatusTimes( Model model, IterableNodeList childNodes)
    {
        int statusTimeIndex = 0;

        for( Node childNode: childNodes)
        {
            String textContent = childNode.getTextContent();

            String nodeName = childNode.getNodeName();

            if( Names.MODEL_STATUS_TIME.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( model.getStatusTimeId( StatusTime.values()[ statusTimeIndex++]), textContent);
            }
            else
            {
                if( debug) java.lang.System.out.println( nodeName + " " + textContent);
            }
        }
    }

    private void importChannels( Channels channels, IterableNodeList childNodes)
    {
        int channelIndex = 0;

        for( Node childNode: childNodes)
        {
            String nodeName = childNode.getNodeName();

            if( Names.CHANNEL.compareToIgnoreCase( nodeName) == 0)
            {
                Channel channel = channels.addChannel( channelIndex++);

                importChannel( channel, getChildNodes( childNode));
            }
            else
            {
                if( debug) java.lang.System.out.println( nodeName);
            }
        }
    }

    private void importChannel( Channel UseChannel, IterableNodeList childNodes)
    {
        for( Node childNode: childNodes)
        {
            String nodeName = childNode.getNodeName();
            String textContent = childNode.getTextContent();

            if( Names.CHANNEL_NAME.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( UseChannel.getName(), textContent);
            }
            else if( Names.CHANNEL_INPUT.compareToIgnoreCase( nodeName) == 0)
            {
                importSourceTuple( UseChannel.getInput(), getChildNodes( childNode));
            }
            else if( Names.CHANNEL_TRIM.compareToIgnoreCase( nodeName) == 0)
            {
                importSourceTuple( UseChannel.getTrim(), getChildNodes( childNode));
            }
            else if( Names.CHANNEL_LIMIT.compareToIgnoreCase( nodeName) == 0)
            {
                importSourceTuple( UseChannel.getLimit(), getChildNodes( childNode));
            }
            else if( Names.CHANNEL_REVERSE.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( UseChannel.getReverse(), textContent);
            }
            else if( Names.CHANNEL_MODE.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( UseChannel.getMode(), textContent);
            }
            else if( Names.CHANNEL_POINTS.compareToIgnoreCase( nodeName) == 0)
            {
                importChannelPoints( UseChannel, getChildNodes( childNode));
            }
            else
            {
                if( debug) java.lang.System.out.println( nodeName + " " + textContent);
            }
        }
    }

    private void importChannelPoints( Channel UseChannel, IterableNodeList childNodes)
    {
        int channelPointIndex = 0;

        for( Node childNode: childNodes)
        {
            String nodeName = childNode.getNodeName();
            String textContent = childNode.getTextContent();

            if( Names.CHANNEL_POINT.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( UseChannel.getPoint( channelPointIndex++), textContent);
            }
            else
            {
                if( debug) java.lang.System.out.println( nodeName + " " + textContent);
            }
        }
    }

    private void importProxySources( ProxyReferences UseProxySources, IterableNodeList childNodes)
    {
        int proxyReferenceIndex = 0;

        for( Node childNode: childNodes)
        {
            String nodeName = childNode.getNodeName();
            String textContent = childNode.getTextContent();

            if( Names.MODEL_PROXY_REFERENCE.compareToIgnoreCase( nodeName) == 0)
            {
                importSourceTuple
                (
                    UseProxySources.getProxyReferenceFromIndex( proxyReferenceIndex++), getChildNodes( childNode)
                );
            }
            else
            {
                if( debug) java.lang.System.out.println( nodeName + " " + textContent);
            }
        }
    }

    private void importSourceTuple( SourceWithVolume sourceTuple, IterableNodeList childNodes)
    {
        for( Node childNode: childNodes)
        {
            String nodeName = childNode.getNodeName();
            String textContent = childNode.getTextContent();

            if( Names.SOURCE_TUPLE_SOURCE_ID.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( sourceTuple.getSourceId(), textContent);
            }
            else if( Names.SOURCE_TUPLE_VOLUME.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( sourceTuple.getVolume(), textContent);
            }
            else
            {
                if( debug) java.lang.System.out.println( nodeName + " " + textContent);
            }
        }
    }

    private void importTypes( Types types, IterableNodeList childNodes)
    {
        for( Node childNode: childNodes)
        {
            String nodeName = childNode.getNodeName();

            if( Names.TYPE.compareToIgnoreCase( nodeName) == 0)
            {
                Type type = new Type();

                importType( type, getChildNodes( childNode));

                types.addType( type);
            }
            else
            {
                if( debug) java.lang.System.out.println( nodeName);
            }
        }
    }

    private void importType( Type type, IterableNodeList childNodes)
    {
        for( Node childNode: childNodes)
        {
            String nodeName = childNode.getNodeName();
            String textContent = childNode.getTextContent();

            if( Names.TYPE_ID.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( type.getId(), textContent);
            }
            else if( Names.TYPE_NAME.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( type.getName(), textContent);
            }
            else if( Names.TYPE_STATE.compareToIgnoreCase( nodeName) == 0)
            {
                type.setState( Utility.convertTypeState( textContent));
            }
            else
            {
                if( debug) java.lang.System.out.println( nodeName + " " + textContent);
            }
        }
    }

    private void importSources( Sources sources, IterableNodeList childNodes)
    {
        for( Node childNode: childNodes)
        {
            String nodeName = childNode.getNodeName();
            Source source = importSource( childNode);

            if( source != null)
            {
                sources.addSource( source);
            }
            else
            {
                if( debug) java.lang.System.out.println( nodeName);
            }
        }
    }

    private Source importSource( Node childNode)
    {
        Source source = null;

        String nodeName = childNode.getNodeName();
        String textContent = childNode.getTextContent();

        if( Names.SOURCE_INPUT_ANALOG.compareToIgnoreCase( nodeName) == 0)
        {
            source = importSourceInputAnalog( new Analog(), getChildNodes( childNode));
        }
        else if( Names.SOURCE_INPUT_BUTTON.compareToIgnoreCase( nodeName) == 0)
        {
            source = importSourceInputButton( new Button(), getChildNodes( childNode));
        }
        else if( Names.SOURCE_INPUT_ROTARY.compareToIgnoreCase( nodeName) == 0)
        {
            source = importSourceInputRotary( new Rotary(), getChildNodes( childNode));
        }
        else if( Names.SOURCE_INPUT_SWITCH.compareToIgnoreCase( nodeName) == 0)
        {
            source = importSourceInputSwitch( new Switch(), getChildNodes( childNode));
        }
        else if( Names.SOURCE_INPUT_TICKER.compareToIgnoreCase( nodeName) == 0)
        {
            source = importSourceInputTicker( new Ticker(), getChildNodes( childNode));
        }
        else if( Names.SOURCE_MAP.compareToIgnoreCase( nodeName) == 0)
        {
            source = importSourceMap( new Map(), getChildNodes( childNode));
        }
        else if( Names.SOURCE_MIX.compareToIgnoreCase( nodeName) == 0)
        {
            source = importSourceMix( new Mix(), getChildNodes( childNode));
        }
        else if( Names.SOURCE_STORE.compareToIgnoreCase( nodeName) == 0)
        {
            source = importSourceStore( new Store(), getChildNodes( childNode));
        }
        else if( Names.SOURCE_FOLLOWER.compareToIgnoreCase( nodeName) == 0)
        {
            source = importSourceFollower( new Follower(), getChildNodes( childNode));
        }
        else if( Names.SOURCE_TIMER.compareToIgnoreCase( nodeName) == 0)
        {
            source = importSourceTimer( new Timer(), getChildNodes( childNode));
        }
        else if( Names.SOURCE_TRIMMER.compareToIgnoreCase( nodeName) == 0)
        {
            source = importSourceTrimmer( new Trimmer(), getChildNodes( childNode));
        }
        else if( Names.SOURCE_PROXY.compareToIgnoreCase( nodeName) == 0)
        {
            source = importSourceProxy( new Proxy(), getChildNodes( childNode));
        }
        else
        {
            if( debug) java.lang.System.out.println( nodeName + " " + textContent);
        }

        return source;
    }

    private boolean importSourceValue( Source source, String nodeName, String textContent)
    {
        if( Names.SOURCE_ID.compareToIgnoreCase( nodeName) == 0)
        {
            readValue( source.getId(), textContent);
        }
        else if( Names.SOURCE_NAME.compareToIgnoreCase( nodeName) == 0)
        {
            readValue( source.getName(), textContent);
        }
        else if( Names.SOURCE_MODEL.compareToIgnoreCase( nodeName) == 0)
        {
            readValue( source.getModelId(), textContent);
        }
        else
        {
            return false;
        }

        return true;
    }

    private Source importSourceInputAnalog( Analog source, IterableNodeList childNodes)
    {
        for( Node childNode: childNodes)
        {
            String nodeName = childNode.getNodeName();
            String textContent = childNode.getTextContent();

            if( Names.SOURCE_INPUT_ANALOG_INPUT.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( source.getInputId(), textContent);
            }
            else if( !importSourceValue( source, nodeName, textContent))
            {
                if( debug) java.lang.System.out.println( nodeName + " " + textContent);
            }
        }

        return source;
    }

    private Source importSourceInputButton( Button source, IterableNodeList childNodes)
    {
        for( Node childNode: childNodes)
        {
            String nodeName = childNode.getNodeName();
            String textContent = childNode.getTextContent();

            if( Names.SOURCE_INPUT_BUTTON_INPUT.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( source.getInputId(), textContent);
            }
            else if( Names.SOURCE_INPUT_BUTTON_INIT.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( source.getInit(), textContent);
            }
            else if( Names.SOURCE_INPUT_BUTTON_STORE.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( source.getStore(), textContent);
            }
            else if( Names.SOURCE_INPUT_BUTTON_TOGGLE.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( source.getToggle(), textContent);
            }
            else if( Names.SOURCE_INPUT_BUTTON_TOP.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( source.getTop(), textContent);
            }
            else if( Names.SOURCE_INPUT_BUTTON_BOTTOM.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( source.getBottom(), textContent);
            }
            else if( !importSourceValue( source, nodeName, textContent))
            {
                if( debug) java.lang.System.out.println( nodeName + " " + textContent);
            }
        }

        return source;
    }

    private Source importSourceInputRotary( Rotary source, IterableNodeList childNodes)
    {
        for( Node childNode: childNodes)
        {
            String nodeName = childNode.getNodeName();
            String textContent = childNode.getTextContent();

            if( Names.SOURCE_INPUT_ROTARY_A_INPUT.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( source.getAInputId(), textContent);
            }
            else if( Names.SOURCE_INPUT_ROTARY_B_INPUT.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( source.getBInputId(), textContent);
            }
            else if( Names.SOURCE_INPUT_ROTARY_STORE.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( source.getStore(), textContent);
            }
            else if( Names.SOURCE_INPUT_ROTARY_INIT.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( source.getInit(), textContent);
            }
            else if( Names.SOURCE_INPUT_ROTARY_STEP.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( source.getStep(), textContent);
            }
            else if( Names.SOURCE_INPUT_ROTARY_TOP.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( source.getTop(), textContent);
            }
            else if( Names.SOURCE_INPUT_ROTARY_BOTTOM.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( source.getBottom(), textContent);
            }
            else if( !importSourceValue( source, nodeName, textContent))
            {
                if( debug) java.lang.System.out.println( nodeName + " " + textContent);
            }
        }

        return source;
    }

    private Source importSourceInputSwitch( Switch source, IterableNodeList childNodes)
    {
        for( Node childNode: childNodes)
        {
            String nodeName = childNode.getNodeName();
            String textContent = childNode.getTextContent();

            if( Names.SOURCE_INPUT_SWITCH_LOW_INPUT.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( source.getLowInputId(), textContent);
            }
            else if( Names.SOURCE_INPUT_SWITCH_HIGH_INPUT.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( source.getHighInputId(), textContent);
            }
            else if( Names.SOURCE_INPUT_SWITCH_TOP.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( source.getTop(), textContent);
            }
            else if( Names.SOURCE_INPUT_SWITCH_BOTTOM.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( source.getBottom(), textContent);
            }
            else if( !importSourceValue( source, nodeName, textContent))
            {
                if( debug) java.lang.System.out.println( nodeName + " " + textContent);
            }
        }

        return source;
    }

    private Source importSourceInputTicker( Ticker source, IterableNodeList childNodes)
    {
        for( Node childNode: childNodes)
        {
            String nodeName = childNode.getNodeName();
            String textContent = childNode.getTextContent();

            if( Names.SOURCE_INPUT_TICKER_LOW_INPUT.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( source.getLowInputId(), textContent);
            }
            else if( Names.SOURCE_INPUT_TICKER_HIGH_INPUT.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( source.getHighInputId(), textContent);
            }
            else if( Names.SOURCE_INPUT_TICKER_INIT.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( source.getInit(), textContent);
            }
            else if( Names.SOURCE_INPUT_TICKER_STEP.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( source.getStep(), textContent);
            }
            else if( Names.SOURCE_INPUT_TICKER_STORE.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( source.getStore(), textContent);
            }
            else if( Names.SOURCE_INPUT_TICKER_TOP.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( source.getTop(), textContent);
            }
            else if( Names.SOURCE_INPUT_TICKER_BOTTOM.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( source.getBottom(), textContent);
            }
            else if( !importSourceValue( source, nodeName, textContent))
            {
                if( debug) java.lang.System.out.println( nodeName + " " + textContent);
            }
        }

        return source;
    }

    private Source importSourceMap( Map source, IterableNodeList childNodes)
    {
        for( Node childNode: childNodes)
        {
            String nodeName = childNode.getNodeName();
            String textContent = childNode.getTextContent();

            if( Names.SOURCE_MAP_INPUT.compareToIgnoreCase( nodeName) == 0)
            {
                importSourceTuple( source.getInput(), getChildNodes( childNode));
            }
            else if( Names.SOURCE_MAP_POINTS.compareToIgnoreCase( nodeName) == 0)
            {
                importSourceMapPoints( source, getChildNodes( childNode));
            }
            else if( !importSourceValue( source, nodeName, textContent))
            {
                if( debug) java.lang.System.out.println( nodeName + " " + textContent);
            }
        }

        return source;
    }

    private void importSourceMapPoints( Map source, IterableNodeList childNodes)
    {
        int pointIndex = 0;

        for( Node childNode: childNodes)
        {
            String textContent = childNode.getTextContent();
            String nodeName = childNode.getNodeName();

            if( Names.SOURCE_MAP_POINT.compareToIgnoreCase( nodeName) == 0)
            {
                importSourceTuple( source.getPoint( pointIndex++), getChildNodes( childNode));
            }
            else
            {
                if( debug) java.lang.System.out.println( nodeName + " " + textContent);
            }
        }
    }

    private Source importSourceMix( Mix source, IterableNodeList childNodes)
    {
        for( Node childNode: childNodes)
        {
            String nodeName = childNode.getNodeName();
            String textContent = childNode.getTextContent();

            if( Names.SOURCE_MIX_INPUTS.compareToIgnoreCase( nodeName) == 0)
            {
                importSourceMixInputs( source, getChildNodes( childNode));
            }
            else if( !importSourceValue( source, nodeName, textContent))
            {
                if( debug) java.lang.System.out.println( nodeName + " " + textContent);
            }
        }

        return source;
    }

    private void importSourceMixInputs( Mix source, IterableNodeList childNodes)
    {
        int inputIndex = 0;

        for( Node childNode: childNodes)
        {
            String textContent = childNode.getTextContent();
            String nodeName = childNode.getNodeName();

            if( Names.SOURCE_MIX_INPUT.compareToIgnoreCase( nodeName) == 0)
            {
                importSourceTuple( source.getInput( inputIndex++), getChildNodes( childNode));
            }
            else
            {
                if( debug) java.lang.System.out.println( nodeName + " " + textContent);
            }
        }
    }

    private Source importSourceStore( Store source, IterableNodeList childNodes)
    {
        for( Node childNode: childNodes)
        {
            String nodeName = childNode.getNodeName();
            String textContent = childNode.getTextContent();

            if( Names.SOURCE_STORE_INPUT.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( source.getInput(), textContent);
            }
            else if( Names.SOURCE_STORE_INIT.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( source.getInit(), textContent);
            }
            else if( !importSourceValue( source, nodeName, textContent))
            {
                if( debug) java.lang.System.out.println( nodeName + " " + textContent);
            }
        }

        return source;
    }

    private Source importSourceFollower( Follower source, IterableNodeList childNodes)
    {
        for( Node childNode: childNodes)
        {
            String nodeName = childNode.getNodeName();
            String textContent = childNode.getTextContent();

            if( Names.SOURCE_FOLLOWER_TARGET.compareToIgnoreCase( nodeName) == 0)
            {
                importSourceTuple( source.getTarget(), getChildNodes( childNode));
            }
            else if( Names.SOURCE_FOLLOWER_STEP.compareToIgnoreCase( nodeName) == 0)
            {
                importSourceTuple( source.getStep(), getChildNodes( childNode));
            }
            else if( Names.SOURCE_FOLLOWER_TRIGGER.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( source.getTrigger(), textContent);
            }
            else if( Names.SOURCE_FOLLOWER_TRIGGER_LOW_LIMIT.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( source.getTriggerLowLimit(), textContent);
            }
            else if( Names.SOURCE_FOLLOWER_TRIGGER_HIGH_LIMIT.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( source.getTriggerHighLimit(), textContent);
            }
            else if( !importSourceValue( source, nodeName, textContent))
            {
                if( debug) java.lang.System.out.println( nodeName + " " + textContent);
            }
        }

        return source;
    }

    private Source importSourceTimer( Timer source, IterableNodeList childNodes)
    {
        for( Node childNode: childNodes)
        {
            String nodeName = childNode.getNodeName();
            String textContent = childNode.getTextContent();

            if( Names.SOURCE_TIMER_INIT_TIME.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( source.getInitTime(), textContent);
            }
            else if( Names.SOURCE_TIMER_CURRENT_TIME.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( source.getCurrentTime(), textContent);
            }
            else if( Names.SOURCE_TIMER_REVERSE.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( source.getReverse(), textContent);
            }
            else if( Names.SOURCE_TIMER_STORE.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( source.getStore(), textContent);
            }
            else if( Names.SOURCE_TIMER_TRIGGER.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( source.getTrigger(), textContent);
            }
            else if( Names.SOURCE_TIMER_TRIGGER_LOW_LIMIT.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( source.getTriggerLowLimit(), textContent);
            }
            else if( Names.SOURCE_TIMER_TRIGGER_HIGH_LIMIT.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( source.getTriggerHighLimit(), textContent);
            }
            else if( Names.SOURCE_TIMER_WARN_LOW_TIME.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( source.getWarnLowTime(), textContent);
            }
            else if( Names.SOURCE_TIMER_WARN_CRITICAL_TIME.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( source.getWarnCriticalTime(), textContent);
            }
            else if( Names.SOURCE_TIMER_WARN_PAUSE_TIME.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( source.getWarnPauseTime(), textContent);
            }
            else if( !importSourceValue( source, nodeName, textContent))
            {
                if( debug) java.lang.System.out.println( nodeName + " " + textContent);
            }
        }

        return source;
    }

    private Source importSourceTrimmer(Trimmer source, IterableNodeList childNodes)
    {
        for( Node childNode: childNodes)
        {
            String nodeName = childNode.getNodeName();
            String textContent = childNode.getTextContent();

            if( Names.SOURCE_TRIMMER_INPUT_SOURCE.compareToIgnoreCase( nodeName) == 0)
            {
                importSourceTuple( source.getInput(), getChildNodes( childNode));
            }
            else if( Names.SOURCE_TRIMMER_TRIM_SOURCE.compareToIgnoreCase( nodeName) == 0)
            {
                importSourceTuple( source.getTrim(), getChildNodes( childNode));
            }
            else if( Names.SOURCE_TRIMMER_LIMIT_SOURCE.compareToIgnoreCase( nodeName) == 0)
            {
                importSourceTuple( source.getLimit(), getChildNodes( childNode));
            }
            else if( Names.SOURCE_TRIMMER_REVERSE.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( source.getReverse(), textContent);
            }
            else if( Names.SOURCE_TRIMMER_POINTS.compareToIgnoreCase( nodeName) == 0)
            {
                importSourceTrimmerPoints( source, getChildNodes( childNode));
            }
            else if( !importSourceValue( source, nodeName, textContent))
            {
                if( debug) java.lang.System.out.println( nodeName + " " + textContent);
            }
        }

        return source;
    }

    private void importSourceTrimmerPoints(Trimmer trim, IterableNodeList childNodes)
    {
        int pointIndex = 0;

        for( Node childNode: childNodes)
        {
            String textContent = childNode.getTextContent();
            String nodeName = childNode.getNodeName();

            if( Names.SOURCE_TRIMMER_POINT.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( trim.getPoint( pointIndex++), textContent);
            }
            else
            {
                if( debug) java.lang.System.out.println( nodeName + " " + textContent);
            }
        }
    }

    private Source importSourceProxy( Proxy source, IterableNodeList childNodes)
    {
        for( Node childNode: childNodes)
        {
            String nodeName = childNode.getNodeName();
            String textContent = childNode.getTextContent();

            if( Names.SOURCE_PROXY_SLOT.compareToIgnoreCase( nodeName) == 0)
            {
                readValue( source.getSlot(), textContent);
            }
            else if( !importSourceValue( source, nodeName, textContent))
            {
                if( debug) java.lang.System.out.println( nodeName + " " + textContent);
            }
        }

        return source;
    }

    private IterableNodeList getChildNodes( Node UseNode)
    {
        return new IterableNodeList( UseNode.getChildNodes());
    }

    private void readValue( Bool value, String textContent)
    {
        value.setConfigurationValue( textContent);
    }

    private void readValue( Text value, String textContent)
    {
        value.setConfigurationValue( textContent);
    }

    private void readValue( Number value, String textContent)
    {
        try
        {
            value.setConfigurationValue( textContent);
        }
        catch( NumberFormatException | ValueOutOfRangeException ignored) {}
    }
}
