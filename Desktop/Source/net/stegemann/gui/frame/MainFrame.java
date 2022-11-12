package net.stegemann.gui.frame;

import java.awt.FileDialog;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JCheckBoxMenuItem;
import javax.swing.JFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.KeyStroke;
import javax.swing.event.MenuEvent;
import javax.swing.event.MenuListener;

import net.stegemann.configuration.Configuration;
import net.stegemann.controller.Controller;
import net.stegemann.gui.dialogs.ProgressDialog;
import net.stegemann.gui.panel.MainPanel;
import net.stegemann.gui.misc.CheckboxMenuItemGroup;
import net.stegemann.io.ReadException;
import net.stegemann.io.WriteException;
import net.stegemann.io.serial.base.Ports;
import net.stegemann.io.serial.configuration.read.SerialConfigurationReader;
import net.stegemann.io.serial.configuration.write.SerialConfigurationWriter;
import net.stegemann.io.xml.XMLReader;
import net.stegemann.io.xml.XMLWriter;

public class MainFrame extends JFrame implements ActionListener
{
	private static final long serialVersionUID = 7528862913260099674L;

	private final Configuration configuration;

	private final MainPanel modelsPanel;

	private JMenuItem openMenuItem;
	private JMenuItem openSystemMenuItem;
	private JMenuItem openModelsMenuItem;
	private JMenuItem closeMenuItem;
	private JMenuItem saveMenuItem;
	private JMenuItem saveAsMenuItem;
	private JMenu portMenu;
	private JMenuItem readMenuItem;
	private JMenuItem writeMenuItem;

	private CheckboxMenuItemGroup portGroup;

	private String directory = null;
	private String file = null;
	private final FileDialog openConfigurationDialog;
	private final FileDialog saveConfigurationDialog;

	public MainFrame( Controller controller)
	{
		super( "hermesONE");

		configuration = controller.getConfiguration();

		file = "Configuration.xml";

		setResizable( true);

		// This is the main window, so it will close the application on close.
		setDefaultCloseOperation( JFrame.EXIT_ON_CLOSE);

		modelsPanel = new MainPanel( controller);

		add( modelsPanel);

		addMenus();

		openConfigurationDialog =
			new FileDialog( this, "Konfiguration laden...", FileDialog.LOAD);
		saveConfigurationDialog =
			new FileDialog( this, "Konfiguration speichern...", FileDialog.SAVE);
	}

	private void addMenus()
	{
		JMenuBar NewMenuBar = new JMenuBar();
		JMenu NewMenu;

		NewMenu = new JMenu( "Datei");

		openMenuItem = addMenuItem( NewMenu, "Konfiguration öffnen...", 'O');
		NewMenu.addSeparator();
		closeMenuItem = addMenuItem( NewMenu, "Schliessen", 'W');
		saveMenuItem = addMenuItem( NewMenu, "Konfiguration speichern", 'S');
		saveAsMenuItem = addMenuItem( NewMenu, "Konfiguration speichern unter...", 'S',
			ActionEvent.SHIFT_MASK);
		NewMenu.addSeparator();
		openSystemMenuItem = addMenuItem( NewMenu, "Systemeinstellungen von Datei importieren...");
		openModelsMenuItem = addMenuItem( NewMenu, "Modelleinstellungen von Datei importieren...");

		NewMenuBar.add( NewMenu);

		NewMenu = new JMenu( "Fernsteuerung");

		readMenuItem = addMenuItem( NewMenu, "Konfiguration lesen...");
		writeMenuItem = addMenuItem( NewMenu, "Konfiguration schreiben...");
		NewMenu.addSeparator();
		portMenu = new JMenu( "Port");
		portMenu.addActionListener( this);
		portMenu.addMenuListener( new PortMenuListener());

		setPortMenu();

		NewMenu.add( portMenu);

		NewMenuBar.add( NewMenu);

//		NewMenu = new Menu( "Modell");
//		NewMenuBar.add( NewMenu);

		setJMenuBar( NewMenuBar);
	}

	private void setPortMenu()
	{
		portMenu.removeAll();

		CheckboxMenuItemGroup NewPortGroup = new CheckboxMenuItemGroup();

		for( String Port: Ports.FindPorts())
		{
			JCheckBoxMenuItem NewMenuItem = new JCheckBoxMenuItem( Port);
			NewPortGroup.add( NewMenuItem);
			portMenu.add( NewMenuItem);

			// Get state from old item.
			if( portGroup != null)
			{
				JCheckBoxMenuItem OldMenuItem = portGroup.get( Port);

				if( OldMenuItem != null)
				{
					NewMenuItem.setSelected( OldMenuItem.getState());
				}
			}
		}

		portGroup = NewPortGroup;
	}

	private class PortMenuListener implements MenuListener
	{
		@Override
		public void menuCanceled( MenuEvent e) {}
		@Override
		public void menuDeselected( MenuEvent e) {}

		@Override
		public void menuSelected( MenuEvent e)
		{
			setPortMenu();
		}
	}

	private JMenuItem addMenuItem( JMenu Parent, String Text)
	{
		JMenuItem NewMenuItem = new JMenuItem( Text);
		NewMenuItem.addActionListener( this); 
		Parent.add( NewMenuItem);

		return NewMenuItem;
	}

