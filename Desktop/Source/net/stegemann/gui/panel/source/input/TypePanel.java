package net.stegemann.gui.panel.source.input;

import net.stegemann.configuration.Configuration;
import net.stegemann.gui.panel.source.SpecificSourcePanel;

public class TypePanel extends SpecificSourcePanel
{
	private static final long serialVersionUID = 7183490640539971112L;

	public TypePanel( Configuration UseConfiguration)
	{
		super( UseConfiguration);
	}
	
	public void set()
	{
		setVisible( true);
	}
}
