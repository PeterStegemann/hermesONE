package net.stegemann.gui.panel.source.input;

import net.stegemann.configuration.Configuration;
import net.stegemann.configuration.source.input.Analog;
import net.stegemann.gui.components.source.AnalogInputIdComponent;

import javax.swing.*;
import java.io.Serial;

public class AnalogPanel extends TypePanel
{
	@Serial
	private static final long serialVersionUID = 1327889084392277200L;

	private final AnalogInputIdComponent inputId;

	public AnalogPanel( Configuration UseConfiguration)
	{
		super( UseConfiguration);

		JLabel InputIdLabel = new JLabel( "Anschluss:");
		inputId = new AnalogInputIdComponent();

		// Layout elements.
		GroupLayout Layout = new GroupLayout( this);
		setLayout( Layout);

		Layout.setHorizontalGroup( Layout.createSequentialGroup()
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.TRAILING)
				.addComponent( InputIdLabel, javax.swing.GroupLayout.PREFERRED_SIZE,
											 javax.swing.GroupLayout.PREFERRED_SIZE,
											 javax.swing.GroupLayout.PREFERRED_SIZE)
			)
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.LEADING)
				.addComponent( inputId, javax.swing.GroupLayout.PREFERRED_SIZE,
										javax.swing.GroupLayout.PREFERRED_SIZE,
										javax.swing.GroupLayout.PREFERRED_SIZE)
			)
		);

		Layout.setVerticalGroup( Layout.createSequentialGroup()
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( InputIdLabel)
				.addComponent( inputId, javax.swing.GroupLayout.PREFERRED_SIZE,
										javax.swing.GroupLayout.PREFERRED_SIZE,
										javax.swing.GroupLayout.PREFERRED_SIZE)
			)
		);
	}

	public void set( Analog analog)
	{
		super.set();
		
		inputId.attachValue( analog.getInputId());
	}
}
