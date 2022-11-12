package net.stegemann.gui.panel;

import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.GroupLayout;
import javax.swing.JButton;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.ListSelectionModel;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;

import net.stegemann.configuration.Configuration;
import net.stegemann.configuration.Model;
import net.stegemann.configuration.source.Proxy;
import net.stegemann.configuration.source.Source;
import net.stegemann.configuration.source.Sources;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.view.SourcesView;
import net.stegemann.configuration.view.SourcesView.HasEmpty;
import net.stegemann.configuration.view.SourcesView.HasFixed;
import net.stegemann.configuration.view.SourcesView.HasProxies;
import net.stegemann.configuration.view.SourcesView.PickGlobals;
import net.stegemann.controller.Controller;
import net.stegemann.gui.model.ListCellRenderer;
import net.stegemann.gui.model.SourcesComboBoxModel;
import net.stegemann.gui.panel.source.SourcePanel;

public class ProxiesPanel extends JPanel implements ActionListener, ListSelectionListener
{
	private static final long serialVersionUID = 2832719260884140511L;

	private final Controller controller;
	private final Configuration configuration;
	private SourcesView proxiesView;
	private Model model;

	private Number typeId;

	private final JList< Source> proxiesList;
	private final JButton addButton;
	private final JButton removeButton;

	private final SourcePanel proxyPanel;

	public ProxiesPanel( Controller controller)
	{
		this.controller = controller;
		configuration = controller.getConfiguration();

		proxiesList = new JList<>();
		proxiesList.setSelectionMode( ListSelectionModel.SINGLE_SELECTION);
		proxiesList.setLayoutOrientation( JList.VERTICAL);
		proxiesList.addListSelectionListener( this);
		proxiesList.setCellRenderer( new ListCellRenderer<Source>());

		JScrollPane ProxiesScrollPane = new JScrollPane( proxiesList);
		ProxiesScrollPane.setMinimumSize( new Dimension( 150, 150));
		ProxiesScrollPane.setVerticalScrollBarPolicy( JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);

		addButton = new JButton( "+");
		addButton.addActionListener( this);
		removeButton = new JButton( "-");
		removeButton.addActionListener( this);

		proxyPanel = new SourcePanel( configuration);

		// Layout elements.
		GroupLayout Layout = new GroupLayout( this);
		setLayout( Layout);

		Layout.setHonorsVisibility( false);
//		Layout.setAutoCreateGaps( true);
		Layout.setAutoCreateContainerGaps( true);

		Layout.setHorizontalGroup( Layout.createSequentialGroup()
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( ProxiesScrollPane, javax.swing.GroupLayout.PREFERRED_SIZE, 150, Integer.MAX_VALUE)
				.addGroup( Layout.createSequentialGroup()
					.addComponent( addButton)
					.addComponent( removeButton)
				)
			)
			.addComponent( proxyPanel)
		);

		Layout.setVerticalGroup( Layout.createParallelGroup( GroupLayout.Alignment.LEADING)
			.addGroup( Layout.createSequentialGroup()
				.addComponent( ProxiesScrollPane)
				.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.LEADING)
					.addComponent( addButton)
					.addComponent( removeButton)
				)
			)
			.addComponent( proxyPanel)
		);
	}

	public void set( Model model)
	{
		this.model = model;

		Sources UseSources = configuration.getSources();

		typeId = model.getTypeId();
		proxiesView =
			new SourcesView( UseSources, PickGlobals.No, typeId, null, Proxy.class, HasEmpty.No,
							 HasFixed.No, HasProxies.Yes);

		proxiesList.setModel( new SourcesComboBoxModel( proxiesView));
		proxyPanel.set( model, proxiesView.getSourceFromIndex( proxiesList.getSelectedIndex()));
	}

	@Override
	public void valueChanged( ListSelectionEvent e)
	{
		if( e.getValueIsAdjusting() == false)
		{
			proxyPanel.set( model, proxiesView.getSourceFromIndex( proxiesList.getSelectedIndex()));
		}
	}

	@Override
	public void actionPerformed( ActionEvent e)
	{
		int SelectedSourceIndex = proxiesList.getSelectedIndex();

		if( e.getSource() == addButton)
		{
			Proxy NewProxy = controller.addProxy( typeId);

			if( NewProxy == null)
			{
				return;
			}

			// Move selection to new proxy.
			proxiesView.rescan();
			proxiesList.setSelectedIndex( proxiesView.getSourceIndexFromId( NewProxy.getId()));
		}
		else if( e.getSource() == removeButton)
		{
			controller.removeProxy( proxiesView.getFullSourceIndex( SelectedSourceIndex));

			if( SelectedSourceIndex > 0)
			{
				proxiesList.setSelectedIndex( SelectedSourceIndex - 1);
			}
		}

		if( SelectedSourceIndex == proxiesList.getSelectedIndex())
		{
			// In this case, valueChanged wasn't triggered, so we set the panel here.
			proxiesView.rescan();
			proxyPanel.set( model, proxiesView.getSourceFromIndex( SelectedSourceIndex));
		}
	}
}
