package net.stegemann.gui.panel.system;

import javax.swing.GroupLayout;
import javax.swing.JLabel;
import javax.swing.JPanel;

import net.stegemann.configuration.System;
import net.stegemann.gui.components.NumberSliderComponent;
import net.stegemann.gui.components.TimeSliderComponent;

public class SetupDisplayPanel extends JPanel
{
	private static final long serialVersionUID = 8256655246117794682L;

	private final System system;

	private final NumberSliderComponent backlight;
	private final TimeSliderComponent blanktime;

	public SetupDisplayPanel( System UseSystem)
	{
		system = UseSystem;

		JLabel BacklightLabel = new JLabel( "Licht:");
		backlight = new NumberSliderComponent( System.SYSTEM_BACKLIGHT_MINIMUM,
											   System.SYSTEM_BACKLIGHT_MAXIMUM);

		JLabel BlanktimeLabel = new JLabel( "Bildschirm Aus:");
		blanktime = new TimeSliderComponent( System.SYSTEM_BLANKTIME_MINIMUM,
											 System.SYSTEM_BLANKTIME_MAXIMUM);

		// Layout elements.
		GroupLayout Layout = new GroupLayout( this);
		setLayout( Layout);

//		Layout.setAutoCreateGaps( true);
		Layout.setAutoCreateContainerGaps( true);

		Layout.setHorizontalGroup( Layout.createSequentialGroup()
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.TRAILING)
				.addComponent( BacklightLabel)
				.addComponent( BlanktimeLabel)
			)
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.LEADING)
				.addComponent( blanktime)
				.addComponent( backlight)
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
				.addComponent( BlanktimeLabel)
				.addComponent( blanktime, javax.swing.GroupLayout.PREFERRED_SIZE,
						 javax.swing.GroupLayout.PREFERRED_SIZE,
						 javax.swing.GroupLayout.PREFERRED_SIZE)
			)
		);
	}

	public void set()
	{
		backlight.attachValue( system.getSetupBacklight());
		blanktime.attachValue( system.getSetupBlankTime());
	}
}
