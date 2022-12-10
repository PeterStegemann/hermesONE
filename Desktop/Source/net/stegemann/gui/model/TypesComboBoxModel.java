package net.stegemann.gui.model;

import net.stegemann.configuration.Type;
import net.stegemann.configuration.Types;
import net.stegemann.misc.ChangeListener;

import javax.swing.*;
import java.io.Serial;

public class TypesComboBoxModel extends AbstractListModel< Type>
                             implements ComboBoxModel< Type>, ChangeListener< Types>
{
	@Serial
	private static final long serialVersionUID = -6662717312228924299L;

	private final Types types;
	private Object selectedItem = null;

	public TypesComboBoxModel( Types useTypes)
	{
		types = useTypes;

		useTypes.addChangeListener( this);
	}

	@Override
	public Object getSelectedItem()
	{
		return selectedItem;
	}

	@Override
	public void setSelectedItem( Object useSelectedItem)
	{
		selectedItem = useSelectedItem;
	}

	@Override
	public Type getElementAt( int index)
	{
		return types.getTypeFromIndex( index);
	}

	@Override
	public int getSize()
	{
		return types.getCount();
	}

	@Override
	public void hasChanged( Types object)
	{
		fireContentsChanged( this, 0, getSize() - 1);
	}
}
