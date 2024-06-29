package net.stegemann.configuration.source;

import lombok.Getter;
import net.stegemann.configuration.Configuration;import net.stegemann.configuration.Signal;
import net.stegemann.configuration.type.SourceId;
import net.stegemann.configuration.type.SourceWithVolume;
import net.stegemann.configuration.type.ValueOutOfRangeException;
import net.stegemann.configuration.util.ConfigurationField;
import net.stegemann.io.xml.Names;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

@Getter
@ConfigurationField( name = Names.SOURCE_MAP)
public final class Map extends Source
{
    public static final int INPUT_SIGNAL_PER_VALUE = ( Signal.VALUE_RANGE / 600);
    public static final int POINT_SIGNAL_PER_VALUE = ( Signal.VALUE_RANGE / 200);

    public static final int POINTS = 7;

    private static final int DEFAULT_INPUT_VOLUME = 100;
    private static final int DEFAULT_POINT_VOLUME = 0;

    @ConfigurationField( name = Names.SOURCE_MAP_INPUT)
    private final SourceWithVolume input;
    @ConfigurationField( name = Names.SOURCE_MAP_POINTS, itemName = Names.SOURCE_MAP_POINT)
    private final List< SourceWithVolume> points = new ArrayList<>();

    public Map()
    {
        try
        {
            input = new SourceWithVolume( INPUT_SIGNAL_PER_VALUE, DEFAULT_INPUT_VOLUME);

            for( int count = 0; count < POINTS; count++)
            {
                points.add( new SourceWithVolume( POINT_SIGNAL_PER_VALUE, DEFAULT_POINT_VOLUME));
            }
        }
        catch( ValueOutOfRangeException reason)
        {
            throw new RuntimeException( reason);
        }
    }

    public Map( Map other)
    {
        super( other);

        input = new SourceWithVolume( other.input);

        for( SourceWithVolume otherPoint: other.points)
        {
            points.add( new SourceWithVolume( otherPoint));
        }
    }

    public SourceWithVolume getPoint( int index)
    {
        return points.get( index);
    }

    @Override
    public Source duplicate()
    {
        return new Map( this);
    }

    @Override
    public void replaceSources( HashMap< SourceId, SourceId> sourcesMap)
    {
        input.replaceSource( sourcesMap);

        points.forEach( point -> point.replaceSource( sourcesMap));
    }

    @Override
    public void switchSources( SourceId sourceIdOne, SourceId sourceIdTwo)
    {
        input.switchSource( sourceIdOne, sourceIdTwo);

        points.forEach( point -> point.switchSource( sourceIdOne, sourceIdTwo));
    }

    @Override
    public boolean validate( Configuration configuration)
    {
        return
            super.validate( configuration) &&
            validateReferencedSource( configuration, input,"input") &&
            validateReferencedSources( configuration, points,"points");
    }

    @Override
    public String toString()
    {
        StringBuffer Buffer = new StringBuffer();

        Buffer.append( "Map = {\n");
        Buffer.append( super.toString());
        Buffer.append( " Input: " + input + "\n");

        Buffer.append( " Points = {\n");

        for( SourceWithVolume point: points)
        {
            Buffer.append( point);
        }

        Buffer.append( " }\n");
        Buffer.append( "}\n");

        return Buffer.toString();
    }
}
