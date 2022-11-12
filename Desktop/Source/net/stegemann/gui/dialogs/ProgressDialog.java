package net.stegemann.gui.dialogs;

import java.awt.Dimension;
import java.awt.Point;

import javax.swing.GroupLayout;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JProgressBar;
import javax.swing.JSeparator;
import javax.swing.SwingConstants;
import javax.swing.LayoutStyle.ComponentPlacement;

import net.stegemann.io.serial.configuration.ConfigurationProgress;
import net.stegemann.misc.ChangeListener;

public class ProgressDialog 
	extends JDialog
	implements ChangeListener<ConfigurationProgress>
{
	private static final long serialVersionUID = 108509389789556744L;

	private final JLabel typesLabel = new JLabel( "0");
	private final JLabel modelsLabel = new JLabel( "0");
	private final JLabel sourcesLabel = new JLabel( "0");

	private boolean gone = false;

	public ProgressDialog( JFrame parent, String text)
	{
		super( parent, "", true);

		setDefaultCloseOperation( JFrame.DO_NOTHING_ON_CLOSE);
		setResizable( false);

		JLabel TextLabel = new JLabel( text);

		JProgressBar ProgressBar = new JProgressBar();
		ProgressBar.setIndeterminate( true);

		JSeparator ValueSeparator = new JSeparator( SwingConstants.HORIZONTAL);

		JLabel TypesLabel = new JLabel( "Types");
		JLabel ModelsLabel = new JLabel( "Models");
		JLabel SourcesLabel = new JLabel( "Sources");

		// Layout elements.
		GroupLayout Layout = new GroupLayout( getContentPane());
		setLayout( Layout);

//		Layout.setAutoCreateGaps( true);
		Layout.setAutoCreateContainerGaps( true);

		Layout.setHorizontalGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
			.addComponent( TextLabel)
			.addComponent( ProgressBar)
			.addComponent( ValueSeparator)
			.addGroup( Layout.createSequentialGroup()
				.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
					.addComponent( TypesLabel)
					.addComponent( typesLabel)
				)
				.addPreferredGap(ComponentPlacement.UNRELATED)
				.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
					.addComponent( ModelsLabel)
					.addComponent( modelsLabel)
				)
				.addPreferredGap(ComponentPlacement.UNRELATED)
				.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
					.addComponent( SourcesLabel)
					.addComponent( sourcesLabel)
				)
			)
		);

		Layout.setVerticalGroup( Layout.createSequentialGroup()
			.addComponent( TextLabel)
			.addComponent( ProgressBar)
			.addComponent( ValueSeparator)
			.addGroup( Layout.createSequentialGroup()
				.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
					.addComponent( TypesLabel)
					.addComponent( ModelsLabel)
					.addComponent( SourcesLabel)
				)
				.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
					.addComponent( typesLabel)
					.addComponent( modelsLabel)
					.addComponent( sourcesLabel)
				)
			)
		);
	}

	public void open()
	{
	    pack(); 

		if( getParent() != null)
		{
			Dimension parentSize = getParent().getSize();
			Point parentLocation = getParent().getLocation();
			Dimension thisSize = getSize();

			setLocation( parentLocation.x + parentSize.width / 2 - thisSize.width / 2,
					     parentLocation.y + parentSize.height / 2 - thisSize.height / 2);
		}

		if( gone == false)
		{
			setVisible( true);
		}
	}

	public void close()
	{
		gone = true;

	    setVisible( false);
	}

	@Override
	public void hasChanged(ConfigurationProgress configurationProgress) {
		typesLabel.setText(String.valueOf(configurationProgress.getTypeCount()));
		modelsLabel.setText(String.valueOf(configurationProgress.getModelCount()));
		sourcesLabel.setText(String.valueOf(configurationProgress.getSourceCount()));
	}
}
