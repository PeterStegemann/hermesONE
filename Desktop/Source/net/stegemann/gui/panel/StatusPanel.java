package net.stegemann.gui.panel;

import javax.swing.GroupLayout;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSeparator;

import net.stegemann.configuration.Configuration;
import net.stegemann.configuration.Model;
import net.stegemann.configuration.source.Timer;
import net.stegemann.configuration.source.Sources;
import net.stegemann.configuration.view.SourcesView;
import net.stegemann.configuration.view.SourcesView.HasEmpty;
import net.stegemann.configuration.view.SourcesView.HasFixed;
import net.stegemann.configuration.view.SourcesView.HasProxies;
import net.stegemann.configuration.view.SourcesView.PickGlobals;
import net.stegemann.gui.components.source.SourceComponent;

public class StatusPanel extends JPanel
{
	private static final long serialVersionUID = 683481639057295222L;

	private final Configuration configuration;

	private final SourceComponent topTimeId;
	private final SourceComponent bottomTimeId;

	private final SourceComponent leftSideId;
	private final SourceComponent leftBottomId;
	private final SourceComponent rightSideId;
	private final SourceComponent rightBottomId;

	public StatusPanel( Configuration UseConfiguration)
	{
		configuration = UseConfiguration;

		JLabel TopTimeLabel = new JLabel( "Uhr Oben:");
		topTimeId = new SourceComponent();

		JLabel BottomTimeLabel = new JLabel( "Uhr Unten:");
		bottomTimeId = new SourceComponent();

		JSeparator Separator = new JSeparator();

		JLabel LeftSideLabel = new JLabel( "Linke Seite:");
		leftSideId = new SourceComponent();

		JLabel LeftBottomLabel = new JLabel( "Links Unten:");
		leftBottomId = new SourceComponent();

		JLabel RightSideLabel = new JLabel( "Rechte Seite:");
		rightSideId = new SourceComponent();

		JLabel RightBottomLabel = new JLabel( "Rechts Unten:");
		rightBottomId = new SourceComponent();

		// Layout elements.
		GroupLayout Layout = new GroupLayout( this);
		setLayout( Layout);

//		Layout.setAutoCreateGaps( true);
		Layout.setAutoCreateContainerGaps( true);

		Layout.setHorizontalGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
			.addComponent( Separator)
			.addGroup( Layout.createSequentialGroup()
				.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.TRAILING)
					.addComponent( TopTimeLabel)
					.addComponent( BottomTimeLabel)
					.addComponent( LeftSideLabel)
					.addComponent( LeftBottomLabel)
					.addComponent( RightSideLabel)
					.addComponent( RightBottomLabel)
				)
				.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
					.addComponent( topTimeId)
					.addComponent( bottomTimeId)
					.addComponent( leftSideId)
					.addComponent( leftBottomId)
					.addComponent( rightSideId)
					.addComponent( rightBottomId)
				)
			)
		);

		Layout.setVerticalGroup( Layout.createSequentialGroup()
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( TopTimeLabel)
				.addComponent( topTimeId)
			)
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( BottomTimeLabel)
				.addComponent( bottomTimeId)
			)
			.addComponent( Separator)
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( LeftSideLabel)
				.addComponent( leftSideId)
			)
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( LeftBottomLabel)
				.addComponent( leftBottomId)
			)
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( RightSideLabel)
				.addComponent( rightSideId)
			)
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( RightBottomLabel)
				.addComponent( rightBottomId)
			)
		);
	}

	public void Set( Model UseModel)
	{
		Sources UseSources = configuration.getSources();

		SourcesView UseSourcesView =
			new SourcesView( UseSources, PickGlobals.Yes, UseModel.getTypeId(), UseModel.getId(),
							 	  Timer.class, HasEmpty.Yes, HasFixed.No, HasProxies.No);

		topTimeId.setSourcesView( UseSourcesView);
		topTimeId.attachValue( UseModel.getStatusTimeId( Model.StatusTime.TOP));
	
		bottomTimeId.setSourcesView( UseSourcesView);
		bottomTimeId.attachValue( UseModel.getStatusTimeId( Model.StatusTime.BOTTOM));

		UseSourcesView =
			new SourcesView( UseSources, PickGlobals.Yes, UseModel.getTypeId(), UseModel.getId(),
							 	  HasEmpty.Yes, HasFixed.Yes, HasProxies.Yes);

		leftSideId.setSourcesView( UseSourcesView);
		leftSideId.attachValue( UseModel.getStatusSourceId( Model.StatusSource.LEFT_SIDE));

		leftBottomId.setSourcesView( UseSourcesView);
		leftBottomId.attachValue( UseModel.getStatusSourceId( Model.StatusSource.LEFT_BOTTOM));

		rightSideId.setSourcesView( UseSourcesView);
		rightSideId.attachValue( UseModel.getStatusSourceId( Model.StatusSource.RIGHT_SIDE));

		rightBottomId.setSourcesView( UseSourcesView);
		rightBottomId.attachValue( UseModel.getStatusSourceId( Model.StatusSource.RIGHT_BOTTOM));
	}
}
