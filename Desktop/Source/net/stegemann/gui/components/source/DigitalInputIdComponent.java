package net.stegemann.gui.components.source;

import java.io.Serial;

public class DigitalInputIdComponent extends InputIdComponent
{
	@Serial
	private static final long serialVersionUID = 4853364146846291274L;

	private static final String[] digitalInputNames =
	{
		"H0",
		"H1",
		"H2",
		"H3",
		"I0",
		"I1",
		"I2",
		"I3",
		"J0",
		"J1",
		"J2",
		"J3",
		"K0",
		"K1",
		"K2",
		"K3",
		"L0",
		"L1",
		"L2",
		"L3"
	};

	public DigitalInputIdComponent()
	{
		super( digitalInputNames);
	}
}
