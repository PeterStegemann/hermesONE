package net.stegemann.gui.components;

import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;

import javax.swing.JComboBox;

import net.stegemann.configuration.Model;
import net.stegemann.configuration.Models;
import net.stegemann.configuration.type.Number;
import net.stegemann.gui.model.ListCellRenderer;
import net.stegemann.gui.model.ModelsComboBoxModel;

public class ModelIdComponent extends JComboBox< Model> implements FocusListener
{
	private static final long serialVersionUID = -3436590634303978847L;

	private Models models;
	private Number modelId;

	public ModelIdComponent()
	{
		setMinimumSize( new Dimension( 150, -1));

		addActionListener( this);
		addFocusListener( this);

		setRenderer( new ListCellRenderer<Model>());
	}

	public void setModels( Models models)
	{
		this.models = models;

		setModel( new ModelsComboBoxModel( models));
	}

	public void attachValue( Number UseModelId)
	{
		modelId = UseModelId;

		setSelectedIndex( models.getIndexFromId( modelId));
	}

	private void setFromComboBox()
	{
		try
		{
			modelId.setValue( models.getIdFromIndex( getSelectedIndex()));
		}
		catch( Exception Reason)
		{
			// Ignore bad input and revert to old.
		}

		setSelectedIndex( models.getIndexFromId( modelId));
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
