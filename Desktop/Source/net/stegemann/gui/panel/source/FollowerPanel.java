package net.stegemann.gui.panel.source;

import net.stegemann.configuration.Configuration;
import net.stegemann.configuration.Model;
import net.stegemann.configuration.Signal;
import net.stegemann.configuration.source.Follower;
import net.stegemann.gui.components.NumberSliderComponent;
import net.stegemann.gui.components.source.SourceComponent;
import net.stegemann.gui.components.source.SourceWithVolumeComponent;

import javax.swing.*;
import java.io.Serial;

public class FollowerPanel extends SpecificSourcePanel
{
	@Serial
	private static final long serialVersionUID = 6553089473965508349L;

	private final SourceWithVolumeComponent target;
	private final SourceWithVolumeComponent step;
	private final SourceComponent trigger;
	private final NumberSliderComponent triggerLowLimit;
	private final NumberSliderComponent triggerHighLimit;

	public FollowerPanel( Configuration configuration)
	{
		super( configuration);

		JLabel TargetLabel = new JLabel( "Soll:");
		target = new SourceWithVolumeComponent( Signal.MINIMUM_VALUE / Follower.TARGET_SIGNAL_PER_VALUE,
									 Signal.MAXIMUM_VALUE / Follower.TARGET_SIGNAL_PER_VALUE);

		JLabel StepLabel = new JLabel( "Schrittweite:");
		step = new SourceWithVolumeComponent( Signal.MINIMUM_VALUE / Follower.STEP_SIGNAL_PER_VALUE,
								   Signal.MAXIMUM_VALUE / Follower.STEP_SIGNAL_PER_VALUE);

		JLabel TriggerLabel = new JLabel( "Trigger:");
		trigger = new SourceComponent();

		JLabel TriggerLowLimitLabel = new JLabel( "Limit Unten:");
		triggerLowLimit =
			new NumberSliderComponent( Signal.MINIMUM_VALUE / Follower.TRIGGER_SIGNAL_PER_VALUE,
									   Signal.MAXIMUM_VALUE / Follower.TRIGGER_SIGNAL_PER_VALUE);

		JLabel TriggerHighLimitLabel = new JLabel( "Limit Oben:");
		triggerHighLimit =
			new NumberSliderComponent( Signal.MINIMUM_VALUE / Follower.TRIGGER_SIGNAL_PER_VALUE,
									   Signal.MAXIMUM_VALUE / Follower.TRIGGER_SIGNAL_PER_VALUE);

		// Layout elements.
		GroupLayout Layout = new GroupLayout( this);
		setLayout( Layout);

//		Layout.setAutoCreateGaps( true);
//		Layout.setAutoCreateContainerGaps( true);

		Layout.setHorizontalGroup( Layout.createSequentialGroup()
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.TRAILING)
				.addComponent( TargetLabel)
				.addComponent( StepLabel)
				.addComponent( TriggerLabel)
				.addComponent( TriggerLowLimitLabel)
				.addComponent( TriggerHighLimitLabel)
			)
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.LEADING)
				.addComponent( target, javax.swing.GroupLayout.PREFERRED_SIZE,
									   javax.swing.GroupLayout.PREFERRED_SIZE,
									   javax.swing.GroupLayout.PREFERRED_SIZE)
				.addComponent( step, javax.swing.GroupLayout.PREFERRED_SIZE,
									 javax.swing.GroupLayout.PREFERRED_SIZE,
									 javax.swing.GroupLayout.PREFERRED_SIZE)
				.addComponent( trigger, javax.swing.GroupLayout.PREFERRED_SIZE,
										javax.swing.GroupLayout.PREFERRED_SIZE,
										javax.swing.GroupLayout.PREFERRED_SIZE)
				.addComponent( triggerLowLimit, javax.swing.GroupLayout.PREFERRED_SIZE,
												javax.swing.GroupLayout.PREFERRED_SIZE,
												javax.swing.GroupLayout.PREFERRED_SIZE)
				.addComponent( triggerHighLimit, javax.swing.GroupLayout.PREFERRED_SIZE,
												 javax.swing.GroupLayout.PREFERRED_SIZE,
												 javax.swing.GroupLayout.PREFERRED_SIZE)
			)
		);

		Layout.setVerticalGroup( Layout.createSequentialGroup()
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( TargetLabel)
				.addComponent( target, javax.swing.GroupLayout.PREFERRED_SIZE,
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
				.addComponent( TriggerLabel)
				.addComponent( trigger, javax.swing.GroupLayout.PREFERRED_SIZE,
										javax.swing.GroupLayout.PREFERRED_SIZE,
										javax.swing.GroupLayout.PREFERRED_SIZE)
			)
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( TriggerLowLimitLabel)
				.addComponent( triggerLowLimit, javax.swing.GroupLayout.PREFERRED_SIZE,
												javax.swing.GroupLayout.PREFERRED_SIZE,
												javax.swing.GroupLayout.PREFERRED_SIZE)
			)
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( TriggerHighLimitLabel)
				.addComponent( triggerHighLimit, javax.swing.GroupLayout.PREFERRED_SIZE,
												 javax.swing.GroupLayout.PREFERRED_SIZE,
												 javax.swing.GroupLayout.PREFERRED_SIZE)
			)
		);
	}

	public void set( Model model, Follower follower)
	{
		super.set( model, follower);

		target.set( sourcesViewWithFixed, follower.getTarget());
		step.set( sourcesViewWithFixed, follower.getStep());

		trigger.setSourcesView( sourcesViewWithoutFixed);
		trigger.attachValue( follower.getTrigger());

		triggerLowLimit.attachValue( follower.getTriggerLowLimit());
		triggerHighLimit.attachValue( follower.getTriggerHighLimit());
	}
}
