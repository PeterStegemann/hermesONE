package net.stegemann.gui.misc;

import javax.swing.*;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.util.ArrayList;
import java.util.List;

public class CheckboxMenuItemGroup implements ItemListener
{
	private final List< JCheckBoxMenuItem> menuItems = new ArrayList<>();
	private ItemListener listener = null;

	public void add( JCheckBoxMenuItem MenuItem)
	{
		menuItems.add( MenuItem);
		MenuItem.addItemListener( this);
	}

	public JCheckBoxMenuItem getActiveItem()
	{
		for( JCheckBoxMenuItem menuItem: menuItems)
		{
			if( menuItem.getState() == true)
			{
				return menuItem;
			}
		}

		return null;
	}

	public JCheckBoxMenuItem get( String Text)
	{
		for( JCheckBoxMenuItem menuItem: menuItems)
		{
			if( Text.compareTo( menuItem.getText()) == 0)
			{
				return menuItem;
			}
		}

		return null;
	}

	@Override
	public void itemStateChanged( ItemEvent event)
	{
		if( event.getStateChange() == ItemEvent.SELECTED)
		{
			// Unselect other items.
			for( JCheckBoxMenuItem menuItem: menuItems)
			{
				if( event.getSource() != menuItem)
				{
					menuItem.setState( false);
				}
			}
		}

		if( listener != null)
		{
			listener.itemStateChanged( event);
		}
	}

	public void addActionListener( ItemListener useListener)
	{
		listener = useListener;
	}
}
