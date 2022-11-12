package net.stegemann.gui.components.source;

import java.awt.event.ActionEvent;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;

import javax.swing.JComboBox;

import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.ValueOutOfRangeException;

public class InputIdComponent extends JComboBox< String> implements FocusListener
{
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
		catch( ValueOutOfRangeException reason)
		{
			// Ignore bad input and revert to old.
		}

		setSelectedIndex( inputId.getValue());
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
