package net.stegemann.gui.components.source;

import javax.swing.BoxLayout;
import javax.swing.JPanel;

import net.stegemann.configuration.type.SourceWithVolume;
import net.stegemann.configuration.view.SourcesView;
import net.stegemann.gui.components.NumberSliderComponent;

public class SourceWithVolumeComponent extends JPanel
{
	private static final long serialVersionUID = -5575143340428100254L;

	private final SourceComponent sourceId;
	private final NumberSliderComponent volume;

	private boolean compact;

	public SourceWithVolumeComponent( int Minimum, int Maximum)
	{
		sourceId = new SourceComponent();
		volume = new NumberSliderComponent( Minimum, Maximum);

		setCompact( false);
	}

	public void set( SourcesView UseSourcesView, SourceWithVolume sourceTupel)
	{
		sourceId.setSourcesView( UseSourcesView);
		sourceId.attachValue( sourceTupel.getSourceId());
		volume.attachValue( sourceTupel.getVolume());
	}

	public void setCompact( boolean compact)
	{
		this.compact = compact;

		if( compact == false)
		{
			// No gaps within component.
			setLayout( new BoxLayout( this, BoxLayout.X_AXIS));
		}
		else
		{
			// No gaps within component.
			setLayout( new BoxLayout( this, BoxLayout.Y_AXIS));
		}

		add( sourceId);
		add( volume);
	}

	public boolean isCompact()
	{
		return compact;
	}
}