package net.stegemann.gui.panel.source;

import net.stegemann.configuration.Configuration;
import net.stegemann.configuration.Model;
import net.stegemann.configuration.source.Timer;
import net.stegemann.configuration.source.*;
import net.stegemann.configuration.source.input.*;
import net.stegemann.controller.SourceUtility;
import net.stegemann.gui.Constants;
import net.stegemann.gui.components.TextComponent;
import net.stegemann.gui.panel.source.input.*;

import javax.swing.*;
import java.io.Serial;

public class SourcePanel extends JPanel
{
	@Serial
	private static final long serialVersionUID = 6054122898469856987L;

	private final TextComponent name;
	private final JLabel type;
	private SpecificSourcePanel sourceTypePanel;

	private final GroupLayout layout;

	private final AnalogPanel analogPanel;
	private final ButtonPanel buttonPanel;
	private final FollowerPanel followerPanel;
	private final MapPanel mapPanel;
	private final MixPanel mixPanel;
	private final ProxyPanel proxyPanel;
	private final RotaryPanel rotaryPanel;
	private final StorePanel storePanel;
	private final SwitchPanel switchPanel;
	private final TickerPanel tickerPanel;
	private final TimerPanel timerPanel;
	private final TrimPanel trimPanel;

	private final SpecificSourcePanel specificSourcePanel;
	
	public SourcePanel( Configuration configuration)
	{
		analogPanel = new AnalogPanel( configuration);
		buttonPanel = new ButtonPanel( configuration);
		followerPanel = new FollowerPanel( configuration);
		mapPanel = new MapPanel( configuration);
		mixPanel = new MixPanel( configuration);
		proxyPanel = new ProxyPanel( configuration);
		rotaryPanel = new RotaryPanel( configuration);
		storePanel = new StorePanel( configuration);
		switchPanel = new SwitchPanel( configuration);
		tickerPanel = new TickerPanel( configuration);
		timerPanel = new TimerPanel( configuration);
		trimPanel = new TrimPanel( configuration);

		specificSourcePanel = new SpecificSourcePanel( configuration);
		
		setVisible( false);

		JLabel NameLabel = new JLabel( "Name:");
		name = new TextComponent( Constants.DEFAULT_TEXTFIELD_WIDTH);

		JLabel TypeLabel = new JLabel( "Typ: ");
		type = new JLabel();

		JSeparator Separator = new JSeparator();

		sourceTypePanel = new SpecificSourcePanel( configuration);

		// Layout elements.
		layout = new GroupLayout( this);
		setLayout( layout);

//		layout.setAutoCreateGaps( true);
		layout.setAutoCreateContainerGaps( true);

		layout.setHorizontalGroup( layout.createParallelGroup( GroupLayout.Alignment.LEADING)
			.addGroup( layout.createSequentialGroup()
				.addGroup( layout.createParallelGroup( GroupLayout.Alignment.TRAILING)
					.addComponent( TypeLabel)
					.addComponent( NameLabel)
				)
				.addGroup( layout.createParallelGroup( GroupLayout.Alignment.LEADING)
					.addComponent( type)
					.addComponent( name, javax.swing.GroupLayout.PREFERRED_SIZE,
										 javax.swing.GroupLayout.PREFERRED_SIZE,
										 javax.swing.GroupLayout.PREFERRED_SIZE)
				)
			)
			.addComponent( Separator)
			.addComponent( sourceTypePanel)
		);

		layout.setVerticalGroup( layout.createSequentialGroup()
			.addGroup( layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( TypeLabel)
				.addComponent( type)
			)
			.addGroup( layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( NameLabel)
				.addComponent( name, javax.swing.GroupLayout.PREFERRED_SIZE,
			   			 			 javax.swing.GroupLayout.PREFERRED_SIZE,
			   			 			 javax.swing.GroupLayout.PREFERRED_SIZE)
			)
			.addComponent( Separator)
			.addComponent( sourceTypePanel)
		);
	}

	public void set( Model model, Source source)
	{
		if( source == null)
		{
			clear();

			return;
		}

		name.attachValue( source.getName());
		type.setText( SourceUtility.getTypeNameForSource( source));

		sourceTypePanel.unset();

		SpecificSourcePanel NewSourceTypePanel = setPanel( model, source);

		// Replace existing panel.
		layout.replace( sourceTypePanel, NewSourceTypePanel);
		sourceTypePanel = NewSourceTypePanel;

		setVisible( true);
	}

	private SpecificSourcePanel setPanel( Model model, Source source)
	{
		if( source instanceof Analog)
		{
			analogPanel.set(( Analog) source);
			return analogPanel;
		}
		else if( source instanceof Button)
		{
			buttonPanel.set(( Button) source);
			return buttonPanel;
		}
		else if( source instanceof Rotary)
		{
			rotaryPanel.set(( Rotary) source);
			return rotaryPanel;
		}
		else if( source instanceof Switch)
		{
			switchPanel.set(( Switch) source);
			return switchPanel;
		}
		else if( source instanceof Ticker)
		{
			tickerPanel.set(( Ticker) source);
			return tickerPanel;
		}
		else if( source instanceof Map)
		{
			mapPanel.set( model, ( Map) source);
			return mapPanel;
		}
		else if( source instanceof Mix)
		{
			mixPanel.set( model, ( Mix) source);
			return mixPanel;
		}
		else if( source instanceof Store)
		{
			storePanel.set( model, ( Store) source);
			return storePanel;
		}
		else if( source instanceof Follower)
		{
			followerPanel.set( model, ( Follower) source);
			return followerPanel;
		}
		else if( source instanceof Timer)
		{
			timerPanel.set( model, ( Timer) source);
			return timerPanel;
		}
		else if( source instanceof Trimmer)
		{
			trimPanel.set( model, (Trimmer) source);
			return trimPanel;
		}
		else if( source instanceof Proxy)
		{
			proxyPanel.set( model, ( Proxy) source);
			return proxyPanel;
		}
		else
		{
			return specificSourcePanel;
		}
	}

	private void clear()
	{
		setVisible( false);

		if( sourceTypePanel != null)
		{
			sourceTypePanel.unset();
		}

		name.attachValue( null);
	}
}
