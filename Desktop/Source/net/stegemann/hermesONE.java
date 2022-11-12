package net.stegemann;
// hermesONE configuration GUI

import javax.swing.UIManager;

import net.stegemann.configuration.Configuration;
import net.stegemann.controller.Controller;
import net.stegemann.gui.frame.MainFrame;

public class hermesONE
{
	private final Configuration configuration = new Configuration();
	private final Controller controller = new Controller( configuration);

	private final MainFrame modelsFrame = new MainFrame( controller);

	public static void main( String[] Arguments)
	{
		System.setProperty( "apple.laf.useScreenMenuBar", "true");
//		System.setProperty( "com.apple.mrj.application.apple.menu.about.name", "Test");

		try
		{
			UIManager.setLookAndFeel( UIManager.getSystemLookAndFeelClassName());
		}
		catch( Exception reason)
		{
			reason.printStackTrace();
		}

		hermesONE hermesONE = new hermesONE();

		hermesONE.Run();
	}

	public void Run()
	{
		// Start UI.
		javax.swing.SwingUtilities.invokeLater(this::run);
	}

	private void run() {
		modelsFrame.set();
		modelsFrame.setLocationRelativeTo(null);
		modelsFrame.open();
	}
}
