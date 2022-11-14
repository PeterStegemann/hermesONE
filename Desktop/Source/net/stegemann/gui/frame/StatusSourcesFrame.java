package net.stegemann.gui.frame;

import java.awt.Menu;
import java.awt.MenuBar;
import java.awt.MenuItem;
import java.awt.MenuShortcut;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;import java.io.Serial;

import javax.swing.JFrame;

import net.stegemann.configuration.Configuration;
import net.stegemann.configuration.Model;
import net.stegemann.gui.panel.StatusPanel;

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

	public void Set( Model UseModel)
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
