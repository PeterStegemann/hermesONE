package net.stegemann.gui.model;

import java.awt.Component;

import javax.swing.DefaultListCellRenderer;
import javax.swing.JList;

import net.stegemann.configuration.Channel;

public class ChannelListCellRenderer extends DefaultListCellRenderer
{
	private static final long serialVersionUID = -7591672497610614495L;

	@Override
   public Component getListCellRendererComponent( JList<?> List, Object Value, int Index,
   	boolean IsSelected, boolean CellHasFocus)
	{
		String TextValue = (( Channel) Value).getName().getValue();

		return super.getListCellRendererComponent( List, TextValue, Index, IsSelected, CellHasFocus);
	}	
}
