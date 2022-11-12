package net.stegemann.gui.panel.system;

import javax.swing.GroupLayout;
import javax.swing.JLabel;
import javax.swing.JPanel;

import net.stegemann.configuration.Battery;
import net.stegemann.gui.components.VoltageComponent;

public class BatteryCalibrationPanel extends JPanel
{
	private static final long serialVersionUID = 4998074720939787525L;

	private final Battery battery;

	private final VoltageComponent minimumVoltage;
	private final VoltageComponent maximumVoltage;
	private final VoltageComponent calibrationVoltage;

	public BatteryCalibrationPanel( Battery UseBattery)
	{
		battery = UseBattery;

		JLabel MinimumVoltageLabel = new JLabel( "Minimum:");
		minimumVoltage = new VoltageComponent();
		JLabel MaximumVoltageLabel = new JLabel( "Maximum:");
		maximumVoltage = new VoltageComponent();
		JLabel CalibrationVoltageLabel = new JLabel( "Kalibrierung:");
		calibrationVoltage = new VoltageComponent( Battery.CALIBRATION_VOLTAGE_MINIMUM,
												   Battery.CALIBRATION_VOLTAGE_MAXIMUM);

		// Layout elements.
		GroupLayout Layout = new GroupLayout( this);
		setLayout( Layout);

//		Layout.setAutoCreateGaps( true);
		Layout.setAutoCreateContainerGaps( true);

		Layout.setHorizontalGroup( Layout.createSequentialGroup()
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.TRAILING)
				.addComponent( MinimumVoltageLabel)
				.addComponent( MaximumVoltageLabel)
				.addComponent( CalibrationVoltageLabel)
			)
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.LEADING)
				.addComponent( minimumVoltage)
				.addComponent( maximumVoltage)
				.addComponent( calibrationVoltage)
			)
		);

		Layout.setVerticalGroup( Layout.createSequentialGroup()
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( MinimumVoltageLabel)
				.addComponent( minimumVoltage, javax.swing.GroupLayout.PREFERRED_SIZE,
						 javax.swing.GroupLayout.PREFERRED_SIZE,
						 javax.swing.GroupLayout.PREFERRED_SIZE)
			)
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( MaximumVoltageLabel)
				.addComponent( maximumVoltage, javax.swing.GroupLayout.PREFERRED_SIZE,
						 javax.swing.GroupLayout.PREFERRED_SIZE,
						 javax.swing.GroupLayout.PREFERRED_SIZE)
			)
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( CalibrationVoltageLabel)
				.addComponent( calibrationVoltage, javax.swing.GroupLayout.PREFERRED_SIZE,
						 javax.swing.GroupLayout.PREFERRED_SIZE,
						 javax.swing.GroupLayout.PREFERRED_SIZE)
			)
		);
	}

	public void set()
	{
		minimumVoltage.attachValue( battery.getMinimumVoltage());
		maximumVoltage.attachValue( battery.getMaximumVoltage());
		calibrationVoltage.attachValue( battery.getCalibrationVoltage());
	}
}
