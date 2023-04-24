package net.stegemann.gui.panel;

import net.stegemann.configuration.Configuration;
import net.stegemann.configuration.Models;
import net.stegemann.configuration.PPMs;
import net.stegemann.configuration.type.Number;
import net.stegemann.gui.Constants;
import net.stegemann.gui.components.ModelIdComponent;
import net.stegemann.gui.components.TextComponent;
import net.stegemann.gui.panel.system.*;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.Serial;
import java.util.ArrayList;
import java.util.List;

public class SystemPanel extends Panel implements ActionListener
{
	@Serial
	private static final long serialVersionUID = -2820312553040435152L;

	private final Configuration configuration;

	private final TextComponent owner;
	private final ModelIdComponent selectedModel;

	private final StatusDisplayPanel statusDisplay;
	private final SetupDisplayPanel setupDisplay;

	private final BatteryWarnPanel batteryWarn;
	private final BatteryCalibrationPanel batteryCalibration;

	private final CalibrationsPanel calibration;

	private final List<PPMPanel> ppms = new ArrayList<PPMPanel>();

	public SystemPanel( Configuration Configuration)
	{
		configuration = Configuration;

		JLabel OwnerLabel = new JLabel( "Besitzer:");
		owner = new TextComponent( Constants.DEFAULT_TEXTFIELD_WIDTH);

		JLabel SelectedModelLabel = new JLabel( "Aktives Modell:");
		selectedModel = new ModelIdComponent();

		JSeparator MainSeparator = new JSeparator();

		JTabbedPane MainTabPane = new JTabbedPane();

		statusDisplay = new StatusDisplayPanel( configuration.getSystem());
		setupDisplay = new SetupDisplayPanel( configuration.getSystem());

		JTabbedPane DisplayTabPane = new JTabbedPane();
		DisplayTabPane.add( statusDisplay, "Status");
		DisplayTabPane.add( setupDisplay, "Setup");

		MainTabPane.add( DisplayTabPane, "Bildschirm");

		batteryWarn = new BatteryWarnPanel( configuration.getSystem().getBattery());
		batteryCalibration = new BatteryCalibrationPanel( configuration.getSystem().getBattery());

		JTabbedPane BatteryTabPane = new JTabbedPane();
		BatteryTabPane.add( batteryWarn, "Warnung");
		BatteryTabPane.add( batteryCalibration, "Kalibrierung");

		MainTabPane.add( BatteryTabPane, "Batterie");

		PPMs UsePPMs = configuration.getSystem().getPpms();

		for( int ppmCount = 0; ppmCount < UsePPMs.getPPMCount(); ppmCount++) {
			PPMPanel ppm = new PPMPanel( UsePPMs.getPPM( ppmCount));
			ppms.add( ppm);

			MainTabPane.add( ppm, "Modul " + ( ppmCount + 1));
		}

		calibration = new CalibrationsPanel( configuration.getSystem().getCalibrations());

		MainTabPane.add( calibration, "Kalibrierung");

		// Layout elements.
		GroupLayout Layout = new GroupLayout( this);
		setLayout( Layout);

//		Layout.setAutoCreateGaps( true);
		Layout.setAutoCreateContainerGaps( true);

		Layout.setHorizontalGroup( Layout.createSequentialGroup()
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.TRAILING)
				.addGroup( Layout.createSequentialGroup()
					.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.TRAILING)
						.addComponent( OwnerLabel)
						.addComponent( SelectedModelLabel)
					)
					.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.TRAILING)
						.addComponent( selectedModel)
						.addComponent( owner)
					)
				)
				.addComponent( MainSeparator)
				.addComponent( MainTabPane)
			)
		);

		Layout.setVerticalGroup( Layout.createSequentialGroup()
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( OwnerLabel)
				.addComponent( owner)
			)
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( SelectedModelLabel)
				.addComponent( selectedModel)
			)
			.addComponent( MainSeparator)
			.addComponent( MainTabPane)
		);
	}

	public void Set()
	{
		net.stegemann.configuration.System System = configuration.getSystem();
		Models UseModels = configuration.getModels();

		owner.attachValue( System.getOwner());

		Number SelectedModel = System.getSelectedModel();

		selectedModel.setModels( UseModels);
		selectedModel.attachValue( SelectedModel);

		statusDisplay.set();
		setupDisplay.set();
		batteryWarn.Set();
		batteryCalibration.set();

		calibration.Set();

		ppms.forEach( PPMPanel::Set);
	}

	@Override
	public void actionPerformed( ActionEvent Event)
	{
	}
}
