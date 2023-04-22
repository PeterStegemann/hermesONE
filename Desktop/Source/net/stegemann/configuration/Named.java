package net.stegemann.configuration;

import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.Text;

public interface Named
{
	Number getId();
	Text getName();
	Number getModel();
}
