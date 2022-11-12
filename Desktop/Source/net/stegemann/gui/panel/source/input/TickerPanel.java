package net.stegemann.gui.panel.source.input;

import javax.swing.GroupLayout;
import javax.swing.JLabel;

import net.stegemann.configuration.Configuration;
import net.stegemann.configuration.Signal;
import net.stegemann.configuration.source.input.Input;
import net.stegemann.configuration.source.input.Ticker;
import net.stegemann.gui.components.BoolComponent;
import net.stegemann.gui.components.NumberSliderComponent;
import net.stegemann.gui.components.source.DigitalInputIdComponent;

public class TickerPanel extends TypePanel
{
	private static final long serialVersionUID = -5004190976118096408L;

	private final DigitalInputIdComponent lowInputId;
	private final DigitalInputIdComponent highInputId;
	private final NumberSliderComponent init;
	private final NumberSliderComponent step;
	private final BoolComponent store;
	private final NumberSliderComponent top;
	private final NumberSliderComponent bottom;

	public TickerPanel( Configuration UseConfiguration)
	{
		super( UseConfiguration);

		JLabel LowInputIdLabel = new JLabel( "Anschluss Tief:");
		lowInputId = new DigitalInputIdComponent();

		JLabel HighInputIdLabel = new JLabel( "Anschluss Hoch:");
		highInputId = new DigitalInputIdComponent();

		JLabel InitLabel = new JLabel( "Start:");
		init = new NumberSliderComponent( Signal.MINIMUM_VALUE / Input.INIT_SIGNAL_PER_VALUE,
				                          Signal.MAXIMUM_VALUE / Input.INIT_SIGNAL_PER_VALUE);

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
				.addComponent( LowInputIdLabel)
				.addComponent( HighInputIdLabel)
				.addComponent( InitLabel)
				.addComponent( StepLabel)
				.addComponent( StoreLabel)
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

	public void set( Ticker ticker)
	{
		super.set();

		lowInputId.attachValue( ticker.getLowInputId());
		highInputId.attachValue( ticker.getHighInputId());
		init.attachValue( ticker.getInit());
		step.attachValue( ticker.getStep());
		store.attachValue( ticker.getStore());
		top.attachValue( ticker.getTop());
		bottom.attachValue( ticker.getBottom());
	}
}
