package net.stegemann.gui.panel.system;

import java.util.ArrayList;
import java.util.List;

import javax.swing.GroupLayout;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.SwingConstants;
import javax.swing.GroupLayout.Group;

import net.stegemann.configuration.Calibration;
import net.stegemann.configuration.Calibrations;
import net.stegemann.gui.components.NumberComponent;

public class CalibrationsPanel extends JPanel
{
	private static final long serialVersionUID = 660474941338897581L;

	private class CalibrationLine
	{
		private final JLabel label;
		private final NumberComponent high;
		private final NumberComponent center;
		private final NumberComponent low;

		public CalibrationLine()
		{
			label = new JLabel();
			label.setHorizontalAlignment( SwingConstants.LEADING);
			high = new NumberComponent( 5);
//			high.setHorizontalAlignment( SwingConstants.TRAILING);
			center = new NumberComponent( 5);
//			center.setHorizontalAlignment( SwingConstants.TRAILING);
			low = new NumberComponent( 5);
//			low.setHorizontalAlignment( SwingConstants.TRAILING);
		}

		public JLabel GetLabel()
		{
			return label;
		}

		public NumberComponent GetHigh()
		{
			return high;
		}

		public NumberComponent GetCenter()
		{
			return center;
		}

		public NumberComponent GetLow()
		{
			return low;
		}
	}

	private final Calibrations calibrations;

	private final JLabel highLabel;
	private final JLabel centerLabel;
	private final JLabel lowLabel;

	private final List< CalibrationLine> calibrationLines = new ArrayList< CalibrationLine>();

	public CalibrationsPanel( Calibrations UseCalibrations)
	{
		calibrations = UseCalibrations;

		highLabel = new JLabel( "Oben");
		highLabel.setHorizontalAlignment( SwingConstants.CENTER);

		centerLabel = new JLabel( "Mitte");
		centerLabel.setHorizontalAlignment( SwingConstants.CENTER);

		lowLabel = new JLabel( "Unten");
		lowLabel.setHorizontalAlignment( SwingConstants.CENTER);

		for( int Index = 0; Index < calibrations.getCalibrationCount(); Index++)
		{
			CalibrationLine NewCalibrationLine = new CalibrationLine();
			calibrationLines.add( NewCalibrationLine);

			NewCalibrationLine.GetLabel().setText( "" + ( char)(( Index / 2) + 'A') + ( Index % 2));
		}

		JLabel CornerLabel = new JLabel();

		// Layout elements.
		GroupLayout Layout = new GroupLayout( this);
		setLayout( Layout);

//		layout.setAutoCreateGaps( true);
		Layout.setAutoCreateContainerGaps( true);

		Group HorizontalGroup = Layout.createSequentialGroup();

		Group LabelColumnGroup =
			Layout.createParallelGroup( GroupLayout.Alignment.CENTER).addComponent( CornerLabel);
		Group HighColumnGroup =
			Layout.createParallelGroup( GroupLayout.Alignment.CENTER).addComponent( highLabel);
		Group CenterColumnGroup =
			Layout.createParallelGroup( GroupLayout.Alignment.CENTER).addComponent( centerLabel);
		Group LowColumnGroup = 
			Layout.createParallelGroup( GroupLayout.Alignment.CENTER).addComponent( lowLabel);

		for( CalibrationLine CurrentCalibrationLine:calibrationLines)
		{
			LabelColumnGroup.addComponent( CurrentCalibrationLine.GetLabel());
			HighColumnGroup.addComponent( CurrentCalibrationLine.GetHigh());
			CenterColumnGroup.addComponent( CurrentCalibrationLine.GetCenter());
			LowColumnGroup.addComponent( CurrentCalibrationLine.GetLow());
		}

		HorizontalGroup.addGroup( LabelColumnGroup);
		HorizontalGroup.addGroup( HighColumnGroup);
		HorizontalGroup.addGroup( CenterColumnGroup);
		HorizontalGroup.addGroup( LowColumnGroup);

		Layout.setHorizontalGroup( HorizontalGroup);

		Group VerticalGroup = Layout.createSequentialGroup()
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( CornerLabel)
				.addComponent( highLabel)
				.addComponent( centerLabel)
				.addComponent( lowLabel)
			);

		for( CalibrationLine CurrentCalibrationLine: calibrationLines)
		{
			VerticalGroup.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( CurrentCalibrationLine.GetLabel())
				.addComponent( CurrentCalibrationLine.GetHigh())
				.addComponent( CurrentCalibrationLine.GetCenter())
				.addComponent( CurrentCalibrationLine.GetLow())
			);
		}

		Layout.setVerticalGroup( VerticalGroup);
	}

	public void Set()
	{
		int Index = 0;

		for( Calibration CurrentCalibration: calibrations)
		{
			CalibrationLine UseCalibrationLine = calibrationLines.get( Index);

			UseCalibrationLine.GetHigh().attachValue( CurrentCalibration.getHigh());
			UseCalibrationLine.GetCenter().attachValue( CurrentCalibration.getCenter());
			UseCalibrationLine.GetLow().attachValue( CurrentCalibration.getLow());

			Index++;
		}
	}
}
