package net.stegemann.gui.frame;

import java.awt.Menu;
import java.awt.MenuBar;
import java.awt.MenuItem;
import java.awt.MenuShortcut;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;import java.io.Serial;

import javax.swing.JFrame;

import net.stegemann.configuration.Calibrations;
import net.stegemann.gui.panel.system.CalibrationsPanel;

public class CalibrationsFrame  extends JFrame implements ActionListener
{
	@Serial
	private static final long serialVersionUID = 8578935790343135041L;

	private final Calibrations calibrations;

	private MenuItem closeMenuItem;

	public CalibrationsFrame( Calibrations useCalibrations)
	{
		super( "Kalibrierungen");

		calibrations = useCalibrations;

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

	public void Set()
	{
		CalibrationsPanel calibrationsPanel = new CalibrationsPanel( calibrations);
		setContentPane( calibrationsPanel);

		calibrationsPanel.Set();

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
