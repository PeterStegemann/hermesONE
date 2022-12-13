package net.stegemann.gui.components;

import net.stegemann.configuration.type.Number;
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

public class TimeSliderComponent extends JPanel implements ActionListener, ChangeListener, FocusListener
{
	@Serial
	private static final long serialVersionUID = 6230884230632929614L;

	private Number value;

	private final int minimum;

	private final JSlider slider;
	private final JTextField textfield;

	public TimeSliderComponent( int minimum, int maximum)
	{
		this.minimum = minimum;

		int Range = maximum - minimum;

		slider = new JSlider();
		slider.setPaintLabels( true);
		slider.setPaintTicks( true);
		slider.setPaintTrack( true);
		slider.setMinimum( minimum);
		slider.setMaximum( maximum);
		slider.setMinorTickSpacing( Range / 10);
		slider.setMajorTickSpacing( Range / 5);

		Hashtable< Integer, JLabel> LabelTable = new Hashtable<>();
		LabelTable.put( minimum, new JLabel( "Aus"));
		LabelTable.put( minimum + ( Range / 2), new JLabel( Utility.formatTime( minimum + ( Range / 2))));
		LabelTable.put( maximum, new JLabel( Utility.formatTime( maximum)));

		slider.setLabelTable( LabelTable);
		slider.addChangeListener( this);

		textfield = new JTextField( Constants.DEFAULT_SOURCEFIELD_WIDTH);
		textfield.setHorizontalAlignment( JTextField.RIGHT);
		textfield.addActionListener( this);
		textfield.addFocusListener( this);

		// No gaps within component.
		setLayout( new FlowLayout( 0, 0, 0));

		add( slider);
		add( textfield);
	}

	public void attachValue( Number useValue)
	{
		value = useValue;

		set();
	}

	private void set()
	{
		int Value = value.getValue();

		if (Value == minimum)
		{
			textfield.setText( "Aus");
		}
		else
		{
			textfield.setText( Utility.formatTime( Value));
		}

		if( slider != null)
		{
			slider.setValue( Value);
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
		catch( Exception Reason)
		{
			// Ignore bad input and revert to old.
		}

		set();
	}

	private void setFromText()
	{
		try
		{
			String Text = textfield.getText();

			if( "Aus".equals( Text) == true)
			{
				value.setValue( minimum);
			}
			else
			{
				value.setValue( Utility.parseTime( Text));
			}
		}
		catch( Exception Reason)
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
