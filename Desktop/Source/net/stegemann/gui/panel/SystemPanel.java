package net.stegemann.gui.panel;

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.Serial;
import java.util.ArrayList;
import java.util.List;
import javax.swing.*;
import net.stegemann.configuration.Configuration;
import net.stegemann.configuration.Models;
import net.stegemann.configuration.PPMs;
import net.stegemann.configuration.System;
import net.stegemann.configuration.type.ModelId;
import net.stegemann.gui.Constants;
import net.stegemann.gui.components.ModelIdComponent;
import net.stegemann.gui.components.TextComponent;
import net.stegemann.gui.panel.system.*;

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

    private final List<PPMPanel> ppms = new ArrayList<>();

    public SystemPanel( Configuration configuration)
    {
        this.configuration = configuration;

        JLabel ownerLabel = new JLabel( "Besitzer:");
        owner = new TextComponent( Constants.DEFAULT_TEXTFIELD_WIDTH);

        JLabel selectedModelLabel = new JLabel( "Aktives Modell:");
        selectedModel = new ModelIdComponent();

        JSeparator mainSeparator = new JSeparator();

        JTabbedPane mainTabPane = new JTabbedPane();

        statusDisplay = new StatusDisplayPanel( this.configuration.getSystem());
        setupDisplay = new SetupDisplayPanel( this.configuration.getSystem());

        JTabbedPane displayTabPane = new JTabbedPane();
        displayTabPane.add( statusDisplay, "Status");
        displayTabPane.add( setupDisplay, "Setup");

        mainTabPane.add( displayTabPane, "Bildschirm");

        batteryWarn = new BatteryWarnPanel( this.configuration.getSystem().getBattery());
        batteryCalibration = new BatteryCalibrationPanel( this.configuration.getSystem().getBattery());

        JTabbedPane batteryTabPane = new JTabbedPane();
        batteryTabPane.add( batteryWarn, "Warnung");
        batteryTabPane.add( batteryCalibration, "Kalibrierung");

        mainTabPane.add( batteryTabPane, "Batterie");

        PPMs ppms = this.configuration.getSystem().getPpms();

        for( int ppmCount = 0; ppmCount < ppms.getPPMCount(); ppmCount++)
        {
            PPMPanel ppm = new PPMPanel( ppms.getPPM( ppmCount));
            this.ppms.add( ppm);

            mainTabPane.add( ppm, "Modul " + ( ppmCount + 1));
        }

        calibration = new CalibrationsPanel( this.configuration.getSystem().getCalibrations());

        mainTabPane.add( calibration, "Kalibrierung");

        // Layout elements.
        GroupLayout layout = new GroupLayout( this);
        setLayout( layout);

//		Layout.setAutoCreateGaps( true);
        layout.setAutoCreateContainerGaps( true);

        layout.setHorizontalGroup
        (
            layout.createSequentialGroup()
            .addGroup
            (
                layout.createParallelGroup( GroupLayout.Alignment.TRAILING)
                .addGroup
                (
                    layout.createSequentialGroup()
                    .addGroup
                    (
                        layout.createParallelGroup( GroupLayout.Alignment.TRAILING)
                        .addComponent( ownerLabel)
                        .addComponent( selectedModelLabel)
                    )
                    .addGroup
                    (
                        layout.createParallelGroup( GroupLayout.Alignment.TRAILING)
                        .addComponent( selectedModel)
                        .addComponent( owner)
                    )
                )
                .addComponent( mainSeparator)
                .addComponent( mainTabPane)
            )
        );

        layout.setVerticalGroup
        (
            layout.createSequentialGroup()
            .addGroup
            (
                layout.createParallelGroup( GroupLayout.Alignment.CENTER)
                .addComponent( ownerLabel)
                .addComponent( owner)
            )
            .addGroup
            (
                layout.createParallelGroup( GroupLayout.Alignment.CENTER)
                .addComponent( selectedModelLabel)
                .addComponent( selectedModel)
            )
            .addComponent( mainSeparator)
            .addComponent( mainTabPane)
        );
    }

    public void Set()
    {
        System system = configuration.getSystem();
        Models models = configuration.getModels();

        owner.attachValue( system.getOwner());

        ModelId selectedModelId = system.getSelectedModel();

        selectedModel.setModels( models);
        selectedModel.attachValue( selectedModelId);

        statusDisplay.set();
        setupDisplay.set();
        batteryWarn.Set();
        batteryCalibration.set();

        calibration.Set();

        ppms.forEach( PPMPanel::Set);
    }

    @Override
    public void actionPerformed( ActionEvent event)
    {
    }
}
