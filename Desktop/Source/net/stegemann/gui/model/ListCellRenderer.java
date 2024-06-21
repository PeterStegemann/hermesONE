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
	public Component getListCellRendererComponent
	(
        JList< ?> list, Object value, int index, boolean isSelected, boolean cellHasFocus
    )
	{
		@SuppressWarnings( "unchecked")
		T namedValue = ( T) value;

		String textValue = value == null ? null : namedValue.getName().getValue();
//		int idValue = value == null ? -1 : namedValue.getId().getValue();
//		int modelValue = value == null ? -1 : namedValue.getModelId().getValue();

//		textValue = textValue + " (" + modelValue + "/" + idValue + ")";

		return super.getListCellRendererComponent( list, textValue, index, isSelected, cellHasFocus);
	}
}
