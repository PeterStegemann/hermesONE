package net.stegemann.gui.components;

import net.stegemann.configuration.Type;
import net.stegemann.configuration.Types;
import net.stegemann.configuration.type.Number;
import net.stegemann.gui.model.ListCellRenderer;
import net.stegemann.gui.model.TypesComboBoxModel;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.io.Serial;

public class TypeIdComponent extends JComboBox< Type> implements FocusListener
{
	@Serial
	private static final long serialVersionUID = -3436590634303978847L;

	private Types types;
	private Number typeId;

	public TypeIdComponent()
	{
		setMinimumSize( new Dimension( 150, -1));

		addActionListener( this);
		addFocusListener( this);

		setRenderer( new ListCellRenderer< Type>());
	}

	public void SetTypes( Types UseTypes)
	{
		types = UseTypes;

		setModel( new TypesComboBoxModel( types));
	}

	public void attachValue( Number useNumber)
	{
		typeId = useNumber;

		setSelectedIndex( types.getIndexFromId( typeId));
	}

	private void setFromComboBox()
	{
		try
		{
			typeId.setValue( types.getTypeIdFromIndex( getSelectedIndex()));
		}
		catch( Exception Reason)
		{
			// Ignore bad input and revert to old.
		}

		setSelectedIndex( types.getIndexFromId( typeId));
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
