package net.stegemann.io.serial.base;

public class TypedProtocol
{
	public static final byte T_Command		= 0;	// byte Value
	public static final byte T_State		= 1;	// byte Value
	public static final byte T_Value		= 2;	// byte[] Characters, byte 0
	public static final byte T_Complex		= 3;	// ..., T_ComplexEnd
	public static final byte T_ComplexEnd	= 4;
}
