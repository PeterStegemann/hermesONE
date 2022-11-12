package net.stegemann.gui.panel.source.input;

import javax.swing.GroupLayout;
import javax.swing.JLabel;

import net.stegemann.configuration.Configuration;
import net.stegemann.configuration.Signal;
import net.stegemann.configuration.source.input.Input;
import net.stegemann.configuration.source.input.Rotary;
import net.stegemann.gui.components.BoolComponent;
import net.stegemann.gui.components.NumberSliderComponent;
import net.stegemann.gui.components.source.DigitalInputIdComponent;

public class RotaryPanel extends TypePanel
{
	private static final long serialVersionUID = -3974584381352474392L;

	private final DigitalInputIdComponent aInputId;
	private final DigitalInputIdComponent bInputId;
	private final NumberSliderComponent init;
	private final NumberSliderComponent step;
	private final BoolComponent store;
	private final NumberSliderComponent top;
	private final NumberSliderComponent bottom;

	public RotaryPanel( Configuration UseConfiguration)
	{
		super( UseConfiguration);

		JLabel AInputIdLabel = new JLabel( "Anschluss A:");
		aInputId = new DigitalInputIdComponent();

		JLabel BInputIdLabel = new JLabel( "Anschluss B:");
		bInputId = new DigitalInputIdComponent();

		JLabel InitLabel = new JLabel( "Start:");
		init = new NumberSliderComponent( Signal.MINIMUM_VALUE / Input.INIT_SIGNAL_PER_VALUE,
				                          Signal.MAXIMUM_VALUE /Input.INIT_SIGNAL_PER_VALUE);

		JLabel StepLabel = new JLabel( "Schritt:");
		step = new NumberSliderComponent( Signal.MINIMUM_VALUE / Input.STEP_SIGNAL_PER_VALUE,
										  Signal.MAXIMUM_VALUE / Input.STEP_SIGNAL_PER_VALUE);

		JLabel StoreLabel = new JLabel( "Speichern:");
		store = new BoolComponent();

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
				.addComponent( AInputIdLabel)
				.addComponent( BInputIdLabel)
				.addComponent( InitLabel)
				.addComponent( StepLabel)
				.addComponent( StoreLabel)
				.addComponent( TopLabel)
				.addComponent( BottomLabel)
			)
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.LEADING)
				.addComponent( aInputId, javax.swing.GroupLayout.PREFERRED_SIZE,
										 javax.swing.GroupLayout.PREFERRED_SIZE,
										 javax.swing.GroupLayout.PREFERRED_SIZE)
				.addComponent( bInputId,  javax.swing.GroupLayout.PREFERRED_SIZE,
										  javax.swing.GroupLayout.PREFERRED_SIZE,
										  javax.swing.GroupLayout.PREFERRED_SIZE)
				.addComponent( init)
				.addComponent( step)
				.addComponent( store)
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
				.addComponent( AInputIdLabel)
				.addComponent( aInputId, javax.swing.GroupLayout.PREFERRED_SIZE,
										 javax.swing.GroupLayout.PREFERRED_SIZE,
										 javax.swing.GroupLayout.PREFERRED_SIZE)
			)
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( BInputIdLabel)
				.addComponent( bInputId, javax.swing.GroupLayout.PREFERRED_SIZE,
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
				.addComponent( StepLabel)
				.addComponent( step, javax.swing.GroupLayout.PREFERRED_SIZE,
									 javax.swing.GroupLayout.PREFERRED_SIZE,
									 javax.swing.GroupLayout.PREFERRED_SIZE)
			)
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( StoreLabel)
				.addComponent( store)
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

	public void set( Rotary rotary)
	{
		super.set();

		aInputId.attachValue( rotary.getAInputId());
		bInputId.attachValue( rotary.getBInputId());
		init.attachValue( rotary.getInit());
		step.attachValue( rotary.getStep());
		store.attachValue( rotary.getStore());
		top.attachValue( rotary.getTop());
		bottom.attachValue( rotary.getBottom());
	}
}
