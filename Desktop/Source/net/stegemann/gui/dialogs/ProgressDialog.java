package net.stegemann.gui.dialogs;

import net.stegemann.io.serial.configuration.ConfigurationProgress;
import net.stegemann.misc.ChangeListener;

import javax.swing.*;
import javax.swing.LayoutStyle.ComponentPlacement;
import java.awt.*;
import java.io.Serial;

public class ProgressDialog extends JDialog implements ChangeListener< ConfigurationProgress>
{
	@Serial
	private static final long serialVersionUID = 108509389789556744L;

	private final JLabel typesValue = new JLabel( "0");
	private final JLabel modelsValue = new JLabel( "0");
	private final JLabel sourcesValue = new JLabel( "0");

	// This flag is set on close and is respected by open, just for the case that close is called before open. In that
	// case, open won't do anything.
	private volatile boolean gone = false;
	// We can't lock because setVisible( true) blocking, but it might happen that we close and open run at the same
	// time. That's why we keep closing until the open flag is set to false.
	private volatile boolean open = false;

	public ProgressDialog( JFrame parent, String text)
	{
		super( parent, "", true);

		setDefaultCloseOperation( JFrame.DO_NOTHING_ON_CLOSE);
		setResizable( false);

		JLabel textLabel = new JLabel( text);

		JProgressBar progressBar = new JProgressBar();
		progressBar.setIndeterminate( true);

		JSeparator valueSeparator = new JSeparator( SwingConstants.HORIZONTAL);

		JLabel typesLabel = new JLabel( "Types");
		JLabel modelsLabel = new JLabel( "Models");
		JLabel sourcesLabel = new JLabel( "Sources");

		// Layout elements.
		GroupLayout layout = new GroupLayout( getContentPane());
		setLayout( layout);

//		Layout.setAutoCreateGaps( true);
		layout.setAutoCreateContainerGaps( true);

		layout.setHorizontalGroup
		(
			layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( textLabel)
				.addComponent( progressBar)
				.addComponent( valueSeparator)
				.addGroup
				(
					layout.createSequentialGroup()
						.addGroup
						(
							layout.createParallelGroup( GroupLayout.Alignment.CENTER)
								.addComponent( typesLabel)
								.addComponent( typesValue)
						)
						.addPreferredGap( ComponentPlacement.UNRELATED)
						.addGroup
						(
							layout.createParallelGroup( GroupLayout.Alignment.CENTER)
								.addComponent( modelsLabel)
								.addComponent( modelsValue)
						)
						.addPreferredGap( ComponentPlacement.UNRELATED)
						.addGroup
						(
							layout.createParallelGroup( GroupLayout.Alignment.CENTER)
								.addComponent( sourcesLabel)
								.addComponent( sourcesValue)
						)
				)
		);

		layout.setVerticalGroup
		(
			layout.createSequentialGroup()
				.addComponent( textLabel)
				.addComponent( progressBar)
				.addComponent( valueSeparator)
				.addGroup
				(
					layout.createSequentialGroup()
						.addGroup
						(
							layout.createParallelGroup( GroupLayout.Alignment.CENTER)
								.addComponent( typesLabel)
								.addComponent( modelsLabel)
								.addComponent( sourcesLabel)
						)
						.addGroup
						(
							layout.createParallelGroup( GroupLayout.Alignment.CENTER)
								.addComponent( typesValue)
								.addComponent( modelsValue)
								.addComponent( sourcesValue)
						)
				)
		);
	}

	public void open()
	{
		open = true;

		if( gone == true)
		{
			open = false;

			return;
		}

		pack();

		if( getParent() != null)
		{
			Dimension parentSize = getParent().getSize();
			Point parentLocation = getParent().getLocation();
			Dimension thisSize = getSize();

			setLocation
			(
				parentLocation.x + parentSize.width / 2 - thisSize.width / 2,
				parentLocation.y + parentSize.height / 2 - thisSize.height / 2
			);
		}

		setVisible( true);

		open = false;
	}

	public void close()
	{
		gone = true;

		while( open == true)
		{
			setVisible( false);

			if( open == true)
			{
				try
				{
					Thread.sleep( 100);
				}
				catch (InterruptedException ignored)
				{
				}
			}
		}
	}

	@Override
	public void hasChanged( ConfigurationProgress configurationProgress)
	{
		typesValue.setText( String.valueOf( configurationProgress.getTypeCount()));
		modelsValue.setText( String.valueOf( configurationProgress.getModelCount()));
		sourcesValue.setText( String.valueOf( configurationProgress.getSourceCount()));
	}
}
