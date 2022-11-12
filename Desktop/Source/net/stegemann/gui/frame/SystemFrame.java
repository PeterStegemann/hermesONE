package net.stegemann.gui.frame;

import java.awt.Menu;
import java.awt.MenuBar;
import java.awt.MenuItem;
import java.awt.MenuShortcut;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JFrame;

import net.stegemann.configuration.Configuration;
import net.stegemann.gui.panel.SystemPanel;

public class SystemFrame extends JFrame implements ActionListener
{
	private static final long serialVersionUID = -6717842936654950443L;

	private final Configuration configuration;

	private final SystemPanel system;

	private MenuItem closeMenuItem;

	public SystemFrame( Configuration UseConfiguration)
	{
		super( "System");

		configuration = UseConfiguration;

		system = new SystemPanel( configuration);

		setResizable( false);

		setContentPane( system);

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

	public void Set()
	{
		system.Set();

		pack();

		setMinimumSize( getPreferredSize());
	}

	public void Open()
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
