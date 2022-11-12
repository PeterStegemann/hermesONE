package net.stegemann.gui.model;

import javax.swing.AbstractListModel;
import javax.swing.ComboBoxModel;

import net.stegemann.configuration.Model;
import net.stegemann.configuration.view.ModelsView;
import net.stegemann.misc.ChangeListener;

public class ModelsViewComboBoxModel extends AbstractListModel< Model>
											 implements ComboBoxModel< Model>, ChangeListener< ModelsView>
{
	private static final long serialVersionUID = 1088277604809645591L;

	private final ModelsView modelsView;
	private Object selectedItem = null;

	public ModelsViewComboBoxModel( ModelsView modelsView)
	{
		this.modelsView = modelsView;

		modelsView.addChangeListener( this);
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
		return modelsView.getModelFromIndex( index);
	}

	@Override
	public int getSize()
	{
		return modelsView.getModelCount();
	}

	@Override
	public void hasChanged( ModelsView object)
	{
		fireContentsChanged( this, 0, getSize() - 1);
	}
}
