package net.stegemann.gui.components;

import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;

import javax.swing.JComboBox;

import net.stegemann.configuration.type.Number;

public class RFModeComponent extends JComboBox< String> implements FocusListener
{
	private static final long serialVersionUID = -4858016417587291366L;

	private static String[] rfModeNames =
	{
		"Keins",
		"Modul 1",
		"Modul 2",
		"Beide"
	};

	private Number rfMode;

	public RFModeComponent()
	{
		super( rfModeNames);

		setMinimumSize( new Dimension( 150, -1));

		addActionListener( this);
		addFocusListener( this);
	}

	public void attachValue( Number UseRFMode)
	{
		rfMode = UseRFMode;

		setSelectedIndex( rfMode.getValue());
	}

	private void setFromComboBox()
	{
		try
		{
			rfMode.setValue( getSelectedIndex());
		}
		catch( Exception Reason)
		{
			// Ignore bad input and revert to old.
		}

		setSelectedIndex( rfMode.getValue());
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
