package net.stegemann.io;

import net.stegemann.configuration.Model;
import net.stegemann.configuration.Type;
import net.stegemann.configuration.Model.RFMode;
import net.stegemann.configuration.Type.State;

public class Utility
{
	public static Model.State ConvertModelState( String StateString)
	{
		return ConvertModelState( Integer.parseInt( StateString));
	}

	/** Convert the model state number to the model state enum.
	 * 
	 * @param StateValue The state as numeric value.
	 * 
	 * @return The state as enum.
	 */
	private static Model.State ConvertModelState( int StateValue)
	{
		try
		{
			return Model.State.values()[ StateValue];
		}
		catch( ArrayIndexOutOfBoundsException Reason)
		{
			// Map bad values to empty.
			return Model.State.EMPTY;
		}
	}

	public static int ConvertModelState( Model.State UseState)
	{
		return UseState.ordinal();
	}

	public static Type.State ConvertTypeState( String StateString)
	{
		return ConvertTypeState( Integer.parseInt( StateString));
	}

	/** Convert the type state number to the type state enum.
	 * 
	 * @param StateValue The state as numeric value.
	 * 
	 * @return The state as enum.
	 */
	private static Type.State ConvertTypeState( int StateValue)
	{	
		try
		{
			return Type.State.values()[ StateValue];
		}
		catch( ArrayIndexOutOfBoundsException Reason)
		{
			// Map bad values to empty.
			return Type.State.EMPTY;
		}
	}

	public static int ConvertTypeState( State UseState)
	{
		return UseState.ordinal();
	}

	public static RFMode ConvertModelRFMode( String RFModeString)
	{
		return ConvertModelRFMode( Integer.parseInt( RFModeString));
	}

	/** Convert the model rf mode number to the model rf mode enum.
	 * 
	 * @param RFModeValue The rf mode as numeric value.
	 * 
	 * @return The rf mode as enum.
	 */
	private static Model.RFMode ConvertModelRFMode( int RFModeValue)
	{
		try
		{
			return Model.RFMode.values()[ RFModeValue];
		}
		catch( ArrayIndexOutOfBoundsException Reason)
		{
			// Map bad values to none.
			return Model.RFMode.NONE;
		}
	}

	public static int ConvertModelRFMode( RFMode UseRFMode)
	{
		return UseRFMode.ordinal();
	}

/*
	public static Source.Type ConvertSourceType( String TypeString)
	{
		return ConvertSourceType( Integer.parseInt( TypeString));
	}
*/
	/** Convert the source type number to the source type enum.
	 * 
	 * @param TypeValue The type as numeric value.
	 * 
	 * @return The type as enum.
	 */
/*	private static Source.Type ConvertSourceType( int TypeValue)
	{
		return Source.Type.values()[ TypeValue];
	}

	public static int ConvertSourceType( Source.Type UseType)
	{
		return UseType.ordinal();
	}
*/
}
