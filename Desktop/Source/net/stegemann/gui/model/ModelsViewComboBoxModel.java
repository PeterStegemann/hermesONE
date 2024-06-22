package net.stegemann.gui.model;

import net.stegemann.configuration.Model;
import net.stegemann.configuration.view.ModelsView;
import net.stegemann.misc.ChangeListener;

import javax.swing.*;
import java.io.Serial;

public class ModelsViewComboBoxModel extends AbstractListModel< Model>
								  implements ComboBoxModel< Model>, ChangeListener< ModelsView>
{
	@Serial
	private static final long serialVersionUID = 1088277604809645591L;

	private final ModelsView modelsView;
	private Object selectedItem = null;

	public ModelsViewComboBoxModel( ModelsView useModelsView)
	{
		modelsView = useModelsView;

		useModelsView.addChangeListener( this);
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
		return modelsView.getModelFromIndex( index);
	}

	@Override
	public int getSize()
	{
		return modelsView.getCount();
	}

	@Override
	public void hasChanged( ModelsView object)
	{
		fireContentsChanged( this, 0, getSize() - 1);
	}
}
