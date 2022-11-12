package net.stegemann.gui.panel.system;

import javax.swing.GroupLayout;
import javax.swing.JLabel;
import javax.swing.JPanel;

import net.stegemann.configuration.Battery;
import net.stegemann.gui.components.VoltageComponent;

public class BatteryWarnPanel extends JPanel
{
	private static final long serialVersionUID = 4998074720939787525L;

	private final Battery battery;

	private final VoltageComponent warnLowVoltage;
	private final VoltageComponent warnCriticalVoltage;

	public BatteryWarnPanel( Battery UseBattery)
	{
		battery = UseBattery;

		JLabel WarnLowVoltageLabel = new JLabel( "Niedrig:");
		warnLowVoltage = new VoltageComponent();
		JLabel WarnCriticalVoltageLabel = new JLabel( "Kritisch:");
		warnCriticalVoltage = new VoltageComponent();

		// Layout elements.
		GroupLayout Layout = new GroupLayout( this);
		setLayout( Layout);

//		Layout.setAutoCreateGaps( true);
		Layout.setAutoCreateContainerGaps( true);

		Layout.setHorizontalGroup( Layout.createSequentialGroup()
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.TRAILING)
				.addComponent( WarnLowVoltageLabel)
				.addComponent( WarnCriticalVoltageLabel)
			)
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.LEADING)
				.addComponent( warnLowVoltage)
				.addComponent( warnCriticalVoltage)
			)
		);

		Layout.setVerticalGroup( Layout.createSequentialGroup()
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( WarnLowVoltageLabel)
				.addComponent( warnLowVoltage, javax.swing.GroupLayout.PREFERRED_SIZE,
											   javax.swing.GroupLayout.PREFERRED_SIZE,
											   javax.swing.GroupLayout.PREFERRED_SIZE)
			)
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( WarnCriticalVoltageLabel)
				.addComponent( warnCriticalVoltage, javax.swing.GroupLayout.PREFERRED_SIZE,
													javax.swing.GroupLayout.PREFERRED_SIZE,
													javax.swing.GroupLayout.PREFERRED_SIZE)
			)
		);
	}

	public void Set()
	{
		warnLowVoltage.attachValue( battery.getWarnLowVoltage());
		warnCriticalVoltage.attachValue( battery.getWarnCriticalVoltage());
	}
}
