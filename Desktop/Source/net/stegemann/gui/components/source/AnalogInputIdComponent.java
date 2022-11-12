package net.stegemann.gui.components.source;

public class AnalogInputIdComponent extends InputIdComponent
{
	private static final long serialVersionUID = 3783810106262477386L;

	private static String[] analogInputNames =
	{
		"A0",
		"A1",
		"B0",
		"B1",
		"C0",
		"C1"
	};

	public AnalogInputIdComponent()
	{
		super( analogInputNames);
	}
}
