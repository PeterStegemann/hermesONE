package net.stegemann.gui.components;

import net.stegemann.configuration.type.Number;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;

public class NumberComponent extends JTextField implements ActionListener, FocusListener
{
	private static final long serialVersionUID = 8618769729240169430L;

	private Number value;

	public NumberComponent( int columns)
	{
		super( columns);

		setHorizontalAlignment( JTextField.RIGHT);
		addActionListener( this);
		addFocusListener( this);
	}

	public void attachValue( Number useValue)
	{
		value = useValue;

		setText( Integer.toString( value.getValue()));
	}

	private void setFromText()
	{
		try
		{
			value.setValue( Integer.parseInt( getText()));
		}
		catch( Exception ignored)
		{
			// Ignore bad input and revert to old.
		}

		setText( Integer.toString( value.getValue()));
	}

	@Override
	public void actionPerformed( ActionEvent e)
	{
		setFromText();
	}

	@Override
	public void focusGained( FocusEvent e)
	{
	}

	@Override
	public void focusLost( FocusEvent e)
	{
		setFromText();
	}
}
