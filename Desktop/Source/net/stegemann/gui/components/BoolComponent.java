package net.stegemann.gui.components;

import net.stegemann.configuration.type.Bool;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.io.Serial;

public class BoolComponent extends JCheckBox implements ActionListener, FocusListener
{
	@Serial
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
