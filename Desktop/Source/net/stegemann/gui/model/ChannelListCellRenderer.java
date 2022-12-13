package net.stegemann.gui.model;

import net.stegemann.configuration.Channel;

import javax.swing.*;
import java.awt.*;
import java.io.Serial;

public class ChannelListCellRenderer extends DefaultListCellRenderer
{
	@Serial
	private static final long serialVersionUID = -7591672497610614495L;

	@Override
   public Component getListCellRendererComponent( JList< ?> list, Object value, int index,
												  boolean isSelected, boolean cellHasFocus)
	{
		String textValue = (( Channel) value).getName().getValue();

		return super.getListCellRendererComponent( list, textValue, index, isSelected, cellHasFocus);
	}	
}
