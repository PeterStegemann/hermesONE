package net.stegemann.gui.model;

import net.stegemann.configuration.source.Source;
import net.stegemann.configuration.view.SourcesView;
import net.stegemann.misc.ChangeListener;

import javax.swing.*;
import java.io.Serial;

public class SourcesComboBoxModel extends AbstractListModel< Source>
                               implements ComboBoxModel< Source>, ChangeListener< SourcesView>
{
	@Serial
	private static final long serialVersionUID = 6829553793468551506L;

	private final SourcesView sourcesView;
	private Object selectedItem = null;

	public SourcesComboBoxModel( SourcesView useSourcesView)
	{
		sourcesView = useSourcesView;

		sourcesView.addChangeListener( this);
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
	public Source getElementAt( int index)
	{
		return sourcesView.getSourceFromIndex( index);
	}

	@Override
	public int getSize()
	{
		return sourcesView.getSourceCount();
	}

	@Override
	public void hasChanged( SourcesView object)
	{
		fireContentsChanged( this, 0, getSize() - 1);
	}
}
