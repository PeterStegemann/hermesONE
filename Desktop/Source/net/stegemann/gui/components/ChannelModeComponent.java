package net.stegemann.gui.components;

import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;

import javax.swing.JComboBox;

import net.stegemann.configuration.type.Number;

public class ChannelModeComponent extends JComboBox< String> implements FocusListener
{
	private static final long serialVersionUID = -989093797363852764L;

	private static String[] CHANNEL_MODES =
	{
		"Kurve",
		"Abschneiden"
	};

	private Number mode;

	public ChannelModeComponent()
	{
		super( CHANNEL_MODES);

		setMinimumSize( new Dimension( 150, -1));

		addActionListener( this);
		addFocusListener( this);
	}

	public void attachValue( Number UseMode)
	{
		mode = UseMode;

		setSelectedIndex( mode.getValue());
	}

	private void setFromComboBox()
	{
		try
		{
			mode.setValue( getSelectedIndex());
		}
		catch( Exception Reason)
		{
			// Ignore bad input and revert to old.
		}

		setSelectedIndex( mode.getValue());
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
