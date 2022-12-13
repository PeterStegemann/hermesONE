package net.stegemann.gui.components;

import net.stegemann.configuration.Battery;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.ValueOutOfRangeException;
import net.stegemann.gui.Constants;
import net.stegemann.misc.Utility;

import javax.swing.*;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.io.Serial;
import java.util.Hashtable;

public class VoltageComponent extends JPanel implements ActionListener, ChangeListener, FocusListener
{
	@Serial
	private static final long serialVersionUID = 2578331261470516782L;

	private Number value;

	private final JSlider slider;
	private final JTextField textField;

	public VoltageComponent()
	{
		this( Battery.VOLTAGE_MINIMUM, Battery.VOLTAGE_MAXIMUM);
	}

	public VoltageComponent( int minimum, int maximum)
	{
		slider = new JSlider();
		slider.setPaintLabels( true);
		slider.setPaintTicks( true);
		slider.setPaintTrack( true);
		slider.setMinimum( minimum);
		slider.setMaximum( maximum);
		slider.setMinorTickSpacing( 10);
		slider.setMajorTickSpacing( 20);

		Hashtable< Integer, JLabel> labelTable = new Hashtable<>();
		labelTable.put(minimum, new JLabel( Utility.formatVoltage(minimum)));
		labelTable.put( maximum / 4 * 1, new JLabel( Utility.formatVoltage( maximum / 4 * 1)));
		labelTable.put( maximum / 4 * 2, new JLabel( Utility.formatVoltage( maximum / 4 * 2)));
		labelTable.put( maximum / 4 * 3, new JLabel( Utility.formatVoltage( maximum / 4 * 3)));
		labelTable.put( maximum, new JLabel( Utility.formatVoltage( maximum)));

		slider.setLabelTable( labelTable);
		slider.addChangeListener( this);

		textField = new JTextField( Constants.DEFAULT_SOURCEFIELD_WIDTH);
		textField.setHorizontalAlignment( JTextField.RIGHT);
		textField.addActionListener( this);
		textField.addFocusListener( this);

		// No gaps within component.
		setLayout( new FlowLayout( 0, 0, 0));

		add( slider);
		add( textField);
	}

	public void attachValue( Number useValue)
	{
		value = useValue;

		set();
	}

	private void set()
	{
		int value = this.value.getValue();

		textField.setText( Utility.formatVoltage( value));

		if( slider != null)
		{
			slider.setValue( value);
		}
	}

	private void setFromSlider()
	{
		if( slider == null)
		{
			return;
		}

		try
		{
			value.setValue( slider.getValue());
		}
		catch( Exception ignored)
		{
			// Ignore bad input and revert to old.
		}

		set();
	}

	private void setFromText()
	{
		try
		{
			value.setValue( Utility.parseVoltage( textField.getText()));
		}
		catch( ValueOutOfRangeException ignored)
		{
			// Ignore bad input and revert to old.
		}

		set();
	}

	@Override
	public void stateChanged( ChangeEvent event)
	{
		setFromSlider();
	}

	@Override
	public void actionPerformed( ActionEvent event)
	{
		setFromText();
	}

	@Override
	public void focusGained( FocusEvent event)
	{
	}

	@Override
	public void focusLost( FocusEvent event)
	{
		setFromText();
	}
}
