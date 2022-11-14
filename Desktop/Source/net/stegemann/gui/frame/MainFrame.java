package net.stegemann.gui.frame;

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.Serial;

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
import net.stegemann.io.serial.configuration.ConfigurationObjectFactory;import net.stegemann.io.serial.configuration.SerialConfigurationReader;
import net.stegemann.io.serial.configuration.SerialConfigurationWriter;
import net.stegemann.io.xml.XMLObjectFactory;
import net.stegemann.io.xml.XMLReader;
import net.stegemann.io.xml.XMLWriter;

public class MainFrame extends JFrame implements ActionListener
{
	@Serial
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

	private String lastDirectory;
	private String lastFile;
	private final FileDialog openConfigurationDialog;
	private final FileDialog saveConfigurationDialog;

	private final SerialConfigurationReader serialConfigurationReader =
	 	ConfigurationObjectFactory.serialConfigurationReader();
	private final SerialConfigurationWriter serialConfigurationWriter =
	 	ConfigurationObjectFactory.serialConfigurationWriter();

	public MainFrame( Controller controller)
	{
		super( "hermesONE");

		configuration = controller.getConfiguration();

		lastDirectory = null;
		lastFile = "Configuration.xml";

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
		JMenuBar newMenuBar = new JMenuBar();
		JMenu newMenu;

		newMenu = new JMenu( "Datei");

		openMenuItem = addMenuItem( newMenu, "Konfiguration öffnen...", 'O');
		newMenu.addSeparator();
		closeMenuItem = addMenuItem( newMenu, "Schliessen", 'W');
		saveMenuItem = addMenuItem( newMenu, "Konfiguration speichern", 'S');
		saveAsMenuItem = addMenuItem
		(
			newMenu, "Konfiguration speichern unter...", 'S', ActionEvent.SHIFT_MASK
		);
		newMenu.addSeparator();
		openSystemMenuItem =
			addMenuItem( newMenu, "Systemeinstellungen von Datei importieren...");
		openModelsMenuItem =
			addMenuItem( newMenu, "Modelleinstellungen von Datei importieren...");

		newMenuBar.add( newMenu);

		newMenu = new JMenu( "Fernsteuerung");

		readMenuItem = addMenuItem( newMenu, "Konfiguration lesen...");
		writeMenuItem = addMenuItem( newMenu, "Konfiguration schreiben...");
		newMenu.addSeparator();
		portMenu = new JMenu( "Port");
		portMenu.addActionListener( this);
		portMenu.addMenuListener( new PortMenuListener());

		setPortMenu();

		newMenu.add( portMenu);

		newMenuBar.add( newMenu);

//		NewMenu = new Menu( "Modell");
//		NewMenuBar.add( NewMenu);

		setJMenuBar( newMenuBar);
	}

