package net.stegemann.gui.components;

import net.stegemann.configuration.type.Number;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.io.Serial;

public class RFModeComponent extends JComboBox< String> implements FocusListener
{
	@Serial
	private static final long serialVersionUID = -4858016417587291366L;

	private static final String[] rfModeNames =
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
		catch( Exception ignored)
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
