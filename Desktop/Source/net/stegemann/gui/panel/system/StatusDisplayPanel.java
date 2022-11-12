package net.stegemann.gui.panel.system;

import javax.swing.GroupLayout;
import javax.swing.JLabel;
import javax.swing.JPanel;

import net.stegemann.configuration.System;
import net.stegemann.gui.components.BoolComponent;
import net.stegemann.gui.components.NumberSliderComponent;
import net.stegemann.gui.components.TimeSliderComponent;

public class StatusDisplayPanel extends JPanel
{
	private static final long serialVersionUID = 4998074720939787525L;

	private final System system;

	private final NumberSliderComponent backlight;
	private final NumberSliderComponent contrast;
	private final TimeSliderComponent blanktime;
	private final BoolComponent inverted;

	public StatusDisplayPanel( System UseSystem)
	{
		system = UseSystem;

		JLabel BacklightLabel = new JLabel( "Licht:");
		backlight = new NumberSliderComponent( System.SYSTEM_BACKLIGHT_MINIMUM,
											   System.SYSTEM_BACKLIGHT_MAXIMUM);
		JLabel ContrastLabel = new JLabel( "Kontrast:");
		contrast = new NumberSliderComponent( System.SYSTEM_CONTRAST_MINIMUM,
											  System.SYSTEM_CONTRAST_MAXIMUM);

		JLabel BlanktimeLabel = new JLabel( "Bildschirm Aus:");
		blanktime = new TimeSliderComponent( System.SYSTEM_BLANKTIME_MINIMUM,
											 System.SYSTEM_BLANKTIME_MAXIMUM);

		JLabel InvertedLabel = new JLabel( "Invertiert");
		inverted = new BoolComponent();

		// Layout elements.
		GroupLayout Layout = new GroupLayout( this);
		setLayout( Layout);

//		Layout.setAutoCreateGaps( true);
		Layout.setAutoCreateContainerGaps( true);

		Layout.setHorizontalGroup( Layout.createSequentialGroup()
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.TRAILING)
				.addComponent( BacklightLabel)
				.addComponent( ContrastLabel)
				.addComponent( BlanktimeLabel)
				.addComponent( InvertedLabel)
			)
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.LEADING)
				.addComponent( backlight)
				.addComponent( contrast)
				.addComponent( blanktime)
				.addComponent( inverted)
			)
		);

		Layout.setVerticalGroup( Layout.createSequentialGroup()
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( BacklightLabel)
				.addComponent( backlight, javax.swing.GroupLayout.PREFERRED_SIZE,
							   javax.swing.GroupLayout.PREFERRED_SIZE,
							   javax.swing.GroupLayout.PREFERRED_SIZE)
			)
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( ContrastLabel)
				.addComponent( contrast, javax.swing.GroupLayout.PREFERRED_SIZE,
							   javax.swing.GroupLayout.PREFERRED_SIZE,
							   javax.swing.GroupLayout.PREFERRED_SIZE)
			)
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( BlanktimeLabel)
				.addComponent( blanktime, javax.swing.GroupLayout.PREFERRED_SIZE,
							   javax.swing.GroupLayout.PREFERRED_SIZE,
							   javax.swing.GroupLayout.PREFERRED_SIZE)
			)
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( InvertedLabel)
				.addComponent( inverted, javax.swing.GroupLayout.PREFERRED_SIZE,
							   javax.swing.GroupLayout.PREFERRED_SIZE,
							   javax.swing.GroupLayout.PREFERRED_SIZE)
				)
		);
	}

	public void set()
	{
	  	backlight.attachValue( system.getStatusBacklight());
	  	contrast.attachValue( system.getStatusContrast());
		blanktime.attachValue( system.getStatusBlankTime());
	  	inverted.attachValue( system.getStatusInverted());
	}
}
