package net.stegemann.gui.model;

import javax.swing.AbstractListModel;
import javax.swing.ComboBoxModel;

import net.stegemann.configuration.Type;
import net.stegemann.configuration.Types;
import net.stegemann.misc.ChangeListener;

public class TypesComboBoxModel extends AbstractListModel< Type>
									  implements ComboBoxModel< Type>, ChangeListener< Types>
{
	private static final long serialVersionUID = -6662717312228924299L;

	private final Types types;
	private Object selectedItem = null;

	public TypesComboBoxModel( Types types)
	{
		this.types = types;

		types.addChangeListener( this);
	}

	@Override
	public Object getSelectedItem()
	{
		return selectedItem;
	}

	@Override
	public void setSelectedItem( Object selectedItem)
	{
		this.selectedItem = selectedItem;
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
		System.out.println("TypesComboBoxModel.hasChanged: " + object);

		fireContentsChanged( this, 0, getSize() - 1);
	}
}