	private JMenuItem addMenuItem( JMenu Parent, String Text, char Shortcut)
	{
		return addMenuItem( Parent, Text, KeyStroke.getKeyStroke( Shortcut,
								Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
	}

	private JMenuItem addMenuItem( JMenu Parent, String Text, char Shortcut, int Modifier)
	{
		return addMenuItem( Parent, Text, KeyStroke.getKeyStroke( Shortcut,
								Modifier | Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
	}

	private JMenuItem addMenuItem( JMenu Parent, String Text, KeyStroke UseKeyStroke)
	{
		JMenuItem NewMenuItem = new JMenuItem( Text);
		NewMenuItem.setAccelerator( UseKeyStroke);
		NewMenuItem.addActionListener( this); 
		Parent.add( NewMenuItem);

		return NewMenuItem;
	}

	public void set()
	{
		modelsPanel.Set();

		pack();

		setMinimumSize( getPreferredSize());
	}

	public void open()
	{
		// Display the window.
		setVisible( true);
	}

	@Override
	public void actionPerformed( ActionEvent CurrentEvent)
	{
		if( CurrentEvent.getSource() == openMenuItem)
		{
			openConfiguration(XMLReader.Mode.All);
		}
		else if( CurrentEvent.getSource() == openSystemMenuItem)
		{
			openConfiguration(XMLReader.Mode.System);
		}
		else if( CurrentEvent.getSource() == openModelsMenuItem)
		{
			openConfiguration(XMLReader.Mode.Models);
		}
		else if( CurrentEvent.getSource() == closeMenuItem)
		{
			System.exit( NORMAL);
		}
		else if( CurrentEvent.getSource() == saveMenuItem)
		{
			saveConfiguration();
		}
		else if( CurrentEvent.getSource() == saveAsMenuItem)
		{
			saveAsConfiguration();
		}
		else if( CurrentEvent.getSource() == readMenuItem)
		{
			readConfiguration();
		}
		else if( CurrentEvent.getSource() == writeMenuItem)
		{
			writeConfiguration();
		}
	}

	private void openConfiguration(XMLReader.Mode mode)
	{
		openConfigurationDialog.setVisible( true);

		String Directory = openConfigurationDialog.getDirectory();
		String File = openConfigurationDialog.getFile();

		if(( Directory != null) && ( File != null))
		{
			XMLReader ConfigurationReader = new XMLReader();

			try
			{
				directory = Directory;
				file = File;

				ConfigurationReader.ReadFromFile( configuration, directory + file, mode);
			}
			catch( ReadException Reason)
			{
				System.out.println( "Failed opening configuration, reason: " + Reason.getMessage());

				Reason.printStackTrace();
			}

			set();
		}
	}

	private void saveConfiguration()
	{
		// No current file yet? Go, pick one.
		if(( directory == null) || ( file == null))
		{
			saveAsConfiguration();

			return;
		}

		// Overwrite old one.
		XMLWriter ConfigurationWriter = new XMLWriter();

		try
		{
			ConfigurationWriter.WriteToFile( configuration, directory + file);
		}
		catch( WriteException Reason)
		{
			Reason.printStackTrace();
		}
	}

	private void saveAsConfiguration()
	{
		saveConfigurationDialog.setDirectory( directory);
		saveConfigurationDialog.setFile( file);

		saveConfigurationDialog.setVisible( true);

		String Directory = saveConfigurationDialog.getDirectory();
		String File = saveConfigurationDialog.getFile();

		if(( Directory != null) && ( File != null))
		{
			XMLWriter ConfigurationWriter = new XMLWriter();

			try
			{
				directory = Directory;
				file = File;

				ConfigurationWriter.WriteToFile( configuration, directory + file);
			}
			catch( WriteException Reason)
			{
				Reason.printStackTrace();
			}
		}
	}

	private void readConfiguration()
	{
		final JCheckBoxMenuItem ActiveItem = portGroup.getActiveItem();

		if( ActiveItem == null)
		{
			JOptionPane.showMessageDialog( this, "Es ist kein serieller Port ausgewaehlt!",
										   "Fehler beim Lesen", JOptionPane.ERROR_MESSAGE);

			return;
		}

		final SerialConfigurationReader ConfigurationReader = new SerialConfigurationReader();
		final ProgressDialog readingDialog = new ProgressDialog( this, "Lese Konfiguration...");

		new Thread( new Runnable()
		{
			@Override
			public void run()
			{
				try
				{
					ConfigurationReader.readFromPort( configuration, ActiveItem.getText(),
							readingDialog);
				}
				catch( ReadException Reason)
				{
					System.out.println( "Failed reading configuration, reason: " +
						Reason.getMessage());

					Reason.printStackTrace();
				}

				readingDialog.close();
			}				
		}).start();

		readingDialog.open();

		set();
	}

	private void writeConfiguration()
	{
		final JCheckBoxMenuItem ActiveItem = portGroup.getActiveItem();

		if( ActiveItem == null)
		{
			JOptionPane.showMessageDialog( this, "Es ist kein serieller Port ausgewaehlt!",
										   "Fehler beim Schreiben", JOptionPane.ERROR_MESSAGE);

			return;
		}

		final SerialConfigurationWriter ConfigurationWriter = new SerialConfigurationWriter();
		final ProgressDialog writingDialog = new ProgressDialog( this, "Schreibe Konfiguration...");

		new Thread( new Runnable()
		{
			@Override
			public void run()
			{
				try
				{
					ConfigurationWriter.writeToPort( configuration, ActiveItem.getText(),
							writingDialog);
				}
				catch( WriteException reason)
				{
					System.out.println( "Failed writing configuration, reason: " + reason.getMessage());

					reason.printStackTrace();
				}

				writingDialog.close();
			}				
		}).start();

		writingDialog.open();

		set();
	}
}
