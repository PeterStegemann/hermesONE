package net.stegemann.gui.components;

import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;

import javax.swing.JComboBox;

import net.stegemann.configuration.Type;
import net.stegemann.configuration.Types;
import net.stegemann.configuration.type.Number;
import net.stegemann.gui.model.ListCellRenderer;
import net.stegemann.gui.model.TypesComboBoxModel;

public class TypeIdComponent extends JComboBox< Type> implements FocusListener
{
	private static final long serialVersionUID = -3436590634303978847L;

	private Types types;
	private Number typeId;

	public TypeIdComponent()
	{
		setMinimumSize( new Dimension( 150, -1));

		addActionListener( this);
		addFocusListener( this);

		setRenderer( new ListCellRenderer<Type>());
	}

	public void SetTypes( Types UseTypes)
	{
		types = UseTypes;

		setModel( new TypesComboBoxModel( types));
	}

	public void attachValue( Number UseNumber)
	{
		typeId = UseNumber;

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
