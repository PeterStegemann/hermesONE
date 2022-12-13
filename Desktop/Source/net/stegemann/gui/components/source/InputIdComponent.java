package net.stegemann.gui.components.source;

import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.ValueOutOfRangeException;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.io.Serial;

public class InputIdComponent extends JComboBox< String> implements FocusListener
{
	@Serial
	private static final long serialVersionUID = -1954851470198264943L;

	private Number inputId;

	public InputIdComponent( String[] names)
	{
		super( names);

		addActionListener( this);
		addFocusListener( this);
	}

	public void attachValue( Number inputId)
	{
		this.inputId = inputId;

		setSelectedIndex( inputId.getValue());
	}

	private void setFromComboBox()
	{
		try
		{
			inputId.setValue( getSelectedIndex());
		}
		catch( ValueOutOfRangeException ignored)
		{
			// Ignore bad input and revert to old.
		}

		setSelectedIndex( inputId.getValue());
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
