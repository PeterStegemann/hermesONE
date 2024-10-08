package net.stegemann.gui.components;

import net.stegemann.configuration.type.Number;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.io.Serial;

public class ChannelIdComponent extends JComboBox< String> implements FocusListener
{
	@Serial
	private static final long serialVersionUID = -7383136374736538998L;

	private static final String[] CHANNEL_IDS =
	{
		"1",
		"2",
		"3",
		"4",
		"5",
		"6",
		"7",
		"8",
		"9"
	};

	private Number channelId;

	public ChannelIdComponent()
	{
		super( CHANNEL_IDS);

		setMinimumSize( new Dimension( 150, -1));

		addActionListener( this);
		addFocusListener( this);
	}

	public void attachValue( Number useChannelId)
	{
		channelId = useChannelId;

		setSelectedIndex( channelId.getValue());
	}

	private void setFromComboBox()
	{
		try
		{
			channelId.setValue( getSelectedIndex());
		}
		catch( Exception Reason)
		{
			// Ignore bad input and revert to old.
		}

		setSelectedIndex( channelId.getValue());
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
