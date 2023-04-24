package net.stegemann.gui.panel;

import net.stegemann.configuration.Configuration;
import net.stegemann.configuration.Model;
import net.stegemann.configuration.source.Source;
import net.stegemann.configuration.source.Sources;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.ValueOutOfRangeException;
import net.stegemann.configuration.view.SourcesView;
import net.stegemann.configuration.view.SourcesView.HasEmpty;
import net.stegemann.configuration.view.SourcesView.HasFixed;
import net.stegemann.configuration.view.SourcesView.HasProxies;
import net.stegemann.configuration.view.SourcesView.PickGlobals;
import net.stegemann.controller.Controller;
import net.stegemann.controller.SourceUtility;
import net.stegemann.gui.model.ListCellRenderer;
import net.stegemann.gui.model.SourcesComboBoxModel;
import net.stegemann.gui.panel.source.SourcePanel;

import javax.swing.*;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.Serial;

public class SourcesPanel extends JPanel implements ActionListener, ListSelectionListener
{
	@Serial
	private static final long serialVersionUID = 2773055623640369468L;

	public enum PanelType
	{
		GLOBAL,
		TYPE,
		MODEL
	}

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

	public SourcesPanel( PanelType PanelType, Controller Controller)
	{
		this.panelType = PanelType;
		this.controller = Controller;
		this.configuration = Controller.getConfiguration();

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

		Layout.setHorizontalGroup
		(
			Layout.createSequentialGroup()
				.addGroup
				(
					Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
						.addComponent
						(
							SourcesScrollPane,
							javax.swing.GroupLayout.PREFERRED_SIZE,
							150,
							Integer.MAX_VALUE
						)
						.addGroup
						(
							Layout.createSequentialGroup()
								.addComponent( addButton)
								.addComponent( cloneButton)
								.addComponent( removeButton)
						)
				)
				.addComponent( sourcePanel)
		);

		Layout.setVerticalGroup
		(
			Layout.createParallelGroup( GroupLayout.Alignment.LEADING)
				.addGroup
				(
					Layout.createSequentialGroup()
						.addComponent( SourcesScrollPane)
						.addGroup
						(
							Layout.createParallelGroup( GroupLayout.Alignment.LEADING)
								.addComponent( addButton)
								.addComponent( cloneButton)
								.addComponent( removeButton)
						)
				)
				.addComponent( sourcePanel)
		);
	}

	@Override
	public void valueChanged( ListSelectionEvent Event)
	{
		if( Event.getValueIsAdjusting() == false)
		{
			sourcePanel.set( model, sourcesView.getSourceFromIndex( sourcesList.getSelectedIndex()));
		}
	}

	@Override
	public void actionPerformed( ActionEvent Event)
	{
		int SelectedSourceIndex = sourcesList.getSelectedIndex();

		if( Event.getSource() == addButton)
		{
			addEvent();
		}
		else if( Event.getSource() == cloneButton)
		{
			cloneEvent( SelectedSourceIndex);
		}
		else if( Event.getSource() == removeButton)
		{
			removeEvent( SelectedSourceIndex);
		}

		if( SelectedSourceIndex == sourcesList.getSelectedIndex())
		{
			// In this case, valueChanged wasn't triggered, so we set the panel here.
			sourcesView.rescan();
			sourcePanel.set( model, sourcesView.getSourceFromIndex( SelectedSourceIndex));
		}
	}

	private void addEvent()
	{
		Object[] Options = SourceUtility.getSelectableTypeNames();

		String TypeName = ( String) JOptionPane.showInputDialog(
			this, null, "Neuen Mischer Typ auswählen...",
			JOptionPane.PLAIN_MESSAGE, null, Options, Options[ 0]);

		Source NewSource;

		try
		{
			NewSource = controller.addSource( SourceUtility.createSourceForTypeName( TypeName), modelId);
		}
		catch( ValueOutOfRangeException Reason)
		{
			throw new RuntimeException( Reason);
		}

		if( NewSource == null)
		{
			return;
		}

		// Select new source.
		sourcesView.rescan();
		sourcesList.setSelectedIndex( sourcesView.getSourceIndexFromId( NewSource.getId()));
	}

	private void cloneEvent( int SelectedSourceIndex)
	{
		Source NewSource = controller.cloneSource( sourcesView.getFullSourceIndex( SelectedSourceIndex));

		if( NewSource == null)
		{
			return;
		}

		// Select new source.
		sourcesView.rescan();
		sourcesList.setSelectedIndex( sourcesView.getSourceIndexFromId( NewSource.getId()));
	}

	private void removeEvent( int SelectedSourceIndex)
	{
		controller.removeSource( sourcesView.getFullSourceIndex( SelectedSourceIndex));

		if( SelectedSourceIndex > 0)
		{
			sourcesList.setSelectedIndex( SelectedSourceIndex - 1);
		}
	}

	public void set( Model model)
	{
		this.model = model;

		Sources sources = configuration.getSources();

		switch( panelType)
		{
			case GLOBAL ->
			{
				modelId = Model.Global;
				sourcesView = sourcesView( sources, PickGlobals.Yes, null, null);
			}

			case TYPE ->
			{
				modelId = model.getTypeId();
				sourcesView = sourcesView( sources, PickGlobals.No, modelId, null);
			}

			case MODEL ->
			{
				modelId = model.getId();
				sourcesView = sourcesView( sources, PickGlobals.No, null, modelId);
			}
		}

		sourcesList.setModel( new SourcesComboBoxModel( sourcesView));
		sourcePanel.set( model, sourcesView.getSourceFromIndex( sourcesList.getSelectedIndex()));
	}

	private SourcesView sourcesView( Sources sources, PickGlobals pickGlobals, Number typeId, Number modelId)
	{
		return new SourcesView( sources, pickGlobals, typeId, modelId, HasEmpty.No, HasFixed.No, HasProxies.No);
	}
}
