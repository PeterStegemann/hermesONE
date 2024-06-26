package net.stegemann.gui.panel;

import javax.swing.*;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.io.Serial;
import net.stegemann.configuration.*;
import net.stegemann.configuration.type.ModelId;import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.view.ModelsView;
import net.stegemann.controller.Controller;
import net.stegemann.gui.Constants;
import net.stegemann.gui.components.TextComponent;
import net.stegemann.gui.frame.SystemFrame;
import net.stegemann.gui.misc.hermesPanel;
import net.stegemann.gui.model.ListCellRenderer;
import net.stegemann.gui.model.ModelsViewComboBoxModel;
import net.stegemann.gui.model.TypesComboBoxModel;

public class MainPanel extends hermesPanel
                    implements ListSelectionListener
{
    @Serial
    private static final long serialVersionUID = -4538240657987172398L;

    private final Controller controller;
    private final Configuration configuration;

    private ModelsView modelsView;

    private final JList< Type> typesList;
    private final TextComponent typeName;
    private final JButton createTypeButton, duplicateTypeButton, removeTypeButton;
    private final JButton upTypeButton, downTypeButton;

    private final JList< Model> modelsList;
    private final JButton createModelButton, duplicateModelButton, removeModelButton;
    private final JButton upModelButton, downModelButton;

    private final ModelPanel modelPanel;

    private final JButton systemButton;

    public MainPanel( Controller controller)
    {
        this.controller = controller;
        configuration = controller.getConfiguration();

        systemButton = button( "System...");

        JSeparator Separator = new JSeparator();

        JLabel TypesListLabel = new JLabel( "Typen:");

        typesList = new JList<>();
        typesList.setSelectionMode( ListSelectionModel.SINGLE_SELECTION);
        typesList.setLayoutOrientation( JList.VERTICAL);
        typesList.addListSelectionListener( this);
        typesList.setCellRenderer( new ListCellRenderer< Type>());

        JScrollPane TypesScrollPane = new JScrollPane( typesList);
        TypesScrollPane.setMinimumSize( new Dimension( 150, 150));
        TypesScrollPane.setVerticalScrollBarPolicy( JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);

        typeName = new TextComponent( Constants.DEFAULT_TEXTFIELD_WIDTH);

        createTypeButton = createButton();
        upTypeButton = upButton();
        duplicateTypeButton = duplicateButton();
        downTypeButton = downButton();
        removeTypeButton = removeButton();

        JLabel ModelsListLabel = new JLabel( "Modelle:");

        modelsList = new JList<>();
        modelsList.setSelectionMode( ListSelectionModel.SINGLE_SELECTION);
        modelsList.setLayoutOrientation( JList.VERTICAL);
        modelsList.addListSelectionListener( this);
        modelsList.setCellRenderer( new ListCellRenderer< Model>());

        JScrollPane ModelsScrollPane = new JScrollPane( modelsList);
        ModelsScrollPane.setMinimumSize( new Dimension( 150, 150));
        ModelsScrollPane.setVerticalScrollBarPolicy( JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);

        createModelButton = createButton();
        upModelButton = upButton();
        duplicateModelButton = duplicateButton();
        downModelButton = downButton();
        removeModelButton = removeButton();

        modelPanel = new ModelPanel( controller);

        // Layout elements.
        GroupLayout layout = new GroupLayout( this);
        setLayout( layout);

        layout.setHonorsVisibility( false);
//		Layout.setAutoCreateGaps( true);
        layout.setAutoCreateContainerGaps( true);

        layout.setHorizontalGroup
        (
            layout.createParallelGroup( GroupLayout.Alignment.LEADING)
            .addComponent( systemButton)
            .addComponent( Separator)
            .addGroup
            (
                layout.createSequentialGroup()
                .addGroup
                (
                    layout.createParallelGroup( GroupLayout.Alignment.CENTER)
                    .addGroup
                    (
                        layout.createParallelGroup( GroupLayout.Alignment.CENTER)
                        .addComponent( TypesListLabel)
                        .addComponent
                        (
                            TypesScrollPane, javax.swing.GroupLayout.PREFERRED_SIZE,150, Integer.MAX_VALUE
                        )
                        .addComponent( typeName)
                        .addGroup
                        (
                            layout.createSequentialGroup()
                            .addComponent( createTypeButton)
                            .addComponent( upTypeButton)
                            .addComponent( duplicateTypeButton)
                            .addComponent( downTypeButton)
                            .addComponent( removeTypeButton)
                        )
                    )
                    .addGroup
                    (
                        layout.createParallelGroup( GroupLayout.Alignment.CENTER)
                        .addComponent( ModelsListLabel)
                        .addComponent
                        (
                            ModelsScrollPane, javax.swing.GroupLayout.PREFERRED_SIZE, 150, Integer.MAX_VALUE
                        )
                        .addGroup
                        (
                            layout.createSequentialGroup()
                            .addComponent( createModelButton)
                            .addComponent( upModelButton)
                            .addComponent( duplicateModelButton)
                            .addComponent( downModelButton)
                            .addComponent( removeModelButton)
                        )
                    )
                )
                .addComponent( modelPanel)
            )
        );

        layout.setVerticalGroup
        (
            layout.createSequentialGroup()
            .addComponent( systemButton)
            .addComponent( Separator)
            .addGroup
            (
                layout.createParallelGroup( GroupLayout.Alignment.LEADING)
                .addGroup
                (
                    layout.createSequentialGroup()
                    .addGroup
                    (
                        layout.createSequentialGroup()
                        .addComponent( TypesListLabel)
                        .addComponent
                        (
                            TypesScrollPane, javax.swing.GroupLayout.PREFERRED_SIZE,
                                             javax.swing.GroupLayout.PREFERRED_SIZE,
                                             javax.swing.GroupLayout.PREFERRED_SIZE
                        )
                        .addComponent
                        (
                            typeName, javax.swing.GroupLayout.PREFERRED_SIZE,
                                      javax.swing.GroupLayout.PREFERRED_SIZE,
                                      javax.swing.GroupLayout.PREFERRED_SIZE
                        )
                        .addGroup
                        (
                            layout.createParallelGroup( GroupLayout.Alignment.LEADING)
                            .addComponent( createTypeButton)
                            .addComponent( upTypeButton)
                            .addComponent( duplicateTypeButton)
                            .addComponent( downTypeButton)
                            .addComponent( removeTypeButton)
                        )
                    )
                    .addGroup
                    (
                        layout.createSequentialGroup()
                        .addComponent( ModelsListLabel)
                        .addComponent( ModelsScrollPane)
                        .addGroup
                        (
                            layout.createParallelGroup( GroupLayout.Alignment.LEADING)
                            .addComponent( createModelButton)
                            .addComponent( upModelButton)
                            .addComponent( duplicateModelButton)
                            .addComponent( downModelButton)
                            .addComponent( removeModelButton)
                        )
                    )
                )
                .addComponent( modelPanel)
            )
        );
    }

    public void set()
    {
        net.stegemann.configuration.System system = configuration.getSystem();
        Types types = configuration.getTypes();
        Models models = configuration.getModels();

        ModelId selectedModelId = system.getSelectedModel();
        Model model = models.getModelFromId( selectedModelId);

        typesList.setModel( new TypesComboBoxModel( types));

        if( model != null)
        {
            Number selectedTypeId = model.getTypeId();
            typesList.setSelectedIndex( types.getIndexFromId( selectedTypeId));

            modelsView = new ModelsView( models, selectedTypeId);
            modelsList.setModel( new ModelsViewComboBoxModel( modelsView));
            modelsList.setSelectedIndex( modelsView.getModelIndexFromId( selectedModelId));
        }
        else
        {
            modelsView = new ModelsView( models, null);
            modelsList.setModel( new ModelsViewComboBoxModel( modelsView));
        }
    }

    @Override
    public void valueChanged( ListSelectionEvent event)
    {
        if( event.getValueIsAdjusting() == true)
        {
            return;
        }

        if( event.getSource() == modelsList)
        {
            modelChanged();
        }
        else if( event.getSource() == typesList)
        {
            typeChanged();
        }
    }

    @Override
    public void actionPerformed( ActionEvent event)
    {
        int selectedTypeIndex = typesList.getSelectedIndex();
        int selectedModelIndex = modelsList.getSelectedIndex();

        if( event.getSource() == systemButton)
        {
            openSystem();
        }
        else if( event.getSource() == createTypeButton)
        {
            createType();
        }
        else if( event.getSource() == upTypeButton)
        {
            upType( selectedTypeIndex);
        }
        else if( event.getSource() == duplicateTypeButton)
        {
            duplicateType( selectedTypeIndex);
        }
        else if( event.getSource() == downTypeButton)
        {
            downType( selectedTypeIndex);
        }
        else if( event.getSource() == removeTypeButton)
        {
            removeType( selectedTypeIndex);
        }
        else if( event.getSource() == createModelButton)
        {
            createModel( selectedTypeIndex);
        }
        else if( event.getSource() == upModelButton)
        {
            upModel( selectedModelIndex);
        }
        else if( event.getSource() == duplicateModelButton)
        {
            duplicateModel( selectedModelIndex);
        }
        else if( event.getSource() == downModelButton)
        {
            downModel( selectedModelIndex);
        }
        else if( event.getSource() == removeModelButton)
        {
            removeModel( selectedModelIndex);
        }
/*
        if( selectedTypeIndex == typesList.getSelectedIndex())
        {
            // In this case, valueChanged wasn't triggered, so we set the panel here.
            typeChanged();
        }

        if( selectedModelIndex == modelsList.getSelectedIndex())
        {
            // In this case, valueChanged wasn't triggered, so we set the panel here.
            modelsView.rescan();
            modelPanel.set( modelsView.getModelFromIndex( selectedModelIndex));
        }
*/
    }

    private void openSystem()
    {
        SystemFrame systemFrame = new SystemFrame( configuration);

        systemFrame.set();
        systemFrame.setLocationRelativeTo( this);
        systemFrame.open();
    }

    private void createType()
    {
        selectType( controller.createType());
    }

    private void upType( int selectedIndex)
    {
        if( selectedIndex == 0)
        {
            return;
        }

        switchTypes( selectedIndex, selectedIndex - 1);
    }

    private void duplicateType( int selectedIndex)
    {
        selectType( controller.duplicateType( selectedIndex));
    }

    private void downType( int selectedIndex)
    {
        if( selectedIndex == configuration.getTypes().getCount() - 1)
        {
            return;
        }

        switchTypes( selectedIndex, selectedIndex + 1);
    }

    private void removeType( int selectedIndex)
    {
        controller.removeType( selectedIndex);

        if( selectedIndex > 0)
        {
            typesList.setSelectedIndex( selectedIndex - 1);
        }
        else
        {
            typeName.attachValue( null);
        }
    }

    private void createModel( int typeIndex)
    {
        Type type = configuration.getTypes().getTypeFromIndex( typeIndex);

        if( type == null)
        {
            return;
        }

        selectModel( controller.createModel( type.getId()));
    }

    private void upModel( int selectedIndex)
    {
        if( selectedIndex == 0)
        {
            return;
        }

        switchModels( selectedIndex, selectedIndex - 1);
    }

    private void duplicateModel( int modelIndex)
    {
        selectModel( controller.duplicateModel( modelsView.getFullModelIndex( modelIndex)));
    }

    private void downModel( int selectedIndex)
    {
        if( selectedIndex == modelsView.getCount() - 1)
        {
            return;
        }

        switchModels( selectedIndex, selectedIndex + 1);
    }

    private void removeModel( int modelIndex)
    {
        controller.removeModel( modelsView.getFullModelIndex( modelIndex));

        // Move model selection one position up.
        if( modelIndex > 0)
        {
            modelsList.setSelectedIndex( modelIndex - 1);
        }
    }

    private void switchTypes( int indexOne, int indexTwo)
    {
        ModelId typeIdOne = configuration.getTypes().getTypeIdFromIndex( indexOne);
        ModelId typeIdTwo = configuration.getTypes().getTypeIdFromIndex( indexTwo);

        controller.switchTypes( typeIdOne, typeIdTwo);

        // Select new type.
        typesList.setSelectedIndex( configuration.getTypes().getIndexFromId( typeIdOne));
    }

    private void switchModels( int indexOne, int indexTwo)
    {
        ModelId modelIdOne = modelsView.getModelIdFromIndex( indexOne);
        ModelId modelIdTwo = modelsView.getModelIdFromIndex( indexTwo);

        controller.switchModels( modelIdOne,modelIdTwo);

        // Select new model.
        modelsView.rescan();
        modelsList.setSelectedIndex( modelsView.getModelIndexFromId( modelIdOne));
    }

    private void modelChanged()
    {
        modelPanel.set( modelsView.getModelFromIndex( modelsList.getSelectedIndex()));
    }

    private void typeChanged()
    {
        Models models = configuration.getModels();
        Types types = configuration.getTypes();

        modelPanel.set( null);

        Type selectedType = types.getTypeFromIndex( typesList.getSelectedIndex());

        if( selectedType != null)
        {
            Number selectedTypeId = selectedType.getId();

            modelsView = new ModelsView( models, selectedTypeId);
            modelsList.setModel( new ModelsViewComboBoxModel( modelsView));

            typeName.attachValue( selectedType.getName());
        }
        else
        {
            typeName.attachValue( null);
        }
    }

    private void selectType( Type type)
    {
        if( type == null)
        {
            return;
        }

        typesList.setSelectedIndex( configuration.getTypes().getIndexFromId( type.getId()));
    }

    private void selectModel( Model model)
    {
        if( model == null)
        {
            return;
        }

        modelsView.rescan();
        modelsList.setSelectedIndex( modelsView.getModelIndexFromId( model.getId()));
    }
}
