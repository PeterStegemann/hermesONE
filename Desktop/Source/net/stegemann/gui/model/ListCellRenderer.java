package net.stegemann.gui.model;

import net.stegemann.configuration.Named;

import javax.swing.*;
import java.awt.*;
import java.io.Serial;

public class ListCellRenderer< T extends Named> extends DefaultListCellRenderer
{
	@Serial
	private static final long serialVersionUID = -2225391241430953553L;

	@Override
	public Component getListCellRendererComponent( JList<?> list, Object value, int index,
												   boolean isSelected, boolean cellHasFocus)
	{
		@SuppressWarnings( "unchecked")
		String textValue = value == null ? null : (( T) value).getName().getValue();
		int idValue = value == null ? -1 : (( T) value).getId().getValue();

		return super.getListCellRendererComponent( list, textValue + " (" + idValue + ")", index,
												   isSelected, cellHasFocus);
	}
}
