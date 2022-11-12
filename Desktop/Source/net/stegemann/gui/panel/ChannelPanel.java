package net.stegemann.gui.panel;

import java.awt.Dimension;

import javax.swing.GroupLayout;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSeparator;

import net.stegemann.configuration.Channel;
import net.stegemann.configuration.Configuration;
import net.stegemann.configuration.Signal;
import net.stegemann.configuration.Model;
import net.stegemann.configuration.source.Sources;
import net.stegemann.configuration.type.Volume;
import net.stegemann.configuration.view.SourcesView;
import net.stegemann.configuration.view.SourcesView.HasEmpty;
import net.stegemann.configuration.view.SourcesView.HasFixed;
import net.stegemann.configuration.view.SourcesView.HasProxies;
import net.stegemann.configuration.view.SourcesView.PickGlobals;
import net.stegemann.gui.components.BoolComponent;
import net.stegemann.gui.components.ChannelModeComponent;
import net.stegemann.gui.components.GraphComponent;
import net.stegemann.gui.components.NumberSliderComponent;
import net.stegemann.gui.components.TextComponent;
import net.stegemann.gui.components.source.SourceWithVolumeComponent;

public class ChannelPanel extends JPanel
{
	private static final long serialVersionUID = 1556370536350374404L;

	private final Configuration configuration;

	private final TextComponent name;

	private final SourceWithVolumeComponent input;
	private final SourceWithVolumeComponent trim;
	private final SourceWithVolumeComponent limit;

	private final BoolComponent reverse;
	private final ChannelModeComponent mode;

	private final NumberSliderComponent[] points;

	private final GraphComponent graph;

	public ChannelPanel( Configuration UseConfiguration)
	{
		setMinimumSize( new Dimension( 450, 450));

		configuration = UseConfiguration;

		JLabel NameLabel = new JLabel( "Name:");
		name = new TextComponent( net.stegemann.gui.Constants.DEFAULT_TEXTFIELD_WIDTH);

		JSeparator Separator = new JSeparator();

		JLabel InputLabel = new JLabel( "Quelle:");
		input = new SourceWithVolumeComponent( Signal.MINIMUM_VALUE / Channel.INPUT_SIGNAL_PER_VALUE,
								    						Signal.MAXIMUM_VALUE / Channel.INPUT_SIGNAL_PER_VALUE);

		JLabel TrimLabel = new JLabel( "Trimm:");
		trim = new SourceWithVolumeComponent( Signal.MINIMUM_VALUE / Channel.TRIM_SIGNAL_PER_VALUE,
								   					  Signal.MAXIMUM_VALUE / Channel.TRIM_SIGNAL_PER_VALUE);

		JLabel LimitLabel = new JLabel( "Limit:");
		limit = new SourceWithVolumeComponent( Signal.MINIMUM_VALUE / Channel.LIMIT_SIGNAL_PER_VALUE,
														   Signal.MAXIMUM_VALUE / Channel.LIMIT_SIGNAL_PER_VALUE);

		JLabel ModeLabel = new JLabel( "Modus:");
		mode = new ChannelModeComponent();

		JLabel ReverseLabel = new JLabel( "Umkehr:");
		reverse = new BoolComponent();

		JLabel PointLabels[] = new JLabel[ Channel.POINTS];
		PointLabels[ 0] = new JLabel( "Unten:");
		PointLabels[ 1] = new JLabel( "Mitte:");
		PointLabels[ 2] = new JLabel( "Oben:");
		points = new NumberSliderComponent[ Channel.POINTS];
		points[ 0] =
			new NumberSliderComponent( Signal.MINIMUM_VALUE / Channel.POINT_SIGNAL_PER_VALUE,
									   		Signal.MAXIMUM_VALUE / Channel.POINT_SIGNAL_PER_VALUE);
		points[ 1] =
			new NumberSliderComponent( Signal.MINIMUM_VALUE / Channel.POINT_SIGNAL_PER_VALUE,
									   	   Signal.MAXIMUM_VALUE / Channel.POINT_SIGNAL_PER_VALUE);
		points[ 2] =
			new NumberSliderComponent( Signal.MINIMUM_VALUE / Channel.POINT_SIGNAL_PER_VALUE,
									   		Signal.MAXIMUM_VALUE / Channel.POINT_SIGNAL_PER_VALUE);

		graph = new GraphComponent();

		// Layout elements.
		GroupLayout Layout = new GroupLayout( this);
		setLayout( Layout);

//		Layout.setAutoCreateGaps( true);
		Layout.setAutoCreateContainerGaps( true);

		Layout.setHorizontalGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
			.addComponent( Separator)
			.addGroup( Layout.createSequentialGroup()
				.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.TRAILING)
					.addComponent( NameLabel)
					.addComponent( InputLabel)
					.addComponent( TrimLabel)
					.addComponent( LimitLabel)
					.addComponent( ModeLabel)
					.addComponent( PointLabels[ 0])
					.addComponent( PointLabels[ 1])
					.addComponent( PointLabels[ 2])
					.addComponent( ReverseLabel)
				)
				.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.LEADING)
					.addComponent( name, javax.swing.GroupLayout.PREFERRED_SIZE,
										 		javax.swing.GroupLayout.PREFERRED_SIZE,
										 		javax.swing.GroupLayout.PREFERRED_SIZE)
					.addComponent( input)
					.addComponent( trim)
					.addComponent( limit)
					.addGroup( Layout.createSequentialGroup()
						.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.LEADING)
							.addComponent( mode)
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
				.addComponent( NameLabel)
				.addComponent( name, javax.swing.GroupLayout.PREFERRED_SIZE,
									 		javax.swing.GroupLayout.PREFERRED_SIZE,
									 		javax.swing.GroupLayout.PREFERRED_SIZE)
			)
			.addComponent( Separator, javax.swing.GroupLayout.PREFERRED_SIZE,
									  		  javax.swing.GroupLayout.PREFERRED_SIZE,
									  		  javax.swing.GroupLayout.PREFERRED_SIZE)
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
						.addComponent( ModeLabel)
						.addComponent( mode, javax.swing.GroupLayout.PREFERRED_SIZE,
											 		javax.swing.GroupLayout.PREFERRED_SIZE,
											 		javax.swing.GroupLayout.PREFERRED_SIZE)
					)
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
					.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
						.addComponent( ReverseLabel)
						.addComponent( reverse)
					)
				)
				.addComponent( graph)
			)
		);
	}

	public void set( Model UseModel, Channel UseChannel)
	{
		if( UseChannel == null)
		{
			setVisible( false);

			return;
		}

		name.attachValue( UseChannel.getName());

		Sources UseSources = configuration.getSources();
		SourcesView UseSourcesView =
			new SourcesView( UseSources, PickGlobals.Yes, UseModel.getTypeId(), UseModel.getId(),
							 HasEmpty.Yes, HasFixed.Yes, HasProxies.Yes);

		input.set( UseSourcesView, UseChannel.getInput());
		trim.set( UseSourcesView, UseChannel.getTrim());
		limit.set( UseSourcesView, UseChannel.getLimit());

		mode.attachValue( UseChannel.getMode());
		reverse.attachValue( UseChannel.getReverse());

		for( int Index = 0; Index < Channel.POINTS; Index++)
		{
			points[ Index].attachValue( UseChannel.getPoint( Index));
		}

		Volume[] Points = new Volume[ Channel.POINTS];

		for( int Index = 0; Index < Points.length; Index++)
		{
			Points[ Index] = UseChannel.getPoint( Index);
		}

		graph.setPoints( Points);
		graph.setMode( UseChannel.getMode());

		setVisible( true);
	}
}
