package net.stegemann.gui.panel;

import java.awt.Dimension;import java.io.Serial;

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
	@Serial
	private static final long serialVersionUID = 1556370536350374404L;

	private final Configuration configuration;

	private final TextComponent name;

	private final SourceWithVolumeComponent input, trim, limit;

	private final BoolComponent reverse;
	private final ChannelModeComponent mode;

	private final NumberSliderComponent[] points;

	private final GraphComponent graph;

	public ChannelPanel( Configuration UseConfiguration)
	{
		setMinimumSize( new Dimension( 450, 450));

		configuration = UseConfiguration;

		JLabel nameLabel = new JLabel( "Name:");
		name = new TextComponent( net.stegemann.gui.Constants.DEFAULT_TEXTFIELD_WIDTH);

		JSeparator separator = new JSeparator();

		JLabel inputLabel = new JLabel( "Quelle:");
		input = sourceWithVolumeComponent( Channel.INPUT_SIGNAL_PER_VALUE);

		JLabel trimLabel = new JLabel( "Trimm:");
		trim = sourceWithVolumeComponent( Channel.TRIM_SIGNAL_PER_VALUE);

		JLabel limitLabel = new JLabel( "Limit:");
		limit = sourceWithVolumeComponent( Channel.LIMIT_SIGNAL_PER_VALUE);

		JLabel modeLabel = new JLabel( "Modus:");
		mode = new ChannelModeComponent();

		JLabel reverseLabel = new JLabel( "Umkehr:");
		reverse = new BoolComponent();

		JLabel[] PointLabels = new JLabel[ Channel.POINTS];
		PointLabels[ 0] = new JLabel( "Unten:");
		PointLabels[ 1] = new JLabel( "Mitte:");
		PointLabels[ 2] = new JLabel( "Oben:");
		points = new NumberSliderComponent[ Channel.POINTS];
		points[ 0] = numberSliderComponent();
		points[ 1] = numberSliderComponent();
		points[ 2] = numberSliderComponent();

		graph = new GraphComponent();

		// Layout elements.
		GroupLayout Layout = new GroupLayout( this);
		setLayout( Layout);

//		Layout.setAutoCreateGaps( true);
		Layout.setAutoCreateContainerGaps( true);

		Layout.setHorizontalGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
			.addComponent( separator)
			.addGroup( Layout.createSequentialGroup()
				.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.TRAILING)
					.addComponent( nameLabel)
					.addComponent( inputLabel)
					.addComponent( trimLabel)
					.addComponent( limitLabel)
					.addComponent( modeLabel)
					.addComponent( PointLabels[ 0])
					.addComponent( PointLabels[ 1])
					.addComponent( PointLabels[ 2])
					.addComponent( reverseLabel)
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
				.addComponent( nameLabel)
				.addComponent( name, javax.swing.GroupLayout.PREFERRED_SIZE,
									 		javax.swing.GroupLayout.PREFERRED_SIZE,
									 		javax.swing.GroupLayout.PREFERRED_SIZE)
			)
			.addComponent( separator, javax.swing.GroupLayout.PREFERRED_SIZE,
									  		  javax.swing.GroupLayout.PREFERRED_SIZE,
									  		  javax.swing.GroupLayout.PREFERRED_SIZE)
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( inputLabel)
				.addComponent( input, javax.swing.GroupLayout.PREFERRED_SIZE,
											 javax.swing.GroupLayout.PREFERRED_SIZE,
											 javax.swing.GroupLayout.PREFERRED_SIZE)
			)
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( trimLabel)
				.addComponent( trim, javax.swing.GroupLayout.PREFERRED_SIZE,
									 		javax.swing.GroupLayout.PREFERRED_SIZE,
									 		javax.swing.GroupLayout.PREFERRED_SIZE)
			)
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( limitLabel)
				.addComponent( limit, javax.swing.GroupLayout.PREFERRED_SIZE,
									  		 javax.swing.GroupLayout.PREFERRED_SIZE,
									  		 javax.swing.GroupLayout.PREFERRED_SIZE)
			)
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.LEADING)
				.addGroup( Layout.createSequentialGroup()
					.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
						.addComponent( modeLabel)
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
						.addComponent( reverseLabel)
						.addComponent( reverse)
					)
				)
				.addComponent( graph)
			)
		);
	}

	public void set( Model model, Channel channel)
	{
		if( channel == null)
		{
			setVisible( false);

			return;
		}

		name.attachValue( channel.getName());

		Sources sources = configuration.getSources();
		SourcesView sourcesView = new SourcesView
		(
            sources, PickGlobals.Yes, model.getTypeId(), model.getId(), HasEmpty.Yes, HasFixed.Yes, HasProxies.Yes
        );

		input.set( sourcesView, channel.getInput());
		trim.set( sourcesView, channel.getTrim());
		limit.set( sourcesView, channel.getLimit());

		mode.attachValue( channel.getMode());
		reverse.attachValue( channel.getReverse());

		for( int index = 0; index < Channel.POINTS; index++)
		{
			points[ index].attachValue( channel.getPoint( index));
		}

		Volume[] points = new Volume[ Channel.POINTS];

		for( int index = 0; index < points.length; index++)
		{
			points[ index] = channel.getPoint( index);
		}

		graph.setPoints( points);
		graph.setMode( channel.getMode());

		setVisible( true);
	}

    private SourceWithVolumeComponent sourceWithVolumeComponent( int signalPerValue)
    {
        return new SourceWithVolumeComponent
        (
            Signal.MINIMUM_VALUE / signalPerValue, Signal.MAXIMUM_VALUE / signalPerValue
        );
    }

    private static NumberSliderComponent numberSliderComponent()
    {
        return new NumberSliderComponent
        (
            Signal.MINIMUM_VALUE / Channel.POINT_SIGNAL_PER_VALUE,
            Signal.MAXIMUM_VALUE / Channel.POINT_SIGNAL_PER_VALUE
        );
    }
}
