package net.stegemann.gui.panel;

import net.stegemann.configuration.*;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.view.ModelsView;
import net.stegemann.controller.Controller;
import net.stegemann.gui.Constants;
import net.stegemann.gui.components.TextComponent;
import net.stegemann.gui.frame.SystemFrame;
import net.stegemann.gui.model.ListCellRenderer;
import net.stegemann.gui.model.ModelsViewComboBoxModel;
import net.stegemann.gui.model.TypesComboBoxModel;

import javax.swing.*;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.Serial;

public class MainPanel extends JPanel
				    implements ActionListener, ListSelectionListener
{
	@Serial
	private static final long serialVersionUID = -4538240657987172398L;

	private final Controller controller;
	private final Configuration configuration;

	private ModelsView modelsView;

	private final JList< Type> typesList;
	private final TextComponent typeName;
	private final JButton addTypeButton;
	private final JButton cloneTypeButton;
	private final JButton removeTypeButton;

	private final JList< Model> modelsList;
	private final JButton addModelButton;
	private final JButton cloneModelButton;
	private final JButton removeModelButton;

	private final ModelPanel modelPanel;

	private final JButton systemButton;

	public MainPanel( Controller useController)
	{
		controller = useController;
		configuration = useController.getConfiguration();
		
		systemButton = new JButton( "System...");
		systemButton.addActionListener( this);

		JSeparator Separator = new JSeparator();

		JLabel TypesListLabel = new JLabel( "Typen:");

		typesList = new JList<>();
		typesList.setSelectionMode( ListSelectionModel.SINGLE_SELECTION);
		typesList.setLayoutOrientation( JList.VERTICAL);
		typesList.addListSelectionListener( this);
		typesList.setCellRenderer( new ListCellRenderer<Type>());

		JScrollPane TypesScrollPane = new JScrollPane( typesList);
		TypesScrollPane.setMinimumSize( new Dimension( 150, 150));
		TypesScrollPane.setVerticalScrollBarPolicy( JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);

		typeName = new TextComponent( Constants.DEFAULT_TEXTFIELD_WIDTH);

		addTypeButton = new JButton( "+");
		addTypeButton.addActionListener( this);
		cloneTypeButton = new JButton( "Kopieren");
		cloneTypeButton.addActionListener( this);
		removeTypeButton = new JButton( "-");
		removeTypeButton.addActionListener( this);

		JLabel ModelsListLabel = new JLabel( "Modelle:");

		modelsList = new JList<>();
		modelsList.setSelectionMode( ListSelectionModel.SINGLE_SELECTION);
		modelsList.setLayoutOrientation( JList.VERTICAL);
		modelsList.addListSelectionListener( this);
		modelsList.setCellRenderer( new ListCellRenderer<Model>());

		JScrollPane ModelsScrollPane = new JScrollPane( modelsList);
		ModelsScrollPane.setMinimumSize( new Dimension( 150, 150));
		ModelsScrollPane.setVerticalScrollBarPolicy( JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);

		addModelButton = new JButton( "+");
		addModelButton.addActionListener( this);
		cloneModelButton = new JButton( "Kopieren");
		cloneModelButton.addActionListener( this);
		removeModelButton = new JButton( "-");
		removeModelButton.addActionListener( this);

		modelPanel = new ModelPanel( useController);

		// Layout elements.
		GroupLayout Layout = new GroupLayout( this);
		setLayout( Layout);

		Layout.setHonorsVisibility( false);
//		Layout.setAutoCreateGaps( true);
		Layout.setAutoCreateContainerGaps( true);

		Layout.setHorizontalGroup( Layout.createParallelGroup( GroupLayout.Alignment.LEADING)
			.addComponent( systemButton)
			.addComponent( Separator)
			.addGroup( Layout.createSequentialGroup()
				.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
					.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
						.addComponent( TypesListLabel)
						.addComponent( TypesScrollPane, javax.swing.GroupLayout.PREFERRED_SIZE,
									   150, Integer.MAX_VALUE)
						.addComponent( typeName)
						.addGroup( Layout.createSequentialGroup()
							.addComponent( addTypeButton)
							.addComponent( cloneTypeButton)
							.addComponent( removeTypeButton)
						)
					)
					.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
						.addComponent( ModelsListLabel)
						.addComponent( ModelsScrollPane, javax.swing.GroupLayout.PREFERRED_SIZE,
									   150, Integer.MAX_VALUE)
						.addGroup( Layout.createSequentialGroup()
							.addComponent( addModelButton)
							.addComponent( cloneModelButton)
							.addComponent( removeModelButton)
						)
					)
				)
				.addComponent( modelPanel)
			)
		);

		Layout.setVerticalGroup( Layout.createSequentialGroup()
			.addComponent( systemButton)
			.addComponent( Separator)
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.LEADING)
				.addGroup( Layout.createSequentialGroup()
					.addGroup( Layout.createSequentialGroup()
						.addComponent( TypesListLabel)
						.addComponent( TypesScrollPane, javax.swing.GroupLayout.PREFERRED_SIZE,
														javax.swing.GroupLayout.PREFERRED_SIZE,
														javax.swing.GroupLayout.PREFERRED_SIZE)
						.addComponent( typeName, javax.swing.GroupLayout.PREFERRED_SIZE,
												 javax.swing.GroupLayout.PREFERRED_SIZE,
												 javax.swing.GroupLayout.PREFERRED_SIZE)
						.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.LEADING)
							.addComponent( addTypeButton)
							.addComponent( cloneTypeButton)
							.addComponent( removeTypeButton)
						)
					)
					.addGroup( Layout.createSequentialGroup()
						.addComponent( ModelsListLabel)
						.addComponent( ModelsScrollPane)
						.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.LEADING)
							.addComponent( addModelButton)
							.addComponent( cloneModelButton)
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
		Types UseTypes = configuration.getTypes();
		Models UseModels = configuration.getModels();

		Number SelectedModel = system.getSelectedModel();
		Model UseModel = UseModels.getModelFromId( SelectedModel);

		typesList.setModel( new TypesComboBoxModel( UseTypes));

		if( UseModel != null)
		{
			Number SelectedType = UseModel.getTypeId();
			typesList.setSelectedIndex( UseTypes.getIndexFromId( SelectedType));

			modelsView = new ModelsView( UseModels, SelectedType);
			modelsList.setModel( new ModelsViewComboBoxModel( modelsView));
			modelsList.setSelectedIndex( modelsView.getModelIndexFromId( SelectedModel));
		}
		else
		{
			modelsView = new ModelsView( UseModels, null);
			modelsList.setModel( new ModelsViewComboBoxModel( modelsView));
		}
	}

	@Override
	public void valueChanged( ListSelectionEvent e)
	{
		if( e.getValueIsAdjusting() == true)
		{
			return;
		}

		if( e.getSource() == modelsList)
		{
			modelPanel.set( modelsView.getModelFromIndex( modelsList.getSelectedIndex()));
		}
		else if( e.getSource() == typesList)
		{
			typeChanged();
		}
	}

	private void typeChanged()
	{
		Models models = configuration.getModels();
		Types types = configuration.getTypes();

		modelPanel.set( null);

		Type SelectedType = types.getTypeFromIndex( typesList.getSelectedIndex());

		if( SelectedType != null)
		{
			Number selectedTypeId = SelectedType.getId();

			modelsView = new ModelsView( models, selectedTypeId);
			modelsList.setModel( new ModelsViewComboBoxModel( modelsView));

			typeName.attachValue( SelectedType.getName());
		}
		else
		{
			typeName.attachValue( null);
		}
	}

	@Override
	public void actionPerformed( ActionEvent event)
	{
		Types types = configuration.getTypes();

		if( event.getSource() == systemButton)
		{
			SystemFrame systemFrame = new SystemFrame( configuration);

			systemFrame.set();
			systemFrame.setLocationRelativeTo( this);
			systemFrame.open();
		}
		else if(( event.getSource() == addTypeButton) ||
				( event.getSource() == cloneTypeButton) ||
				( event.getSource() == removeTypeButton))
		{
			int selectedIndex = typesList.getSelectedIndex();

			if( event.getSource() == addTypeButton)
			{
				Type type = controller.addType();

				if( type == null)
				{
					return;
				}

				typesList.setSelectedIndex( types.getIndexFromId( type.getId()));
			}
			else if( event.getSource() == cloneTypeButton)
			{
				Type NewType = controller.cloneType( selectedIndex);

				if( NewType == null)
				{
					return;
				}

				// Select new type.
				typesList.setSelectedIndex( types.getIndexFromId( NewType.getId()));
			}
			else if( event.getSource() == removeTypeButton)
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

			if( selectedIndex == typesList.getSelectedIndex())
			{
				// In this case, valueChanged wasn't triggered, so we set the panel here.
				typeChanged();
			}
		}
		else if(( event.getSource() == addModelButton) ||
				( event.getSource() == cloneModelButton) ||
				( event.getSource() == removeModelButton))
		{
			int selectedTypeIndex = typesList.getSelectedIndex();
			int selectedModelIndex = modelsList.getSelectedIndex();

			if( event.getSource() == addModelButton)
			{
				Type type = types.getTypeFromIndex( selectedTypeIndex);

				if( type == null)
				{
					return;
				}

				Model model = controller.addModel( type.getId());

				if( model == null)
				{
					return;
				}

				// Select new model.
				modelsView.rescan();
				modelsList.setSelectedIndex( modelsView.getModelIndexFromId( model.getId()));
			}
			else if( event.getSource() == cloneModelButton)
			{
				Model NewModel =
					controller.cloneModel( modelsView.getFullModelIndex( selectedModelIndex));

				if( NewModel == null)
				{
					return;
				}

				// Select new model.
				modelsView.rescan();
				modelsList.setSelectedIndex( modelsView.getModelIndexFromId( NewModel.getId()));
			}
			else if( event.getSource() == removeModelButton)
			{
				controller.removeModel( modelsView.getFullModelIndex( selectedModelIndex));

				// Model selection one position up.
				if( selectedModelIndex > 0)
				{
					modelsList.setSelectedIndex( selectedModelIndex - 1);
				}
			}

			if( selectedModelIndex == modelsList.getSelectedIndex())
			{
				// In this case, valueChanged wasn't triggered, so we set the panel here.
				modelsView.rescan();
				modelPanel.set( modelsView.getModelFromIndex( selectedModelIndex));
			}
		}
	}
}
