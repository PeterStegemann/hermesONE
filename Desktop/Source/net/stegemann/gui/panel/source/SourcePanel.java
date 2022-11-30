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

public class SourcePanel extends JPanel
{
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

	public void set( Model UseModel, Source UseSource)
	{
		if( UseSource == null)
		{
			clear();

			return;
		}

		name.attachValue( UseSource.getName());
		type.setText( SourceUtility.getTypeNameForSource( UseSource));

		sourceTypePanel.unset();

		SpecificSourcePanel NewSourceTypePanel = setPanel( UseModel, UseSource);

		// Replace existing panel.
		layout.replace( sourceTypePanel, NewSourceTypePanel);
		sourceTypePanel = NewSourceTypePanel;

		setVisible( true);
	}

	private SpecificSourcePanel setPanel( Model UseModel, Source UseSource)
	{
		if( UseSource instanceof Analog)
		{
			analogPanel.set(( Analog) UseSource);
			return analogPanel;
		}
		else if( UseSource instanceof Button)
		{
			buttonPanel.set(( Button) UseSource);
			return buttonPanel;
		}
		else if( UseSource instanceof Rotary)
		{
			rotaryPanel.set(( Rotary) UseSource);
			return rotaryPanel;
		}
		else if( UseSource instanceof Switch)
		{
			switchPanel.set(( Switch) UseSource);
			return switchPanel;
		}
		else if( UseSource instanceof Ticker)
		{
			tickerPanel.set(( Ticker) UseSource);
			return tickerPanel;
		}
		else if( UseSource instanceof Map)
		{
			mapPanel.set( UseModel, ( Map) UseSource);
			return mapPanel;
		}
		else if( UseSource instanceof Mix)
		{
			mixPanel.set( UseModel, ( Mix) UseSource);
			return mixPanel;
		}
		else if( UseSource instanceof Store)
		{
			storePanel.set( UseModel, ( Store) UseSource);
			return storePanel;
		}
		else if( UseSource instanceof Follower)
		{
			followerPanel.set( UseModel, ( Follower) UseSource);
			return followerPanel;
		}
		else if( UseSource instanceof Timer)
		{
			timerPanel.set( UseModel, ( Timer) UseSource);
			return timerPanel;
		}
		else if( UseSource instanceof Trimmer)
		{
			trimPanel.set( UseModel, (Trimmer) UseSource);
			return trimPanel;
		}
		else if( UseSource instanceof Proxy)
		{
			proxyPanel.set( UseModel, ( Proxy) UseSource);
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

		return;
	}
}
