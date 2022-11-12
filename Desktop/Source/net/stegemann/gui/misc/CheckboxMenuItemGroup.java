package net.stegemann.gui.misc;

import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.util.ArrayList;
import java.util.List;

import javax.swing.JCheckBoxMenuItem;

public class CheckboxMenuItemGroup implements ItemListener
{
	private List< JCheckBoxMenuItem> menuItems = new ArrayList< JCheckBoxMenuItem>();
	private ItemListener listener = null;

	public void add( JCheckBoxMenuItem MenuItem)
	{
		menuItems.add( MenuItem);
		MenuItem.addItemListener( this);
	}

	public JCheckBoxMenuItem getActiveItem()
	{
		for( JCheckBoxMenuItem MenuItem: menuItems)
		{
			if( MenuItem.getState() == true)
			{
				return MenuItem;
			}
		}

		return null;
	}

	public JCheckBoxMenuItem get( String Text)
	{
		for( JCheckBoxMenuItem MenuItem: menuItems)
		{
			if( Text.compareTo( MenuItem.getText()) == 0)
			{
				return MenuItem;
			}
		}

		return null;
	}

	@Override
	public void itemStateChanged( ItemEvent CurrentEvent)
	{
		if( CurrentEvent.getStateChange() == ItemEvent.SELECTED)
		{
			// Unselect other items.
			for( JCheckBoxMenuItem MenuItem: menuItems)
			{
				if( CurrentEvent.getSource() != MenuItem)
				{
					MenuItem.setState( false);
				}
			}
		}

		if( listener != null)
		{
			listener.itemStateChanged( CurrentEvent);
		}
	}

	public void addActionListener( ItemListener Listener)
	{
		listener = Listener;
	}
}
