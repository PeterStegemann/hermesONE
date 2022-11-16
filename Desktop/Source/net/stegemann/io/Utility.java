package net.stegemann.io;

import net.stegemann.configuration.Model;
import net.stegemann.configuration.Type;
import net.stegemann.configuration.Model.RFMode;
import net.stegemann.configuration.Type.State;

public class Utility
{
	public static Model.State convertModelState( String stateString)
	{
		return convertModelState( Integer.parseInt( stateString));
	}

	/** Convert the model state number to the model state enum.
	 * 
	 * @param stateValue The state as numeric value.
	 * 
	 * @return The state as enum.
	 */
	private static Model.State convertModelState( int stateValue)
	{
		try
		{
			return Model.State.values()[ stateValue];
		}
		catch( ArrayIndexOutOfBoundsException reason)
		{
			// Map bad values to empty.
			return Model.State.EMPTY;
		}
	}

	public static int convertModelState( Model.State state)
	{
		return state.ordinal();
	}

	public static Type.State convertTypeState( String stateString)
	{
		return convertTypeState( Integer.parseInt( stateString));
	}

	/** Convert the type state number to the type state enum.
	 * 
	 * @param stateValue The state as numeric value.
	 * 
	 * @return The state as enum.
	 */
	private static Type.State convertTypeState( int stateValue)
	{	
		try
		{
			return Type.State.values()[ stateValue];
		}
		catch( ArrayIndexOutOfBoundsException reason)
		{
			// Map bad values to empty.
			return Type.State.EMPTY;
		}
	}

	public static int convertTypeState( State state)
	{
		return state.ordinal();
	}

	@SuppressWarnings("unused")
	public static RFMode convertModelRFMode( String rfModeString)
	{
		return convertModelRFMode( Integer.parseInt( rfModeString));
	}

	/** Convert the model rf mode number to the model rf mode enum.
	 * 
	 * @param rfModeValue The rf mode as numeric value.
	 * 
	 * @return The rf mode as enum.
	 */
	private static Model.RFMode convertModelRFMode( int rfModeValue)
	{
		try
		{
			return Model.RFMode.values()[ rfModeValue];
		}
		catch( ArrayIndexOutOfBoundsException reason)
		{
			// Map bad values to none.
			return Model.RFMode.NONE;
		}
	}

	@SuppressWarnings("unused")
	public static int convertModelRFMode( RFMode rfMode)
	{
		return rfMode.ordinal();
	}
/*
	public static Source.Type convertSourceType( String typeString)
	{
		return convertSourceType( Integer.parseInt( typeString));
	}
*/
	/** Convert the source type number to the source type enum.
	 * 
	 * @param TypeValue The type as numeric value.
	 * 
	 * @return The type as enum.
	 */
/*	private static Source.Type convertSourceType( int typeValue)
	{
		return Source.Type.values()[ typeValue];
	}

	public static int convertSourceType( Source.Type type)
	{
		return type.ordinal();
	}
*/
}
