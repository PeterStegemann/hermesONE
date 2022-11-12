package net.stegemann.gui.panel.source.input;

import javax.swing.GroupLayout;
import javax.swing.JLabel;

import net.stegemann.configuration.Configuration;
import net.stegemann.configuration.Signal;
import net.stegemann.configuration.source.input.Input;
import net.stegemann.configuration.source.input.Switch;
import net.stegemann.gui.components.NumberSliderComponent;
import net.stegemann.gui.components.source.DigitalInputIdComponent;

public class SwitchPanel extends TypePanel
{
	private static final long serialVersionUID = 1609882007457371119L;

	private final DigitalInputIdComponent lowInputId;
	private final DigitalInputIdComponent highInputId;
	private final NumberSliderComponent top;
	private final NumberSliderComponent bottom;

	public SwitchPanel( Configuration UseConfiguration)
	{
		super( UseConfiguration);

		JLabel LowInputIdLabel = new JLabel( "Anschluss Tief:");
		lowInputId = new DigitalInputIdComponent();

		JLabel HighInputIdLabel = new JLabel( "Anschluss Hoch:");
		highInputId = new DigitalInputIdComponent();

		JLabel TopLabel = new JLabel( "Oben:");
		top = new NumberSliderComponent( Signal.MINIMUM_VALUE / Input.TOP_SIGNAL_PER_VALUE,
										 Signal.MAXIMUM_VALUE / Input.TOP_SIGNAL_PER_VALUE);

		JLabel BottomLabel = new JLabel( "Unten:");
		bottom = new NumberSliderComponent( Signal.MINIMUM_VALUE / Input.BOTTOM_SIGNAL_PER_VALUE,
											Signal.MAXIMUM_VALUE / Input.BOTTOM_SIGNAL_PER_VALUE);

		// Layout elements.
		GroupLayout Layout = new GroupLayout( this);
		setLayout( Layout);

		Layout.setHorizontalGroup( Layout.createSequentialGroup()
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.TRAILING)
				.addComponent( LowInputIdLabel)
				.addComponent( HighInputIdLabel)
				.addComponent( TopLabel)
				.addComponent( BottomLabel)
			)
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.LEADING)
				.addComponent( lowInputId, javax.swing.GroupLayout.PREFERRED_SIZE,
										   javax.swing.GroupLayout.PREFERRED_SIZE,
										   javax.swing.GroupLayout.PREFERRED_SIZE)
				.addComponent( highInputId, javax.swing.GroupLayout.PREFERRED_SIZE,
											javax.swing.GroupLayout.PREFERRED_SIZE,
											javax.swing.GroupLayout.PREFERRED_SIZE)
				.addComponent( top, javax.swing.GroupLayout.PREFERRED_SIZE,
									javax.swing.GroupLayout.PREFERRED_SIZE,
									javax.swing.GroupLayout.PREFERRED_SIZE)
				.addComponent( bottom, javax.swing.GroupLayout.PREFERRED_SIZE,
									   javax.swing.GroupLayout.PREFERRED_SIZE,
									   javax.swing.GroupLayout.PREFERRED_SIZE)
			)
		);

		Layout.setVerticalGroup( Layout.createSequentialGroup()
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( LowInputIdLabel)
				.addComponent( lowInputId, javax.swing.GroupLayout.PREFERRED_SIZE,
										   javax.swing.GroupLayout.PREFERRED_SIZE,
										   javax.swing.GroupLayout.PREFERRED_SIZE)
			)
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( HighInputIdLabel)
				.addComponent( highInputId, javax.swing.GroupLayout.PREFERRED_SIZE,
											javax.swing.GroupLayout.PREFERRED_SIZE,
											javax.swing.GroupLayout.PREFERRED_SIZE)
			)
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
					.addComponent( TopLabel)
					.addComponent( top, javax.swing.GroupLayout.PREFERRED_SIZE,
										javax.swing.GroupLayout.PREFERRED_SIZE,
										javax.swing.GroupLayout.PREFERRED_SIZE)
			)
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( BottomLabel)
				.addComponent( bottom, javax.swing.GroupLayout.PREFERRED_SIZE,
									   javax.swing.GroupLayout.PREFERRED_SIZE,
									   javax.swing.GroupLayout.PREFERRED_SIZE)
			)
		);
	}

	public void set( Switch switsch)
	{
		super.set();

		lowInputId.attachValue( switsch.getLowInputId());
		highInputId.attachValue( switsch.getHighInputId());
		top.attachValue( switsch.getTop());
		bottom.attachValue( switsch.getBottom());
	}
}
