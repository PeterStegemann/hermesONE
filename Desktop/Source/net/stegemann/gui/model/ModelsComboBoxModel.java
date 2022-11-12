package net.stegemann.gui.model;

import javax.swing.AbstractListModel;
import javax.swing.ComboBoxModel;

import net.stegemann.configuration.Model;
import net.stegemann.configuration.Models;
import net.stegemann.misc.ChangeListener;

public class ModelsComboBoxModel extends AbstractListModel< Model>
										implements ComboBoxModel< Model>, ChangeListener< Models>
{
	private static final long serialVersionUID = 1088277604809645591L;

	private final Models models;
	private Object selectedItem = null;

	public ModelsComboBoxModel( Models models)
	{
		this.models = models;

		models.addChangeListener( this);
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
