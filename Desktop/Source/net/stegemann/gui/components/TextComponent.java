package net.stegemann.gui.components;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

import javax.swing.JTextField;

import net.stegemann.configuration.type.Text;

public class TextComponent extends JTextField implements ActionListener, FocusListener, KeyListener
{
	private static final long serialVersionUID = -3194040846647409327L;

	private Text value;

	public TextComponent( int columns)
	{
		super( columns);

		setEnabled( false);

		setHorizontalAlignment( JTextField.LEFT);
		addActionListener( this);
		addFocusListener( this);
		addKeyListener( this);
	}

	public void attachValue( Text value)
	{
		this.value = value;

		if( value != null)
		{
			setText( value.getValue());

			setEnabled( true);
		}
		else
		{
			setText( "");

			setEnabled( false);
		}
	}

	private void setFromText()
	{
		value.setValue( getText());
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

	@Override
	public void keyPressed( KeyEvent e)
	{
	}

	@Override
	public void keyReleased( KeyEvent e)
	{
		// Give us live update!
		setFromText();
	}

	@Override
	public void keyTyped( KeyEvent e)
	{
	}
}
