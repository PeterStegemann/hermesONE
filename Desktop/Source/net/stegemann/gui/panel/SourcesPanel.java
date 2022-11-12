package net.stegemann.gui.panel;

import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.GroupLayout;
import javax.swing.JButton;
import javax.swing.JList;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.ListSelectionModel;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;

import net.stegemann.configuration.Configuration;
import net.stegemann.configuration.Model;
import net.stegemann.configuration.source.Source;
import net.stegemann.configuration.source.Sources;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.view.SourcesView;
import net.stegemann.configuration.view.SourcesView.HasEmpty;
import net.stegemann.configuration.view.SourcesView.HasFixed;
import net.stegemann.configuration.view.SourcesView.HasProxies;
import net.stegemann.configuration.view.SourcesView.PickGlobals;
import net.stegemann.controller.Controller;
import net.stegemann.gui.misc.SourceUtility;
import net.stegemann.gui.model.ListCellRenderer;
import net.stegemann.gui.model.SourcesComboBoxModel;
import net.stegemann.gui.panel.source.SourcePanel;

public class SourcesPanel extends JPanel implements ActionListener, ListSelectionListener
{
	public enum PanelType
	{
		GLOBAL,
		TYPE,
		MODEL
	}

	private static final long serialVersionUID = 2773055623640369468L;

	private final Controller controller;
	private final Configuration configuration;
	private SourcesView sourcesView;
	private Model model;

	private final PanelType panelType;

	private Number modelId;

	private final JList< Source> sourcesList;
	private final JButton addButton;
	private final JButton cloneButton;
	private final JButton removeButton;

	private final SourcePanel sourcePanel;

	public SourcesPanel( PanelType panelType, Controller controller)
	{
		this.panelType = panelType;
		this.controller = controller;
		this.configuration = controller.getConfiguration();

		sourcesList = new JList<>();
		sourcesList.setSelectionMode( ListSelectionModel.SINGLE_SELECTION);
		sourcesList.setLayoutOrientation( JList.VERTICAL);
		sourcesList.addListSelectionListener( this);
		sourcesList.setCellRenderer( new ListCellRenderer<Source>());

		JScrollPane SourcesScrollPane = new JScrollPane( sourcesList);
		SourcesScrollPane.setMinimumSize( new Dimension( 150, 150));
		SourcesScrollPane.setVerticalScrollBarPolicy( JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);

		addButton = new JButton( "+");
		addButton.addActionListener( this);
		cloneButton = new JButton( "Kopieren");
		cloneButton.addActionListener( this);
		removeButton = new JButton( "-");
		removeButton.addActionListener( this);

		sourcePanel = new SourcePanel( configuration);

		// Layout elements.
		GroupLayout Layout = new GroupLayout( this);
		setLayout( Layout);

		Layout.setHonorsVisibility( false);
//		Layout.setAutoCreateGaps( true);
		Layout.setAutoCreateContainerGaps( true);

		Layout.setHorizontalGroup( Layout.createSequentialGroup()
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( SourcesScrollPane, javax.swing.GroupLayout.PREFERRED_SIZE, 150,
							   Integer.MAX_VALUE)
				.addGroup( Layout.createSequentialGroup()
					.addComponent( addButton)
					.addComponent( cloneButton)
					.addComponent( removeButton)
				)
			)
			.addComponent( sourcePanel)
		);

		Layout.setVerticalGroup( Layout.createParallelGroup( GroupLayout.Alignment.LEADING)
			.addGroup( Layout.createSequentialGroup()
				.addComponent( SourcesScrollPane)
				.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.LEADING)
					.addComponent( addButton)
					.addComponent( cloneButton)
					.addComponent( removeButton)
				)
			)
			.addComponent( sourcePanel)
		);
	}

	@Override
	public void valueChanged( ListSelectionEvent e)
	{
		if( e.getValueIsAdjusting() == false)
		{
			sourcePanel.set( model, sourcesView.getSourceFromIndex( sourcesList.getSelectedIndex()));
		}
	}

	@Override
	public void actionPerformed( ActionEvent e)
	{
		int SelectedSourceIndex = sourcesList.getSelectedIndex();

		if( e.getSource() == addButton)
		{
			Object[] Options = SourceUtility.getSelectableTypeNames();

			String TypeName =
				( String) JOptionPane.showInputDialog(
					this, null, "Neuen Mischer Typ auswaehlen...", JOptionPane.PLAIN_MESSAGE,
					null, Options, Options[ 0]);

			Source NewSource =
				controller.addSource( SourceUtility.createSourceForTypeName( TypeName), modelId);

			if( NewSource == null)
			{
				return;
			}

			// Select new source.
			sourcesView.rescan();
			sourcesList.setSelectedIndex( sourcesView.getSourceIndexFromId( NewSource.getId()));
		}
		else if( e.getSource() == cloneButton)
		{
			Source NewSource =
				controller.cloneSource( sourcesView.getFullSourceIndex( SelectedSourceIndex));

			if( NewSource == null)
			{
				return;
			}

			// Select new source.
			sourcesView.rescan();
			sourcesList.setSelectedIndex( sourcesView.getSourceIndexFromId( NewSource.getId()));
		}
		else if( e.getSource() == removeButton)
		{
			controller.removeSource( sourcesView.getFullSourceIndex( SelectedSourceIndex));

			if( SelectedSourceIndex > 0)
			{
				sourcesList.setSelectedIndex( SelectedSourceIndex - 1);
			}
		}

		if( SelectedSourceIndex == sourcesList.getSelectedIndex())
		{
			// In this case, valueChanged wasn't triggered, so we set the panel here.
			sourcesView.rescan();
			sourcePanel.set( model, sourcesView.getSourceFromIndex( SelectedSourceIndex));
		}
	}

	public void set( Model model)
	{
		this.model = model;

		Sources UseSources = configuration.getSources();

		switch( panelType)
		{
			case GLOBAL :
			{
				modelId = Model.Global;
				sourcesView = new SourcesView( UseSources, PickGlobals.Yes, null, null, HasEmpty.No,
														 HasFixed.No, HasProxies.No);
			}
			break;

			case TYPE :
			{
				modelId = model.getTypeId();
				sourcesView = new SourcesView( UseSources, PickGlobals.No, modelId, null, HasEmpty.No,
														 HasFixed.No, HasProxies.No);
			}
			break;

			case MODEL :
			{
				modelId = model.getId();
				sourcesView = new SourcesView( UseSources, PickGlobals.No, null, modelId, HasEmpty.No,
														 HasFixed.No, HasProxies.No);
			}
			break;
		}

		sourcesList.setModel( new SourcesComboBoxModel( sourcesView));
		sourcePanel.set( model, sourcesView.getSourceFromIndex( sourcesList.getSelectedIndex()));
	}
}
