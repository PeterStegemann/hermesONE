package net.stegemann.gui.panel.source;

import javax.swing.GroupLayout;
import javax.swing.JLabel;

import net.stegemann.configuration.Configuration;
import net.stegemann.configuration.Model;
import net.stegemann.configuration.Signal;
import net.stegemann.configuration.source.Store;
import net.stegemann.gui.components.NumberSliderComponent;
import net.stegemann.gui.components.source.SourceComponent;

public class StorePanel extends SpecificSourcePanel
{
	private static final long serialVersionUID = 285574214590891920L;

	private final SourceComponent inputId;
	private final NumberSliderComponent init;

	public StorePanel( Configuration UseConfiguration)
	{
		super( UseConfiguration);

		JLabel InputLabel = new JLabel( "Quelle:");
		inputId = new SourceComponent();

		JLabel InitLabel = new JLabel( "Start:");
		init = new NumberSliderComponent( Signal.MINIMUM_VALUE / Store.INIT_SIGNAL_PER_VALUE,
										  Signal.MAXIMUM_VALUE / Store.INIT_SIGNAL_PER_VALUE);

		// Layout elements.
		GroupLayout Layout = new GroupLayout( this);
		setLayout( Layout);

		Layout.setHorizontalGroup( Layout.createSequentialGroup()
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.TRAILING)
				.addComponent( InputLabel)
				.addComponent( InitLabel)
			)
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.LEADING)
				.addComponent( inputId, javax.swing.GroupLayout.PREFERRED_SIZE,
										javax.swing.GroupLayout.PREFERRED_SIZE,
										javax.swing.GroupLayout.PREFERRED_SIZE)
				.addComponent( init)
			)
		);

		Layout.setVerticalGroup( Layout.createParallelGroup( GroupLayout.Alignment.LEADING)
			.addGroup( Layout.createSequentialGroup()
				.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
					.addComponent( InputLabel)
					.addComponent( inputId, javax.swing.GroupLayout.PREFERRED_SIZE,
											javax.swing.GroupLayout.PREFERRED_SIZE,
											javax.swing.GroupLayout.PREFERRED_SIZE)
				)
				.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
					.addComponent( InitLabel)
					.addComponent( init, javax.swing.GroupLayout.PREFERRED_SIZE,
										 javax.swing.GroupLayout.PREFERRED_SIZE,
										 javax.swing.GroupLayout.PREFERRED_SIZE)
				)
			)
		);
	}

	public void set( Model UseModel, Store UseSource)
	{
		super.set( UseModel, UseSource);

		inputId.setSourcesView( sourcesViewWithFixed);
		inputId.attachValue( UseSource.getInput());
		init.attachValue( UseSource.getInit());
	}

	@Override
	public void unset()
	{
/*		inputId.setSourcesView( sourcesViewWithFixed);
		inputId.detachValue( UseSource.getInput());
		init.detachValue();
*/
		super.unset();
	}
}
