package net.stegemann.gui.frame;

import java.awt.Menu;
import java.awt.MenuBar;
import java.awt.MenuItem;
import java.awt.MenuShortcut;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JFrame;

import net.stegemann.configuration.ChannelMappings;
import net.stegemann.gui.panel.system.ChannelMappingsPanel;

public class ChannelMappingsFrame  extends JFrame implements ActionListener
{
	private static final long serialVersionUID = 8578935790343135041L;

	private ChannelMappings channelMappings;

	private ChannelMappingsPanel channelMappingsPanel;

	private MenuItem closeMenuItem;

	public ChannelMappingsFrame( ChannelMappings UseChannelMappings)
	{
		super( "Kanalzuweisung");

		channelMappings = UseChannelMappings;

		setResizable( false);

		addMenus();
	}

	private void addMenus()
	{
		MenuBar NewMenuBar = new MenuBar();
		Menu NewMenu;

		NewMenu = new Menu( "Fenster");

		closeMenuItem = new MenuItem( "Schliessen", new MenuShortcut( 'W'));
		closeMenuItem.addActionListener( this); 
		NewMenu.add( closeMenuItem);

		NewMenuBar.add( NewMenu);

		setMenuBar( NewMenuBar);
	}

	public void set()
	{
		channelMappingsPanel = new ChannelMappingsPanel( channelMappings);
		setContentPane( channelMappingsPanel);

		channelMappingsPanel.set();

		pack();

		setMinimumSize( getPreferredSize());
	}

	public void Show()
	{
		// Display the window.
		setVisible( true);
	}

	@Override
	public void actionPerformed( ActionEvent e)
	{
		if( e.getSource() == closeMenuItem)
		{
			setVisible( false);
		}
	}
}
