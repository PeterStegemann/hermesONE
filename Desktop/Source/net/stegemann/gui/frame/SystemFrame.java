package net.stegemann.gui.frame;

import net.stegemann.configuration.Configuration;
import net.stegemann.gui.panel.SystemPanel;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.Serial;

public class SystemFrame extends JFrame implements ActionListener
{
	@Serial
	private static final long serialVersionUID = -6717842936654950443L;

	private final SystemPanel system;

	private MenuItem closeMenuItem;

	public SystemFrame( Configuration configuration)
	{
		super( "System");

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

	public void set()
	{
		system.Set();

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
