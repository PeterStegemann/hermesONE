package net.stegemann.gui.panel.source;

import net.stegemann.configuration.Configuration;
import net.stegemann.configuration.Model;
import net.stegemann.configuration.Signal;
import net.stegemann.configuration.source.Trimmer;
import net.stegemann.configuration.type.Volume;
import net.stegemann.gui.components.BoolComponent;
import net.stegemann.gui.components.GraphComponent;
import net.stegemann.gui.components.NumberSliderComponent;
import net.stegemann.gui.components.source.SourceWithVolumeComponent;

import javax.swing.*;

public class TrimPanel extends SpecificSourcePanel
{
	private static final long serialVersionUID	= -1846522709554529521L;

	private final SourceWithVolumeComponent input;
	private final SourceWithVolumeComponent trim;
	private final SourceWithVolumeComponent limit;

	private final BoolComponent reverse;

	private final NumberSliderComponent[] points;

	private final GraphComponent graph;

	public TrimPanel( Configuration UseConfiguration)
	{
		super( UseConfiguration);

		JLabel InputLabel = new JLabel( "Quelle:");
		input =
			new SourceWithVolumeComponent( Signal.MINIMUM_VALUE / Trimmer.INPUT_SIGNAL_PER_VALUE,
										   			 Signal.MAXIMUM_VALUE / Trimmer.INPUT_SIGNAL_PER_VALUE);

		JLabel TrimLabel = new JLabel( "Trimm:");
		trim =
			new SourceWithVolumeComponent( Signal.MINIMUM_VALUE / Trimmer.TRIM_SIGNAL_PER_VALUE,
										   			 Signal.MAXIMUM_VALUE / Trimmer.TRIM_SIGNAL_PER_VALUE);

		JLabel LimitLabel = new JLabel( "Limit:");
		limit =
			new SourceWithVolumeComponent( Signal.MINIMUM_VALUE / Trimmer.LIMIT_SIGNAL_PER_VALUE,
										   			 Signal.MAXIMUM_VALUE / Trimmer.LIMIT_SIGNAL_PER_VALUE);

		JLabel ReverseLabel = new JLabel( "Umkehr:");
		reverse = new BoolComponent();

		JLabel PointLabels[] = new JLabel[ Trimmer.POINTS];
		PointLabels[ 0] = new JLabel( "Oben:");
		PointLabels[ 1] = new JLabel( "Mitte:");
		PointLabels[ 2] = new JLabel( "Unten:");
		points = new NumberSliderComponent[ Trimmer.POINTS];

		for(int Index = 0; Index < Trimmer.POINTS; Index++)
		{
			points[ Index] =
				new NumberSliderComponent( Signal.MINIMUM_VALUE / Trimmer.POINT_SIGNAL_PER_VALUE,
										   			Signal.MAXIMUM_VALUE / Trimmer.POINT_SIGNAL_PER_VALUE);
		}

		graph = new GraphComponent();

		// Layout elements.
		GroupLayout Layout = new GroupLayout( this);
		setLayout( Layout);

//		Layout.setAutoCreateGaps( true);
		Layout.setAutoCreateContainerGaps( true);

		Layout.setHorizontalGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
			.addGroup( Layout.createSequentialGroup()
				.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.TRAILING)
					.addComponent( InputLabel)
					.addComponent( TrimLabel)
					.addComponent( LimitLabel)
					.addComponent( PointLabels[ 0])
					.addComponent( PointLabels[ 1])
					.addComponent( PointLabels[ 2])
					.addComponent( ReverseLabel)
				)
				.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.LEADING)
					.addComponent( input)
					.addComponent( trim)
					.addComponent( limit)
					.addGroup( Layout.createSequentialGroup()
						.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.TRAILING)
							.addComponent( points[ 0])
							.addComponent( points[ 1])
							.addComponent( points[ 2])
							.addComponent( reverse)
						)
						.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.LEADING)
							.addComponent( graph)
						)
					)
				)
			)
		);

		Layout.setVerticalGroup( Layout.createSequentialGroup()
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( InputLabel)
				.addComponent( input, javax.swing.GroupLayout.PREFERRED_SIZE,
									  		 javax.swing.GroupLayout.PREFERRED_SIZE,
									  		 javax.swing.GroupLayout.PREFERRED_SIZE)
			)
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( TrimLabel)
				.addComponent( trim, javax.swing.GroupLayout.PREFERRED_SIZE,
									 		javax.swing.GroupLayout.PREFERRED_SIZE,
									 		javax.swing.GroupLayout.PREFERRED_SIZE)
			)
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( LimitLabel)
				.addComponent( limit, javax.swing.GroupLayout.PREFERRED_SIZE,
									  		 javax.swing.GroupLayout.PREFERRED_SIZE,
									  		 javax.swing.GroupLayout.PREFERRED_SIZE)
			)
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.LEADING)
				.addGroup( Layout.createSequentialGroup()
					.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
						.addGroup( Layout.createSequentialGroup()
							.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
								.addComponent( PointLabels[ 0])
								.addComponent( points[ 0], javax.swing.GroupLayout.PREFERRED_SIZE,
														   			javax.swing.GroupLayout.PREFERRED_SIZE,
														   			javax.swing.GroupLayout.PREFERRED_SIZE)
							)
							.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
								.addComponent( PointLabels[ 1])
								.addComponent( points[ 1], javax.swing.GroupLayout.PREFERRED_SIZE,
																	javax.swing.GroupLayout.PREFERRED_SIZE,
																	javax.swing.GroupLayout.PREFERRED_SIZE)
							)
							.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
								.addComponent( PointLabels[ 2])
								.addComponent( points[ 2], javax.swing.GroupLayout.PREFERRED_SIZE,
														   			javax.swing.GroupLayout.PREFERRED_SIZE,
														   			javax.swing.GroupLayout.PREFERRED_SIZE)
							)
						)
					)
					.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.TRAILING)
						.addComponent( ReverseLabel)
						.addComponent( reverse)
					)
				)
				.addComponent( graph)
			)
		);
	}

	public void set( Model UseModel, Trimmer Source)
	{
		super.set( UseModel, Source);

		input.set( sourcesViewWithFixed, Source.getInput());
		trim.set( sourcesViewWithFixed, Source.getTrim());
		limit.set( sourcesViewWithFixed, Source.getLimit());

		reverse.attachValue( Source.getReverse());

		for(int Index = 0; Index < Trimmer.POINTS; Index++)
		{
			points[ Index].attachValue( Source.getPoint( Index));
		}

		Volume[] Points = new Volume[ Trimmer.POINTS];

		for( int Index = 0; Index < Points.length; Index++)
		{
			Points[ Index] = Source.getPoint( Index);
		}

		graph.setPoints( Points);
//		graph.SetMode( UseSource.getMode());
	}
}
