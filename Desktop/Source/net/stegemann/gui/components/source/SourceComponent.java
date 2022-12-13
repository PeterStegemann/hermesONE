package net.stegemann.gui.components.source;

import net.stegemann.configuration.source.Source;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.ValueOutOfRangeException;
import net.stegemann.configuration.view.SourcesView;
import net.stegemann.gui.model.ListCellRenderer;
import net.stegemann.gui.model.SourcesComboBoxModel;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.io.Serial;

public class SourceComponent extends JComboBox< Source> implements FocusListener
{
	@Serial
	private static final long serialVersionUID = -1954851470198264943L;

	private SourcesView sourcesView;
	private Number sourceId;

	public SourceComponent()
	{
		setMinimumSize( new Dimension( 150, -1));

		addActionListener( this);
		addFocusListener( this);

		setRenderer( new ListCellRenderer< Source>());
	}

	public void setSourcesView( SourcesView useSourcesView)
	{
		sourcesView = useSourcesView;

		setModel( new SourcesComboBoxModel( useSourcesView));
	}

	public void attachValue( Number useSourceId)
	{
		sourceId = useSourceId;

		setSelectedIndex( sourcesView.getSourceIndexFromId( useSourceId));
	}

	private void setFromComboBox()
	{
		try
		{
			sourceId.setValue( sourcesView.getSourceIdFromIndex( getSelectedIndex()));
		}
		catch( ValueOutOfRangeException ignored)
		{
			// Ignore bad input and revert to old.
		}

		setSelectedIndex( sourcesView.getSourceIndexFromId( sourceId));
	}

	@Override
	public void actionPerformed( ActionEvent event)
	{
		setFromComboBox();
	}

	@Override
	public void focusGained( FocusEvent event)
	{
	}

	@Override
	public void focusLost( FocusEvent event)
	{
		setFromComboBox();
	}
}
