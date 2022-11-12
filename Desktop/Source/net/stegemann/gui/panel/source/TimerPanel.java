package net.stegemann.gui.panel.source;

import javax.swing.GroupLayout;
import javax.swing.JLabel;

import net.stegemann.configuration.Configuration;
import net.stegemann.configuration.Model;
import net.stegemann.configuration.Signal;
import net.stegemann.configuration.source.Timer;
import net.stegemann.gui.components.NumberSliderComponent;
import net.stegemann.gui.components.TimeSliderComponent;
import net.stegemann.gui.components.source.SourceComponent;

public class TimerPanel extends SpecificSourcePanel
{
	private static final long serialVersionUID = -4900020774311747722L;

	private final TimeSliderComponent initTime;
	private final TimeSliderComponent currentTime;
	private final SourceComponent trigger;
	private final NumberSliderComponent triggerLowLimit;
	private final NumberSliderComponent triggerHighLimit;
	private final TimeSliderComponent warnLowTime;
	private final TimeSliderComponent warnCriticalTime;
	private final TimeSliderComponent warnPauseTime;

	public TimerPanel( Configuration UseConfiguration)
	{
		super( UseConfiguration);

		JLabel InitTimeLabel = new JLabel( "Start:");
		initTime = new TimeSliderComponent( 0, Timer.TIME_MAXIMUM_VALUE);

		JLabel CurrentTimeLabel = new JLabel( "Aktuell:");
		currentTime = new TimeSliderComponent( 0, Timer.TIME_MAXIMUM_VALUE);

		JLabel TriggerLabel = new JLabel( "Trigger:");
		trigger = new SourceComponent();

		JLabel TriggerLowLimitLabel = new JLabel( "Limit Unten:");
		triggerLowLimit =
			new NumberSliderComponent( Signal.MINIMUM_VALUE / Timer.TRIGGER_SIGNAL_PER_VALUE,
									   Signal.MAXIMUM_VALUE / Timer.TRIGGER_SIGNAL_PER_VALUE);

		JLabel TriggerHighLimitLabel = new JLabel( "Limit Oben:");
		triggerHighLimit =
			new NumberSliderComponent( Signal.MINIMUM_VALUE / Timer.TRIGGER_SIGNAL_PER_VALUE,
									   Signal.MAXIMUM_VALUE / Timer.TRIGGER_SIGNAL_PER_VALUE);

		JLabel WarnLowTimeLabel = new JLabel( "Alarm niedrig:");
		warnLowTime = new TimeSliderComponent( 0, Timer.TIME_MAXIMUM_VALUE);

		JLabel WarnCriticalTimeLabel = new JLabel( "Alarm kritisch:");
		warnCriticalTime = new TimeSliderComponent( 0, Timer.TIME_MAXIMUM_VALUE);

		JLabel WarnPauseTimeLabel = new JLabel( "Alarm pause:");
		warnPauseTime = new TimeSliderComponent(  0, Timer.TIME_MAXIMUM_VALUE);

		// Layout elements.
		GroupLayout Layout = new GroupLayout( this);
		setLayout( Layout);

//		Layout.setAutoCreateGaps( true);
//		Layout.setAutoCreateContainerGaps( true);

		Layout.setHorizontalGroup( Layout.createSequentialGroup()
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.TRAILING)
				.addComponent( InitTimeLabel)
				.addComponent( CurrentTimeLabel)
				.addComponent( TriggerLabel)
				.addComponent( TriggerLowLimitLabel)
				.addComponent( TriggerHighLimitLabel)
				.addComponent( WarnLowTimeLabel)
				.addComponent( WarnCriticalTimeLabel)
				.addComponent( WarnPauseTimeLabel)
			)
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.LEADING)
				.addComponent( initTime, javax.swing.GroupLayout.PREFERRED_SIZE,
									   	 javax.swing.GroupLayout.PREFERRED_SIZE,
									   	 javax.swing.GroupLayout.PREFERRED_SIZE)
				.addComponent( currentTime, javax.swing.GroupLayout.PREFERRED_SIZE,
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
				.addComponent( warnLowTime, javax.swing.GroupLayout.PREFERRED_SIZE,
											javax.swing.GroupLayout.PREFERRED_SIZE,
											javax.swing.GroupLayout.PREFERRED_SIZE)
				.addComponent( warnCriticalTime, javax.swing.GroupLayout.PREFERRED_SIZE,
												 javax.swing.GroupLayout.PREFERRED_SIZE,
												 javax.swing.GroupLayout.PREFERRED_SIZE)
				.addComponent( warnPauseTime, javax.swing.GroupLayout.PREFERRED_SIZE,
											  javax.swing.GroupLayout.PREFERRED_SIZE,
											  javax.swing.GroupLayout.PREFERRED_SIZE)
			)
		);

		Layout.setVerticalGroup( Layout.createSequentialGroup()
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( InitTimeLabel)
				.addComponent( initTime, javax.swing.GroupLayout.PREFERRED_SIZE,
										 		 javax.swing.GroupLayout.PREFERRED_SIZE,
										 		 javax.swing.GroupLayout.PREFERRED_SIZE)
			)
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( CurrentTimeLabel)
				.addComponent( currentTime, javax.swing.GroupLayout.PREFERRED_SIZE,
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
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( WarnLowTimeLabel)
				.addComponent( warnLowTime, javax.swing.GroupLayout.PREFERRED_SIZE,
													 javax.swing.GroupLayout.PREFERRED_SIZE,
													 javax.swing.GroupLayout.PREFERRED_SIZE)
			)
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( WarnCriticalTimeLabel)
				.addComponent( warnCriticalTime, javax.swing.GroupLayout.PREFERRED_SIZE,
												 			javax.swing.GroupLayout.PREFERRED_SIZE,
												 			javax.swing.GroupLayout.PREFERRED_SIZE)
			)
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( WarnPauseTimeLabel)
				.addComponent( warnPauseTime, javax.swing.GroupLayout.PREFERRED_SIZE,
											  			javax.swing.GroupLayout.PREFERRED_SIZE,
											  			javax.swing.GroupLayout.PREFERRED_SIZE)
			)
		);		
	}

	public void set( Model UseModel, Timer UseSource)
	{
		super.set( UseModel, UseSource);

		initTime.attachValue( UseSource.getInitTime());
		currentTime.attachValue( UseSource.getCurrentTime());

		trigger.setSourcesView( sourcesViewWithoutFixed);
		trigger.attachValue( UseSource.getTrigger());

		triggerLowLimit.attachValue( UseSource.getTriggerLowLimit());
		triggerHighLimit.attachValue( UseSource.getTriggerHighLimit());

		warnLowTime.attachValue( UseSource.getWarnLowTime());
		warnCriticalTime.attachValue( UseSource.getWarnCriticalTime());
		warnPauseTime.attachValue( UseSource.getWarnPauseTime());
	}
}
