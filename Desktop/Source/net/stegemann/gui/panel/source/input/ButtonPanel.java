package net.stegemann.gui.panel.source.input;

import javax.swing.GroupLayout;
import javax.swing.JLabel;

import net.stegemann.configuration.Configuration;
import net.stegemann.configuration.Signal;
import net.stegemann.configuration.source.input.Button;
import net.stegemann.configuration.source.input.Input;
import net.stegemann.gui.components.BoolComponent;
import net.stegemann.gui.components.NumberSliderComponent;
import net.stegemann.gui.components.source.DigitalInputIdComponent;

public class ButtonPanel extends TypePanel
{
	private static final long serialVersionUID = 6542977472347597121L;

	private final DigitalInputIdComponent inputId;
	private final NumberSliderComponent init;
	private final BoolComponent store;
	private final BoolComponent toggle;
	private final NumberSliderComponent top;
	private final NumberSliderComponent bottom;

	public ButtonPanel( Configuration UseConfiguration)
	{
		super( UseConfiguration);

		JLabel InputIdLabel = new JLabel( "Anschluss:");
		inputId = new DigitalInputIdComponent();

		JLabel InitLabel = new JLabel( "Start:");
		init = new NumberSliderComponent( Signal.MINIMUM_VALUE / Input.INIT_SIGNAL_PER_VALUE,
										  Signal.MAXIMUM_VALUE / Input.INIT_SIGNAL_PER_VALUE);

		JLabel StoreLabel = new JLabel( "Speichern:");
		store = new BoolComponent();

		JLabel ToggleLabel = new JLabel( "Wechseln:");
		toggle = new BoolComponent();

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
				.addComponent( InputIdLabel)
				.addComponent( InitLabel)
				.addComponent( StoreLabel)
				.addComponent( ToggleLabel)
				.addComponent( TopLabel)
				.addComponent( BottomLabel)
			)
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.LEADING)
				.addComponent( inputId, javax.swing.GroupLayout.PREFERRED_SIZE,
										javax.swing.GroupLayout.PREFERRED_SIZE,
										javax.swing.GroupLayout.PREFERRED_SIZE)
				.addComponent( init, javax.swing.GroupLayout.PREFERRED_SIZE,
									 javax.swing.GroupLayout.PREFERRED_SIZE,
									 javax.swing.GroupLayout.PREFERRED_SIZE)
				.addComponent( store, javax.swing.GroupLayout.PREFERRED_SIZE,
									  javax.swing.GroupLayout.PREFERRED_SIZE,
									  javax.swing.GroupLayout.PREFERRED_SIZE)
				.addComponent( toggle)
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
				.addComponent( InputIdLabel)
				.addComponent( inputId, javax.swing.GroupLayout.PREFERRED_SIZE,
										javax.swing.GroupLayout.PREFERRED_SIZE,
										javax.swing.GroupLayout.PREFERRED_SIZE)
			)
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( InitLabel)
				.addComponent( init, javax.swing.GroupLayout.PREFERRED_SIZE,
									 javax.swing.GroupLayout.PREFERRED_SIZE,
									 javax.swing.GroupLayout.PREFERRED_SIZE)
			)
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( StoreLabel)
				.addComponent( store)
			)
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( ToggleLabel)
				.addComponent( toggle)
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

	public void set( Button button)
	{
		super.set();

		inputId.attachValue( button.getInputId());
		init.attachValue( button.getInit());
		store.attachValue( button.getStore());
		toggle.attachValue( button.getToggle());
		top.attachValue( button.getTop());
		bottom.attachValue( button.getBottom());
	}
}
