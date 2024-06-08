package net.stegemann.gui.panel.source;

import net.stegemann.configuration.Configuration;
import net.stegemann.configuration.Model;
import net.stegemann.configuration.ProxyReferences;
import net.stegemann.configuration.Signal;
import net.stegemann.configuration.source.Proxy;
import net.stegemann.configuration.source.Sources;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.view.SourcesView;
import net.stegemann.configuration.view.SourcesView.HasEmpty;
import net.stegemann.configuration.view.SourcesView.HasFixed;
import net.stegemann.configuration.view.SourcesView.HasProxies;
import net.stegemann.configuration.view.SourcesView.PickGlobals;
import net.stegemann.gui.components.source.SourceWithVolumeComponent;

import javax.swing.*;

public class ProxyPanel extends SpecificSourcePanel
{
	private static final long serialVersionUID = 2207731913976777397L;

	private final SourceWithVolumeComponent proxyReference;

	public ProxyPanel( Configuration UseConfiguration)
	{
		super( UseConfiguration);

		JLabel SlotLabel = new JLabel( "Quelle:");
		proxyReference = new SourceWithVolumeComponent( Signal.MINIMUM_VALUE /
											 ProxyReferences.PROXY_REFERENCE_SIGNAL_PER_VALUE,
											 Signal.MAXIMUM_VALUE /
											 ProxyReferences.PROXY_REFERENCE_SIGNAL_PER_VALUE);

		// Layout elements.
		GroupLayout Layout = new GroupLayout( this);
		setLayout( Layout);

//		Layout.setAutoCreateGaps( true);
		Layout.setAutoCreateContainerGaps( true);

		Layout.setHorizontalGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
			.addGroup( Layout.createSequentialGroup()
				.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.TRAILING)
					.addComponent( SlotLabel)
				)
				.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.LEADING)
					.addComponent( proxyReference)
				)
			)
		);

		Layout.setVerticalGroup( Layout.createSequentialGroup()
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( SlotLabel)
				.addComponent( proxyReference, javax.swing.GroupLayout.PREFERRED_SIZE,
											   javax.swing.GroupLayout.PREFERRED_SIZE,
											   javax.swing.GroupLayout.PREFERRED_SIZE)
			)
		);
	}

	public void set( Model model, Proxy source)
	{
		super.set( model, source);

		Sources sources = configuration.getSources();

		// This is a special case, the proxy source is a type source, but we still need to be able
		// to select from all levels.
		Number modelId = model.getId();
		Number typeId = model.getTypeId();

		SourcesView sourcesView =
			new SourcesView( sources, PickGlobals.Yes, typeId, modelId, HasEmpty.Yes, HasFixed.Yes,
							 HasProxies.Yes);

		ProxyReferences UseProxyReferences = model.getProxyReferences();

		proxyReference.set( sourcesView, UseProxyReferences.getProxyReferenceFromIndex(
							source.getSlot().getValue()));

		setVisible( true);
	}
}