	private void setPortMenu()
	{
		portMenu.removeAll();

		CheckboxMenuItemGroup newPortGroup = new CheckboxMenuItemGroup();

		for( String port: Ports.FindPorts())
		{
			JCheckBoxMenuItem newMenuItem = new JCheckBoxMenuItem( port);
			newPortGroup.add( newMenuItem);
			portMenu.add( newMenuItem);

			// Get state from old item.
			if( portGroup != null)
			{
				JCheckBoxMenuItem oldMenuItem = portGroup.get( port);

				if( oldMenuItem != null)
				{
					newMenuItem.setSelected( oldMenuItem.getState());
				}
			}
		}

		portGroup = newPortGroup;
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

	private JMenuItem addMenuItem( JMenu parent, String text, char shortcut)
	{
		return addMenuItem( parent, text, shortcut, 0);
	}

	private JMenuItem addMenuItem( JMenu parent, String text, char shortcut, int modifier)
	{
		KeyStroke keyStroke = KeyStroke.getKeyStroke
		(
			shortcut,
			modifier | Toolkit.getDefaultToolkit().getMenuShortcutKeyMaskEx()
		);

		JMenuItem menuItem = addMenuItem( parent, text);

		menuItem.setAccelerator( keyStroke);

		return menuItem;
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
	public void actionPerformed( ActionEvent actionEvent)
	{
		if( actionEvent.getSource() == openMenuItem)
		{
			openConfiguration( XMLReader.Mode.All);
		}
		else if( actionEvent.getSource() == openSystemMenuItem)
		{
			openConfiguration( XMLReader.Mode.System);
		}
		else if( actionEvent.getSource() == openModelsMenuItem)
		{
			openConfiguration( XMLReader.Mode.Models);
		}
		else if( actionEvent.getSource() == closeMenuItem)
		{
			System.exit( 0);
		}
		else if( actionEvent.getSource() == saveMenuItem)
		{
			saveConfiguration();
		}
		else if( actionEvent.getSource() == saveAsMenuItem)
		{
			saveAsConfiguration();
		}
		else if( actionEvent.getSource() == readMenuItem)
		{
			readConfiguration();
		}
		else if( actionEvent.getSource() == writeMenuItem)
		{
			writeConfiguration();
		}
	}

	private void openConfiguration( XMLReader.Mode mode)
	{
		openConfigurationDialog.setVisible( true);

		String directory = openConfigurationDialog.getDirectory();
		String file = openConfigurationDialog.getFile();

		if(( directory != null) && ( file != null))
		{
			lastDirectory = directory;
			lastFile = file;

			XMLReader configurationReader = XMLObjectFactory.xmlReader();

			try
			{
				configurationReader.readFromFile
				(
					configuration,
					lastDirectory + lastFile,
			 		mode
				);
			}
			catch( ReadException reason)
			{
				System.out.println( "Failed opening configuration, reason: " + reason.getMessage());

				reason.printStackTrace();
			}

			set();
		}
	}

	private void saveConfiguration()
	{
		// No current file yet? Go, pick one.
		if(( lastDirectory == null) || ( lastFile == null))
		{
			saveAsConfiguration();

			return;
		}

		// Overwrite old one.
		doSaveConfiguration();
	}

	private void saveAsConfiguration()
	{
		saveConfigurationDialog.setDirectory( lastDirectory);
		saveConfigurationDialog.setFile( lastFile);

		saveConfigurationDialog.setVisible( true);

		String directory = saveConfigurationDialog.getDirectory();
		String file = saveConfigurationDialog.getFile();

		if(( directory != null) && ( file != null))
		{
			lastDirectory = directory;
			lastFile = file;

			doSaveConfiguration();
		}
	}

	private void doSaveConfiguration()
	{
		XMLWriter configurationWriter = XMLObjectFactory.xmlWriter();

		try
		{
			configurationWriter.writeToFile( configuration, lastDirectory + lastFile);
		}
		catch( WriteException reason)
		{
			System.out.println( "Failed writing configuration, reason: " + reason.getMessage());

			reason.printStackTrace();
		}
	}

	private void readConfiguration()
	{
		final JCheckBoxMenuItem activeItem = portGroup.getActiveItem();

		if( activeItem == null)
		{
			JOptionPane.showMessageDialog
			(
				this,
				"Es ist kein serieller Port ausgewaehlt!",
				"Fehler beim Lesen",
				JOptionPane.ERROR_MESSAGE
			);

			return;
		}

		String portName = activeItem.getText();
		ProgressDialog progressDialog =
				new ProgressDialog( this, "Lese Konfiguration...");

		runWithProgress( progressDialog, () -> readConfiguration( portName, progressDialog ) );

		set();
	}

	private void readConfiguration( String portName, ProgressDialog progressDialog)
	{
		try
		{
			serialConfigurationReader.readFromPort( configuration, portName, progressDialog);
		}
		catch( ReadException reason)
		{
			System.out.println( "Failed reading configuration, reason: " + reason.getMessage());

			reason.printStackTrace();
		}
	}

	private void writeConfiguration()
	{
		final JCheckBoxMenuItem activeItem = portGroup.getActiveItem();

		if( activeItem == null)
		{
			JOptionPane.showMessageDialog
			(
				this,
				"Es ist kein serieller Port ausgewaehlt!",
				"Fehler beim Schreiben",
				JOptionPane.ERROR_MESSAGE
			);

			return;
		}

		String portName = activeItem.getText();
		ProgressDialog progressDialog =
				new ProgressDialog( this, "Schreibe Konfiguration..." );

		runWithProgress( progressDialog, () -> writeConfiguration( portName, progressDialog));

		set();
	}

	private void writeConfiguration( String portName, ProgressDialog progressDialog)
	{
		try
		{
			serialConfigurationWriter.writeToPort( configuration, portName, progressDialog);
		}
		catch( WriteException reason)
		{
			System.out.println( "Failed writing configuration, reason: " + reason.getMessage());

			reason.printStackTrace();
		}
	}

	private void runWithProgress( ProgressDialog progressDialog, Runnable runnable)
	{
		new Thread
		(
			() ->
			{
				runnable.run();

				progressDialog.close();
			}
		).start();

		progressDialog.open();
	}
}
