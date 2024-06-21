package net.stegemann.gui.panel;

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
import net.stegemann.controller.SourceUtility;
import net.stegemann.gui.misc.hermesPanel;import net.stegemann.gui.model.ListCellRenderer;
import net.stegemann.gui.model.SourcesComboBoxModel;
import net.stegemann.gui.panel.source.SourcePanel;

import javax.swing.*;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.io.Serial;

public class SourcesPanel extends hermesPanel
                       implements ListSelectionListener
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
    private final JButton addButton, cloneButton, removeButton;
    private final JButton upButton, downButton;

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
        sourcesList.setCellRenderer( new ListCellRenderer< Source>());

        JScrollPane sourcesScrollPane = new JScrollPane( sourcesList);
        sourcesScrollPane.setMinimumSize( new Dimension( 150, 150));
        sourcesScrollPane.setVerticalScrollBarPolicy( JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);

        addButton = button("+");
        upButton = button("<");
        cloneButton = button( "Kopieren");
        downButton = button( ">");
        removeButton = button( "-");

        sourcePanel = new SourcePanel( configuration);

        // Layout elements.
        GroupLayout layout = new GroupLayout( this);
        setLayout( layout);

        layout.setHonorsVisibility( false);
//		Layout.setAutoCreateGaps( true);
        layout.setAutoCreateContainerGaps( true);

        layout.setHorizontalGroup
        (
            layout.createSequentialGroup()
            .addGroup
            (
                layout.createParallelGroup( GroupLayout.Alignment.CENTER)
                .addComponent
                (
                    sourcesScrollPane,
                    javax.swing.GroupLayout.PREFERRED_SIZE,
                    150,
                    Integer.MAX_VALUE
                )
                .addGroup
                (
                    layout.createSequentialGroup()
                    .addComponent( addButton)
                    .addComponent( upButton)
                    .addComponent( cloneButton)
                    .addComponent( downButton)
                    .addComponent( removeButton)
                )
            )
            .addComponent( sourcePanel)
        );

        layout.setVerticalGroup
        (
            layout.createParallelGroup( GroupLayout.Alignment.LEADING)
            .addGroup
            (
                layout.createSequentialGroup()
                .addComponent( sourcesScrollPane)
                .addGroup
                (
                    layout.createParallelGroup( GroupLayout.Alignment.LEADING)
                    .addComponent( addButton)
                    .addComponent( upButton)
                    .addComponent( cloneButton)
                    .addComponent( downButton)
                    .addComponent( removeButton)
                )
            )
            .addComponent( sourcePanel)
        );
    }

    @Override
    public void valueChanged( ListSelectionEvent event)
    {
        if( event.getValueIsAdjusting() == false)
        {
            sourcePanel.set( model, sourcesView.getSourceFromIndex( sourcesList.getSelectedIndex()));
        }
    }

    @Override
    public void actionPerformed( ActionEvent event)
    {
        int selectedSourceIndex = sourcesList.getSelectedIndex();

        if( event.getSource() == addButton)
        {
            addEvent();
        }
        else if( event.getSource() == upButton)
        {
            upEvent( selectedSourceIndex);
        }
        else if( event.getSource() == cloneButton)
        {
            cloneEvent( selectedSourceIndex);
        }
        else if( event.getSource() == downButton)
        {
            downEvent( selectedSourceIndex);
        }
        else if( event.getSource() == removeButton)
        {
            removeEvent( selectedSourceIndex);
        }

        if( selectedSourceIndex == sourcesList.getSelectedIndex())
        {
            // In this case, valueChanged wasn't triggered, so we set the panel here.
            sourcesView.rescan();
            sourcePanel.set( model, sourcesView.getSourceFromIndex( selectedSourceIndex));
        }
    }

    private void addEvent()
    {
        Object[] options = SourceUtility.getSelectableTypeNames();

        String typeName = ( String) JOptionPane.showInputDialog
        (
            this, null, "Neuen Quellentyp auswählen...",
            JOptionPane.PLAIN_MESSAGE, null, options, options[ 0]
        );

        Source newSource = controller.addSource( SourceUtility.createSourceForTypeName( typeName), modelId);

        if( newSource == null)
        {
            return;
        }

        // Select new source.
        sourcesView.rescan();
        sourcesList.setSelectedIndex( sourcesView.getSourceIndexFromId( newSource.getId()));
    }

    private void upEvent( int selectedSourceIndex)
    {
        if( selectedSourceIndex == 0)
        {
            return;
        }

        switchSources( selectedSourceIndex, selectedSourceIndex - 1);
    }

    private void cloneEvent( int selectedSourceIndex)
    {
        Source newSource = controller.duplicateSource( sourcesView.getFullSourceIndex( selectedSourceIndex));

        if( newSource == null)
        {
            return;
        }

        // Select new source.
        sourcesView.rescan();
        sourcesList.setSelectedIndex( sourcesView.getSourceIndexFromId( newSource.getId()));
    }

    private void downEvent( int selectedSourceIndex)
    {
        if( selectedSourceIndex == sourcesView.getCount() - 1)
        {
            return;
        }

        switchSources( selectedSourceIndex, selectedSourceIndex + 1);
    }

    private void removeEvent( int selectedSourceIndex)
    {
        controller.removeSource( sourcesView.getFullSourceIndex( selectedSourceIndex));

        if( selectedSourceIndex > 0)
        {
            sourcesList.setSelectedIndex( selectedSourceIndex - 1);
        }
    }

    private void switchSources( int indexOne, int indexTwo)
    {
        Source sourceOne = sourcesView.getSourceFromIndex( indexOne);
        Source sourceTwo = sourcesView.getSourceFromIndex( indexTwo);

        controller.switchSources( sourceOne, sourceTwo);

        // Select new source.
        sourcesView.rescan();
        sourcesList.setSelectedIndex( sourcesView.getSourceIndexFromId( sourceOne.getId()));
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
