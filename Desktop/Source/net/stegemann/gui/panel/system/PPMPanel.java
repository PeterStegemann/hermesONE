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

	public PPMPanel( PPM UsePPM)
	{
		ppm = UsePPM;

		JLabel NameLabel = new JLabel( "Name:");
		name = new TextComponent( net.stegemann.gui.Constants.DEFAULT_TEXTFIELD_WIDTH);

		JSeparator InvertedPPMSeparator = new JSeparator( SwingConstants.VERTICAL);

		JLabel PPMCenterLabel = new JLabel( "PPM Mitte:");
		ppmCenter = new MilliSecondComponent( PPM.PPM_CENTER_MINIMUM, PPM.PPM_CENTER_MAXIMUM, 15);

		JLabel InvertedPPMLabel = new JLabel( "PPM Invertieren");
		invertedPPM = new BoolComponent();

		JSeparator ChannelMappingsSeparator = new JSeparator();

		JLabel ChannelMappingLabel = new JLabel( "Kanalzuweisung:");
		channelMapping = new ChannelMappingsPanel( ppm.getChannelMappings());

		// Layout elements.
		GroupLayout Layout = new GroupLayout( this);
		setLayout( Layout);

//		Layout.setAutoCreateGaps( true);
		Layout.setAutoCreateContainerGaps( true);

		Layout.setHorizontalGroup( Layout.createSequentialGroup()
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.LEADING)
				.addGroup( Layout.createSequentialGroup()
					.addComponent( NameLabel)
					.addComponent( name, javax.swing.GroupLayout.PREFERRED_SIZE,
										 javax.swing.GroupLayout.PREFERRED_SIZE,
										 javax.swing.GroupLayout.PREFERRED_SIZE)
				)
				.addGroup( Layout.createSequentialGroup()
					.addComponent( PPMCenterLabel)
					.addComponent( ppmCenter)
					.addComponent( InvertedPPMSeparator)
					.addComponent( invertedPPM)
					.addComponent( InvertedPPMLabel)
				)
				.addComponent( ChannelMappingsSeparator)
				.addComponent( ChannelMappingLabel)
				.addComponent( channelMapping)
			)
		);

		Layout.setVerticalGroup( Layout.createSequentialGroup()
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( NameLabel)
				.addComponent( name, javax.swing.GroupLayout.PREFERRED_SIZE,
									 javax.swing.GroupLayout.PREFERRED_SIZE,
									 javax.swing.GroupLayout.PREFERRED_SIZE)
			)
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( PPMCenterLabel)
				.addComponent( ppmCenter)
				.addComponent( InvertedPPMSeparator)
				.addComponent( invertedPPM)
				.addComponent( InvertedPPMLabel)
			)
			.addComponent( ChannelMappingsSeparator)
			.addComponent( ChannelMappingLabel)
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
	public void actionPerformed( ActionEvent e)
	{
	}
}
