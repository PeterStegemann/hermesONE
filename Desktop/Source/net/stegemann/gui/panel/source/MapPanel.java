package net.stegemann.gui.panel.source;

import java.util.ArrayList;

import javax.swing.GroupLayout;
import javax.swing.JLabel;
import javax.swing.GroupLayout.Group;

import net.stegemann.configuration.Configuration;
import net.stegemann.configuration.Model;
import net.stegemann.configuration.Signal;
import net.stegemann.configuration.source.Map;
import net.stegemann.configuration.source.Source;
import net.stegemann.configuration.type.SourceWithVolume;
import net.stegemann.configuration.type.Volume;
import net.stegemann.gui.components.GraphComponent;
import net.stegemann.gui.components.source.SourceWithVolumeComponent;

public class MapPanel extends SpecificSourcePanel
{
	private static final long serialVersionUID = 1160706832820802495L;

	private final SourceWithVolumeComponent input;
	private final GraphComponent graph;

	private final SourceWithVolumeComponent[] sourceTupelPanel;

	public MapPanel( Configuration UseConfiguration)
	{
		super( UseConfiguration);
	
		JLabel InputLabel = new JLabel( "Quelle:");
		input = new SourceWithVolumeComponent( Signal.MINIMUM_VALUE / Map.INPUT_SIGNAL_PER_VALUE,
									Signal.MAXIMUM_VALUE / Map.INPUT_SIGNAL_PER_VALUE);

		graph = new GraphComponent();

		sourceTupelPanel = new SourceWithVolumeComponent[ Map.POINTS];

		// Layout elements.
		GroupLayout Layout = new GroupLayout( this);
		setLayout( Layout);

		Group HorizontalGroup = Layout.createParallelGroup( GroupLayout.Alignment.LEADING);

		Layout.setHorizontalGroup( Layout.createParallelGroup( GroupLayout.Alignment.LEADING)
		 	.addGroup( Layout.createSequentialGroup()
				.addComponent( InputLabel)
				.addComponent( input, javax.swing.GroupLayout.PREFERRED_SIZE,
									  javax.swing.GroupLayout.PREFERRED_SIZE,
									  javax.swing.GroupLayout.PREFERRED_SIZE)
			)
		 	.addGroup( Layout.createSequentialGroup()
		 		.addGroup( HorizontalGroup)
				.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.LEADING)
					.addComponent( graph)
				)
			)
		);

		Group VerticalGroup = Layout.createSequentialGroup();

		Layout.setVerticalGroup( Layout.createSequentialGroup()
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( InputLabel)
				.addComponent( input, javax.swing.GroupLayout.PREFERRED_SIZE,
									  javax.swing.GroupLayout.PREFERRED_SIZE,
									  javax.swing.GroupLayout.PREFERRED_SIZE)
			)
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
		 		.addGroup( VerticalGroup)
				.addComponent( graph)
			)
		);

		for( int Index = 0; Index < Map.POINTS; Index++)
		{
			sourceTupelPanel[ Index] =
				new SourceWithVolumeComponent( Signal.MINIMUM_VALUE / Map.POINT_SIGNAL_PER_VALUE,
									Signal.MAXIMUM_VALUE / Map.POINT_SIGNAL_PER_VALUE);
//			NewSourceTupelPanel.SetCompact( true);

			HorizontalGroup.addComponent( sourceTupelPanel[ Index],
										  javax.swing.GroupLayout.PREFERRED_SIZE,
										  javax.swing.GroupLayout.PREFERRED_SIZE,
										  javax.swing.GroupLayout.PREFERRED_SIZE);

			VerticalGroup.addComponent( sourceTupelPanel[ Index],
										javax.swing.GroupLayout.PREFERRED_SIZE,
										javax.swing.GroupLayout.PREFERRED_SIZE,
										javax.swing.GroupLayout.PREFERRED_SIZE);
		}
	}

	public void set( Model UseModel, Map UseSource)
	{
		super.set( UseModel, UseSource);

		input.set( sourcesViewWithoutFixed, UseSource.getInput());

		ArrayList< Volume> Points = new ArrayList< Volume>();

		int Index = 0;

		for( SourceWithVolume Point: UseSource.getPoints())
		{
			sourceTupelPanel[ Index++].set( sourcesViewWithFixed, Point);

			if( Point.getSourceId().getValue() != Source.SOURCE_NONE)
			{
				Points.add( Point.getVolume());
			}
		}

		Volume[] PointsArray = new Volume[ Points.size()];

		Index = 0;

		for( Volume PointVolume: Points)
		{
			PointsArray[ Index++] = PointVolume;
		}

		graph.setPoints( PointsArray);
	}
}
