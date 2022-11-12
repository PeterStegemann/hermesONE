package net.stegemann.gui.panel.source;

import javax.swing.GroupLayout;
import javax.swing.GroupLayout.ParallelGroup;
import javax.swing.GroupLayout.SequentialGroup;

import net.stegemann.configuration.Configuration;
import net.stegemann.configuration.Model;
import net.stegemann.configuration.Signal;
import net.stegemann.configuration.source.Mix;
import net.stegemann.configuration.type.SourceWithVolume;
import net.stegemann.gui.components.source.SourceWithVolumeComponent;

public class MixPanel extends SpecificSourcePanel
{
	private static final long serialVersionUID = -7497608305714139003L;

	private final SourceWithVolumeComponent[] sourceTupelPanel;

	public MixPanel( Configuration UseConfiguration)
	{
		super( UseConfiguration);

		sourceTupelPanel = new SourceWithVolumeComponent[ Mix.INPUTS];

		// Layout elements.
		GroupLayout Layout = new GroupLayout( this);
		setLayout( Layout);

		ParallelGroup HorizontalGroup = Layout.createParallelGroup( GroupLayout.Alignment.CENTER);
		SequentialGroup VerticalGroup = Layout.createSequentialGroup();

		for( int Index = 0; Index < Mix.INPUTS; Index++)
		{
			sourceTupelPanel[ Index] =
				new SourceWithVolumeComponent( Signal.MINIMUM_VALUE / Mix.INPUT_SIGNAL_PER_VALUE,
									Signal.MAXIMUM_VALUE / Mix.INPUT_SIGNAL_PER_VALUE);

			HorizontalGroup.addComponent( sourceTupelPanel[ Index],
										  javax.swing.GroupLayout.PREFERRED_SIZE,
										  javax.swing.GroupLayout.PREFERRED_SIZE,
										  javax.swing.GroupLayout.PREFERRED_SIZE);

			VerticalGroup.addComponent( sourceTupelPanel[ Index],
										javax.swing.GroupLayout.PREFERRED_SIZE,
										javax.swing.GroupLayout.PREFERRED_SIZE,
										javax.swing.GroupLayout.PREFERRED_SIZE);
		}

		Layout.setHorizontalGroup( HorizontalGroup);
		Layout.setVerticalGroup( VerticalGroup);
	}

	public void set( Model model, Mix source)
	{
		super.set( model, source);

		int Index = 0;

		for( SourceWithVolume CurrentSourceTupel: source.getInputs())
		{
			sourceTupelPanel[ Index++].set( sourcesViewWithFixed, CurrentSourceTupel);
		}
	}
}
