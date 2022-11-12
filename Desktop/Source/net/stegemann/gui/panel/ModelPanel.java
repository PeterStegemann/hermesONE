package net.stegemann.gui.panel;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.GroupLayout;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTabbedPane;

import net.stegemann.configuration.Configuration;
import net.stegemann.configuration.Model;
import net.stegemann.controller.Controller;
import net.stegemann.gui.Constants;
import net.stegemann.gui.components.RFModeComponent;
import net.stegemann.gui.components.TextComponent;
import net.stegemann.gui.components.TypeIdComponent;
import net.stegemann.gui.frame.StatusSourcesFrame;
import net.stegemann.gui.panel.SourcesPanel.PanelType;

public class ModelPanel extends JPanel implements ActionListener
{
	private static final long serialVersionUID = -4564059407785726681L;

	private final Configuration configuration;

	private final TextComponent name;
	private final TypeIdComponent typeId;
	private final RFModeComponent rfMode;
	private final JButton statusSourcesButton;

	private final ProxiesPanel proxiesPanel;
	private final SourcesPanel globalSourcesPanel;
	private final SourcesPanel typeSourcesPanel;
	private final SourcesPanel modelSourcesPanel;
	private final ChannelsPanel channelsPanel;

	private final StatusSourcesFrame statusSources;

	public ModelPanel( Controller UseController)
	{
		configuration = UseController.getConfiguration();

		statusSources = new StatusSourcesFrame( configuration);

		setVisible( false);

		JLabel NameLabel = new JLabel( "Name:");
		name = new TextComponent( Constants.DEFAULT_TEXTFIELD_WIDTH);

		JLabel TypeLabel = new JLabel( "Typ:");
		typeId = new TypeIdComponent();

		JLabel RFModeLabel = new JLabel( "HF Modul:");
		rfMode = new RFModeComponent();

		statusSourcesButton = new JButton( "Status...");
		statusSourcesButton.addActionListener( this);

		globalSourcesPanel = new SourcesPanel( PanelType.GLOBAL, UseController);
		typeSourcesPanel = new SourcesPanel( PanelType.TYPE, UseController);
		modelSourcesPanel = new SourcesPanel( PanelType.MODEL, UseController);
		proxiesPanel = new ProxiesPanel( UseController);
		channelsPanel = new ChannelsPanel( configuration);

		JTabbedPane SourcesPane = new JTabbedPane();
		SourcesPane.add( modelSourcesPanel, "Modell");
		SourcesPane.add( typeSourcesPanel, "Typ");
		SourcesPane.add( globalSourcesPanel, "Global");
		SourcesPane.add( proxiesPanel, "Proxies");
		SourcesPane.add( channelsPanel, "Kanäle");

		// Layout elements.
		GroupLayout Layout = new GroupLayout( this);
		setLayout( Layout);

//		Layout.setAutoCreateGaps( true);
		Layout.setAutoCreateContainerGaps( true);

		Layout.setHorizontalGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
			.addGroup( Layout.createSequentialGroup()
				.addComponent( NameLabel)
				.addComponent( name)
				.addComponent( TypeLabel)
				.addComponent( typeId)
				.addComponent( RFModeLabel)
				.addComponent( rfMode)
				.addComponent( statusSourcesButton)
			)
			.addComponent( SourcesPane)
		);

		Layout.setVerticalGroup( Layout.createSequentialGroup()
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( NameLabel, javax.swing.GroupLayout.PREFERRED_SIZE,
										  javax.swing.GroupLayout.PREFERRED_SIZE,
										  javax.swing.GroupLayout.PREFERRED_SIZE)
				.addComponent( name, javax.swing.GroupLayout.PREFERRED_SIZE,
						   			 javax.swing.GroupLayout.PREFERRED_SIZE,
						   			 javax.swing.GroupLayout.PREFERRED_SIZE)
				.addComponent( TypeLabel, javax.swing.GroupLayout.PREFERRED_SIZE,
										  javax.swing.GroupLayout.PREFERRED_SIZE,
										  javax.swing.GroupLayout.PREFERRED_SIZE)
				.addComponent( typeId, javax.swing.GroupLayout.PREFERRED_SIZE,
									   javax.swing.GroupLayout.PREFERRED_SIZE,
									   javax.swing.GroupLayout.PREFERRED_SIZE)
				.addComponent( RFModeLabel, javax.swing.GroupLayout.PREFERRED_SIZE,
									   	    javax.swing.GroupLayout.PREFERRED_SIZE,
									   	    javax.swing.GroupLayout.PREFERRED_SIZE)
				.addComponent( rfMode, javax.swing.GroupLayout.PREFERRED_SIZE,
								       javax.swing.GroupLayout.PREFERRED_SIZE,
									   javax.swing.GroupLayout.PREFERRED_SIZE)
				.addComponent( statusSourcesButton, javax.swing.GroupLayout.PREFERRED_SIZE,
													javax.swing.GroupLayout.PREFERRED_SIZE,
													javax.swing.GroupLayout.PREFERRED_SIZE)
			)
			.addComponent( SourcesPane)
		);
	}

	public void Set( Model UseModel)
	{
		statusSources.Set( UseModel);

		if( UseModel == null)
		{
			setVisible( false);

			return;
		}

		name.attachValue( UseModel.getName());

		typeId.SetTypes( configuration.getTypes());
		typeId.attachValue( UseModel.getTypeId());

		rfMode.attachValue( UseModel.getRFMode());

		globalSourcesPanel.set( UseModel);
		typeSourcesPanel.set( UseModel);
		modelSourcesPanel.set( UseModel);
		proxiesPanel.set( UseModel);
		channelsPanel.set( UseModel);

		setVisible( true);
	}

	@Override
	public void actionPerformed( ActionEvent e)
	{
		if( e.getSource() == statusSourcesButton)
		{
			statusSources.setLocationRelativeTo( this);
			statusSources.open();
		}
	}
}
