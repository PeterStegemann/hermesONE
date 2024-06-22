package net.stegemann.configuration.type;

import net.stegemann.configuration.Model;

import static net.stegemann.misc.Utility.indent;

public class ModelId extends Number
{
    public ModelId()
    {
        this( Model.MODEL_START, Model.MODEL_NONE);
    }

    public ModelId( int minimum, int maximum)
    {
        super( minimum, maximum);
    }

    public ModelId( int value)
        throws ValueOutOfRangeException
    {
        this( Model.MODEL_START, Model.MODEL_NONE, value);
    }

    public ModelId( int minimum, int maximum, int value)
        throws ValueOutOfRangeException
    {
        super( minimum, maximum, value);
    }

    public ModelId( ModelId other)
    {
        super( other);
    }

    public void switchModel( ModelId modelIdOne, ModelId modelIdTwo)
    {
        try
        {
            if( this.equals( modelIdOne))
            {
                setValue( modelIdTwo);
            }
            else if( this.equals( modelIdTwo))
            {
                setValue( modelIdOne);
            }
        }
        catch( ValueOutOfRangeException reason)
        {
            throw new RuntimeException( reason);
        }
    }

    @Override
    public String toString()
    {
        return String.format
        (
            """
            ModelId
            {
                %s
            }
            """,
            indent( super.toString())
        );
    }
}
