package net.stegemann.gui.components;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;

import javax.swing.JCheckBox;

import net.stegemann.configuration.type.Bool;

public class BoolComponent extends JCheckBox implements ActionListener, FocusListener
{
	private static final long serialVersionUID = -5648128911287846276L;

	private Bool value;

	public BoolComponent()
	{
		addActionListener( this);
		addFocusListener( this);
	}

	public void attachValue( Bool value)
	{
		this.value = value;

		setSelected( value.getValue());
	}

	private void setFromCheckBox()
	{
		value.setValue( isSelected());
	}

	@Override
	public void actionPerformed( ActionEvent e)
	{
		setFromCheckBox();
	}

	@Override
	public void focusGained( FocusEvent e)
	{
	}

	@Override
	public void focusLost( FocusEvent e)
	{
		setFromCheckBox();
	}
}
