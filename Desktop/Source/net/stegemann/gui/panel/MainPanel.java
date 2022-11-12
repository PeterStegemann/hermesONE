package net.stegemann.gui.panel;

import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.GroupLayout;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JSeparator;
import javax.swing.ListSelectionModel;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;

import net.stegemann.configuration.Configuration;
import net.stegemann.configuration.Model;
import net.stegemann.configuration.Models;
import net.stegemann.configuration.Type;
import net.stegemann.configuration.Types;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.view.ModelsView;
import net.stegemann.controller.Controller;
import net.stegemann.gui.Constants;
import net.stegemann.gui.components.TextComponent;
import net.stegemann.gui.frame.SystemFrame;
import net.stegemann.gui.model.ListCellRenderer;
import net.stegemann.gui.model.ModelsViewComboBoxModel;
import net.stegemann.gui.model.TypesComboBoxModel;

public class MainPanel extends JPanel
				    implements ActionListener, ListSelectionListener
{
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

	public MainPanel( Controller UseController)
	{
		controller = UseController;
		configuration = UseController.getConfiguration();
		
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

		modelPanel = new ModelPanel( UseController);

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

	public void Set()
	{
		net.stegemann.configuration.System UseSystem = configuration.getSystem();
		Types UseTypes = configuration.getTypes();
		Models UseModels = configuration.getModels();

		Number SelectedModel = UseSystem.getSelectedModel();
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
		if( e.getValueIsAdjusting() == false)
		{
			if( e.getSource() == modelsList)
			{
				modelPanel.Set( modelsView.getModelFromIndex( modelsList.getSelectedIndex()));
			}
			else if( e.getSource() == typesList)
			{
				typeChanged();
			}
		}
	}

	private void typeChanged()
	{
		Models UseModels = configuration.getModels();
		Types UseTypes = configuration.getTypes();

		modelPanel.Set( null);

		Type SelectedType = UseTypes.getTypeFromIndex( typesList.getSelectedIndex());

		if( SelectedType != null)
		{
			Number SelectedTypeId = SelectedType.getId();

			modelsView = new ModelsView( UseModels, SelectedTypeId);
			modelsList.setModel( new ModelsViewComboBoxModel( modelsView));

			typeName.attachValue( SelectedType.getName());
		}
		else
		{
			typeName.attachValue( null);
		}
	}

	@Override
	public void actionPerformed( ActionEvent e)
	{
		Types UseTypes = configuration.getTypes();

		if( e.getSource() == systemButton)
		{
			SystemFrame System = new SystemFrame( configuration);

			System.Set();
			System.setLocationRelativeTo( this);
			System.Open();
		}
		else if(( e.getSource() == addTypeButton) ||
				( e.getSource() == cloneTypeButton) ||
				( e.getSource() == removeTypeButton))
		{
			int SelectedTypeIndex = typesList.getSelectedIndex();

			if( e.getSource() == addTypeButton)
			{
				Type NewType = controller.addType();

				if( NewType == null)
				{
					return;
				}

				typesList.setSelectedIndex( UseTypes.getIndexFromId( NewType.getId()));
			}
			else if( e.getSource() == cloneTypeButton)
			{
				Type NewType = controller.cloneType( SelectedTypeIndex);

				if( NewType == null)
				{
					return;
				}

				// Select new type.
				typesList.setSelectedIndex( UseTypes.getIndexFromId( NewType.getId()));
			}
			else if( e.getSource() == removeTypeButton)
			{
				controller.removeType( SelectedTypeIndex);

				if( SelectedTypeIndex > 0)
				{
					typesList.setSelectedIndex( SelectedTypeIndex - 1);
				}
				else
				{
					typeName.attachValue( null);
				}
			}

			if( SelectedTypeIndex == typesList.getSelectedIndex())
			{
				// In this case, valueChanged wasn't triggered, so we set the panel here.
				typeChanged();
			}
		}
		else if(( e.getSource() == addModelButton) ||
				( e.getSource() == cloneModelButton) ||
				( e.getSource() == removeModelButton))
		{
			int SelectedTypeIndex = typesList.getSelectedIndex();
			int SelectedModelIndex = modelsList.getSelectedIndex();

			if( e.getSource() == addModelButton)
			{
				Type Type = UseTypes.getTypeFromIndex( SelectedTypeIndex);

				if( Type == null)
				{
					return;
				}

				Model NewModel = controller.addModel( Type.getId());

				if( NewModel == null)
				{
					return;
				}

				// Select new model.
				modelsView.rescan();
				modelsList.setSelectedIndex( modelsView.getModelIndexFromId( NewModel.getId()));
			}
			else if( e.getSource() == cloneModelButton)
			{
				Model NewModel =
					controller.cloneModel( modelsView.getFullModelIndex( SelectedModelIndex));

				if( NewModel == null)
				{
					return;
				}

				// Select new model.
				modelsView.rescan();
				modelsList.setSelectedIndex( modelsView.getModelIndexFromId( NewModel.getId()));
			}
			else if( e.getSource() == removeModelButton)
			{
				controller.removeModel( modelsView.getFullModelIndex( SelectedModelIndex));

				// Model selection one position up.
				if( SelectedModelIndex > 0)
				{
					modelsList.setSelectedIndex( SelectedModelIndex - 1);
				}
			}

			if( SelectedModelIndex == modelsList.getSelectedIndex())
			{
				// In this case, valueChanged wasn't triggered, so we set the panel here.
				modelsView.rescan();
				modelPanel.Set( modelsView.getModelFromIndex( SelectedModelIndex));
			}
		}
	}
}
