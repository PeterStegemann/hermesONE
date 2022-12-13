package net.stegemann.gui.frame;

import net.stegemann.configuration.Calibrations;
import net.stegemann.gui.panel.system.CalibrationsPanel;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.Serial;

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
		CalibrationsPanel calibrationsPanel = new CalibrationsPanel( calibrations);
		setContentPane( calibrationsPanel);

		calibrationsPanel.Set();

		pack();

		setMinimumSize( getPreferredSize());
	}

	public void open()
	{		
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
