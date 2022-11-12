package net.stegemann.gui.components;

import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.util.Hashtable;

import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSlider;
import javax.swing.JTextField;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

import net.stegemann.configuration.type.Number;
import net.stegemann.gui.Constants;
import net.stegemann.misc.Utility;

public class MilliSecondComponent extends JPanel
							   implements ActionListener, ChangeListener, FocusListener
{
	private static final long serialVersionUID = 1372270755854462788L;

	private Number value;

	// Display milliseconds with an offset.
	private int offset;

	private final JSlider slider;
	private final JTextField textfield;

	public MilliSecondComponent( int minimum, int maximum, int offset)
	{
		this.offset = offset;

		int Range = maximum - minimum;

		slider = new JSlider();
		slider.setPaintLabels( true);
		slider.setPaintTicks( true);
		slider.setPaintTrack( true);
		slider.setMinimum( minimum);
		slider.setMaximum( maximum);
		slider.setMinorTickSpacing( Range / 10);
		slider.setMajorTickSpacing( Range / 2);

		Hashtable< Integer, JLabel> LabelTable = new Hashtable< Integer, JLabel>();
		LabelTable.put( new Integer( minimum), new JLabel( Utility.formatMilliSeconds( minimum + offset)));
		LabelTable.put( new Integer( minimum + Range / 2 * 1),
				        new JLabel( Utility.formatMilliSeconds( minimum + offset + Range / 2 * 1)));
		LabelTable.put( new Integer( minimum + Range), new JLabel( Utility.formatMilliSeconds( maximum + offset)));

		slider.setLabelTable( LabelTable);
		slider.addChangeListener( this);

		textfield = new JTextField( Constants.DEFAULT_SOURCEFIELD_WIDTH);
		textfield.setHorizontalAlignment( JTextField.RIGHT);
		textfield.addActionListener( this);
		textfield.addFocusListener( this);

		// No gaps within component.
		setLayout( new FlowLayout( 0, 0, 0));

		if( slider != null)
		{
			add( slider);
		}

		add( textfield);
	}

	public void attachValue( Number UseValue)
	{
		value = UseValue;

		set();
	}

	private void set()
	{
		int Value = value.getValue();

		textfield.setText( Utility.formatMilliSeconds( Value + offset));

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
			value.setValue( Utility.parseMilliSeconds( textfield.getText()) - offset);
		}
		catch( Exception Reason)
		{
			// Ignore bad input and revert to old.
		}

		set();
	}

	@Override
	public void stateChanged( ChangeEvent e)
	{
		setFromSlider();
	}

	@Override
	public void actionPerformed( ActionEvent e)
	{
		setFromText();
	}

	@Override
	public void focusGained( FocusEvent e)
	{
	}

	@Override
	public void focusLost( FocusEvent e)
	{
		setFromText();
	}
}
