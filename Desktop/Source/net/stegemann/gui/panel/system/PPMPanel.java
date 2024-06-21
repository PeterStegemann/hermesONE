package net.stegemann.gui.panel.system;

import net.stegemann.configuration.PPM;
import net.stegemann.gui.components.BoolComponent;
import net.stegemann.gui.components.MilliSecondComponent;
import net.stegemann.gui.components.TextComponent;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.Serial;

public class PPMPanel extends Panel implements ActionListener
{
	@Serial
	private static final long serialVersionUID = 9037123876874360654L;

	private final PPM ppm;

	private final MilliSecondComponent ppmCenter;
	private final BoolComponent invertedPPM;
	private final TextComponent name;

	private final ChannelMappingsPanel channelMapping;

	public PPMPanel( PPM ppm)
	{
		this.ppm = ppm;

		JLabel nameLabel = new JLabel( "Name:");
		name = new TextComponent( net.stegemann.gui.Constants.DEFAULT_TEXTFIELD_WIDTH);

		JSeparator invertedPPMSeparator = new JSeparator( SwingConstants.VERTICAL);

		JLabel ppmCenterLabel = new JLabel( "PPM Mitte:");
		ppmCenter = new MilliSecondComponent( PPM.PPM_CENTER_MINIMUM, PPM.PPM_CENTER_MAXIMUM, 15);

		JLabel invertedPPMLabel = new JLabel( "PPM Invertieren");
		invertedPPM = new BoolComponent();

		JSeparator channelMappingsSeparator = new JSeparator();

		JLabel channelMappingLabel = new JLabel( "Kanalzuweisung:");
		channelMapping = new ChannelMappingsPanel( this.ppm.getChannelMappings());

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
                layout.createParallelGroup( GroupLayout.Alignment.LEADING)
				.addGroup
				(
                    layout.createSequentialGroup()
					.addComponent( nameLabel)
					.addComponent( name, javax.swing.GroupLayout.PREFERRED_SIZE,
										 javax.swing.GroupLayout.PREFERRED_SIZE,
										 javax.swing.GroupLayout.PREFERRED_SIZE)
				)
				.addGroup
				(
                    layout.createSequentialGroup()
					.addComponent( ppmCenterLabel)
					.addComponent( ppmCenter)
					.addComponent( invertedPPMSeparator)
					.addComponent( invertedPPM)
					.addComponent( invertedPPMLabel)
				)
				.addComponent( channelMappingsSeparator)
				.addComponent( channelMappingLabel)
				.addComponent( channelMapping)
			)
		);

		layout.setVerticalGroup
		(
            layout.createSequentialGroup()
			.addGroup
			(
                layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( nameLabel)
				.addComponent( name, javax.swing.GroupLayout.PREFERRED_SIZE,
									 javax.swing.GroupLayout.PREFERRED_SIZE,
									 javax.swing.GroupLayout.PREFERRED_SIZE)
			)
			.addGroup
			(
                layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( ppmCenterLabel)
				.addComponent( ppmCenter)
				.addComponent( invertedPPMSeparator)
				.addComponent( invertedPPM)
				.addComponent( invertedPPMLabel)
			)
			.addComponent( channelMappingsSeparator)
			.addComponent( channelMappingLabel)
			.addComponent( channelMapping)
		);
	}

	public void Set()
	{
	  	ppmCenter.attachValue( ppm.getCenter());
	  	invertedPPM.attachValue( ppm.getInverted());
	  	name.attachValue( ppm.getName());

		channelMapping.set();
	}

	@Override
	public void actionPerformed( ActionEvent event)
	{
	}
}
