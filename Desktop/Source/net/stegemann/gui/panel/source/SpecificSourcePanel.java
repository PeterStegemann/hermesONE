package net.stegemann.gui.panel.source;

import net.stegemann.configuration.Configuration;
import net.stegemann.configuration.Model;
import net.stegemann.configuration.source.Source;
import net.stegemann.configuration.source.Sources;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.view.SourcesView;
import net.stegemann.configuration.view.SourcesView.HasEmpty;
import net.stegemann.configuration.view.SourcesView.HasFixed;
import net.stegemann.configuration.view.SourcesView.HasProxies;
import net.stegemann.configuration.view.SourcesView.PickGlobals;

import javax.swing.*;
import java.awt.*;
import java.io.Serial;

public class SpecificSourcePanel extends JPanel
{
	@Serial
	private static final long serialVersionUID = 7581521960156885148L;

	protected final Configuration configuration;
	protected SourcesView sourcesViewWithFixed;
	protected SourcesView sourcesViewWithoutFixed;

	public SpecificSourcePanel( Configuration configuration)
	{
		this.configuration = configuration;

		setMinimumSize( new Dimension( 450, 450));
	}

	public void set( Model model, Source source)
	{
		Sources sources = configuration.getSources();

		Number typeId = null;
		Number modelId = null;

		int sourceModelId = source.getModelId().getValue();

		if(( sourceModelId >= Model.MODEL_START) && ( sourceModelId <= Model.MODEL_END))
		{
			modelId = model.getId();
			typeId = model.getTypeId();
		}
		else if(( sourceModelId >= Model.TYPE_START) && ( sourceModelId <= Model.TYPE_END))
		{
			typeId = model.getTypeId();
		}

		sourcesViewWithoutFixed =
			new SourcesView( sources, PickGlobals.Yes, typeId, modelId, HasEmpty.Yes, HasFixed.No, HasProxies.Yes);
		sourcesViewWithFixed =
			new SourcesView( sources, PickGlobals.Yes, typeId, modelId, HasEmpty.Yes, HasFixed.Yes, HasProxies.Yes);

		setVisible( true);
}

	public void unset()
	{
		setVisible( false);

		sourcesViewWithFixed = null;
		sourcesViewWithoutFixed = null;
	}
}
