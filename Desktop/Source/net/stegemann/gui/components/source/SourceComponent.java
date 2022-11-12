package net.stegemann.gui.components.source;

import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;

import javax.swing.JComboBox;

import net.stegemann.configuration.source.Source;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.ValueOutOfRangeException;
import net.stegemann.configuration.view.SourcesView;
import net.stegemann.gui.model.ListCellRenderer;
import net.stegemann.gui.model.SourcesComboBoxModel;

public class SourceComponent extends JComboBox< Source> implements FocusListener
{
	private static final long serialVersionUID = -1954851470198264943L;

	private SourcesView sourcesView;
	private Number sourceId;

	public SourceComponent()
	{
		setMinimumSize( new Dimension( 150, -1));

		addActionListener( this);
		addFocusListener( this);

		setRenderer( new ListCellRenderer<Source>());
	}

	public void setSourcesView( SourcesView UseSourcesView)
	{
		this.sourcesView = UseSourcesView;

		setModel( new SourcesComboBoxModel( UseSourcesView));
	}

	public void attachValue( Number SourceId)
	{
		this.sourceId = SourceId;

		setSelectedIndex( sourcesView.getSourceIndexFromId( SourceId));
	}

	private void setFromComboBox()
	{
		try
		{
			sourceId.setValue( sourcesView.getSourceIdFromIndex( getSelectedIndex()));
		}
		catch( ValueOutOfRangeException reason)
		{
			// Ignore bad input and revert to old.
		}

		setSelectedIndex( sourcesView.getSourceIndexFromId( sourceId));
	}

	@Override
	public void actionPerformed( ActionEvent e)
	{
		setFromComboBox();
	}

	@Override
	public void focusGained( FocusEvent e)
	{
	}

	@Override
	public void focusLost( FocusEvent e)
	{
		setFromComboBox();
	}
}
