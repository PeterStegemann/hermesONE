package net.stegemann.gui.model;

import java.awt.Component;

import javax.swing.DefaultListCellRenderer;
import javax.swing.JList;

import net.stegemann.configuration.Named;

public class ListCellRenderer<T extends Named> extends DefaultListCellRenderer
{
	private static final long serialVersionUID = -2225391241430953553L;

	@Override
   public Component getListCellRendererComponent( JList<?> List, Object Value, int Index,
   	boolean IsSelected, boolean CellHasFocus)
	{
		@SuppressWarnings( "unchecked")
		String TextValue = Value == null ? null : (( T) Value).getName().getValue();

		return super.getListCellRendererComponent( List, TextValue, Index, IsSelected, CellHasFocus);
	}	
}
