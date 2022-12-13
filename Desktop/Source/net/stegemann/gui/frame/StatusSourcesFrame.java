package net.stegemann.gui.frame;

import net.stegemann.configuration.Configuration;
import net.stegemann.configuration.Model;
import net.stegemann.gui.panel.StatusPanel;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.Serial;

public class StatusSourcesFrame extends JFrame implements ActionListener
{
	@Serial
	private static final long serialVersionUID = 2158442385827846260L;

	private final StatusPanel statusSources;

	private MenuItem closeMenuItem;

	public StatusSourcesFrame( Configuration configuration)
	{
		super( "Status Quellen");

		statusSources = new StatusPanel( configuration);

		setResizable( false);

		setContentPane( statusSources);

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

	public void set(Model UseModel)
	{
		if( UseModel == null)
		{
			setVisible( false);

			return;
		}
	
		statusSources.Set( UseModel);

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
