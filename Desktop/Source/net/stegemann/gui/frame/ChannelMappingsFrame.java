package net.stegemann.gui.frame;

import net.stegemann.configuration.ChannelMappings;
import net.stegemann.gui.panel.system.ChannelMappingsPanel;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.Serial;

public class ChannelMappingsFrame extends JFrame implements ActionListener
{
	@Serial
	private static final long serialVersionUID = 8578935790343135041L;

	private final ChannelMappings channelMappings;

	private MenuItem closeMenuItem;

	public ChannelMappingsFrame( ChannelMappings useChannelMappings)
	{
		super( "Kanalzuweisung");

		channelMappings = useChannelMappings;

		setResizable( false);

		addMenus();
	}

	private void addMenus()
	{
		MenuBar newMenuBar = new MenuBar();
		Menu newMenu;

		newMenu = new Menu( "Fenster");

		closeMenuItem = new MenuItem( "Schliessen", new MenuShortcut( 'W'));
		closeMenuItem.addActionListener( this); 
		newMenu.add( closeMenuItem);

		newMenuBar.add( newMenu);

		setMenuBar( newMenuBar);
	}

	public void set()
	{
		ChannelMappingsPanel channelMappingsPanel = new ChannelMappingsPanel( channelMappings);
		setContentPane( channelMappingsPanel);

		channelMappingsPanel.set();

		pack();

		setMinimumSize( getPreferredSize());
	}

	public void open()
	{
		setVisible( true);
	}

	@Override
	public void actionPerformed( ActionEvent event)
	{
		if( event.getSource() == closeMenuItem)
		{
			setVisible( false);
		}
	}
}
