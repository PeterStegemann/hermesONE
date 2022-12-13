package net.stegemann.gui.model;

import net.stegemann.configuration.Model;
import net.stegemann.configuration.Models;
import net.stegemann.misc.ChangeListener;

import javax.swing.*;
import java.io.Serial;

public class ModelsComboBoxModel extends AbstractListModel< Model>
							  implements ComboBoxModel< Model>, ChangeListener< Models>
{
	@Serial
	private static final long serialVersionUID = 1088277604809645591L;

	private final Models models;
	private Object selectedItem = null;

	public ModelsComboBoxModel( Models useModels)
	{
		models = useModels;

		useModels.addChangeListener( this);
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
	public Model getElementAt( int index)
	{
		return models.getModelFromIndex( index);
	}

	@Override
	public int getSize()
	{
		return models.getCount();
	}

	@Override
	public void hasChanged( Models object)
	{
		fireContentsChanged( this, 0, getSize() - 1);
	}
}
