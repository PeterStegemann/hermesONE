package net.stegemann.gui.panel;

import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.GroupLayout;
import javax.swing.JButton;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.ListSelectionModel;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;

import net.stegemann.configuration.Channel;
import net.stegemann.configuration.Channels;
import net.stegemann.configuration.Configuration;
import net.stegemann.configuration.Model;
import net.stegemann.gui.model.ChannelListCellRenderer;
import net.stegemann.gui.model.ChannelsComboBoxModel;

public class ChannelsPanel extends JPanel implements ActionListener, ListSelectionListener
{
	private static final long serialVersionUID = 6397934418258601251L;

	private final Configuration configuration;
	private Channels channels;
	private Model model;

	private final JList< Channel> channelsList;
	private final JButton addButton;
	private final JButton removeButton;

	private final ChannelPanel channelPanel;

	@Override
	public void valueChanged( ListSelectionEvent e)
	{
		if( e.getValueIsAdjusting() == false)
		{
			channelPanel.set( model, channels.getChannelFromIndex( channelsList.getSelectedIndex()));
		}
	}

	@Override
	public void actionPerformed( ActionEvent e)
	{
		if( e.getSource() == addButton)
		{
			Channel NewChannel = channels.addChannel(
				new Channel( "Kanal " + channels.getChannelCount()));

			channelsList.setSelectedIndex( channels.getChannelIndexFromChannel( NewChannel));

			channelPanel.set( model, channels.getChannelFromIndex( channelsList.getSelectedIndex()));
		}
		else if( e.getSource() == removeButton)
		{
/*			int SelectedChannelIndex = channelsList.getSelectedIndex();

			channels.RemoveChannel( SelectedChannelIndex);

			channelPanel.Set( model, channels.getChannelFromIndex( channelsList.getSelectedIndex()));
*/		}
	}

	public ChannelsPanel( Configuration UseConfiguration)
	{
		configuration = UseConfiguration;

		channelsList = new JList<>();
		channelsList.setSelectionMode( ListSelectionModel.SINGLE_SELECTION);
		channelsList.setLayoutOrientation( JList.VERTICAL);
		channelsList.addListSelectionListener( this);
		channelsList.setCellRenderer( new ChannelListCellRenderer());

		JScrollPane ChannelsScrollPane = new JScrollPane( channelsList);
		ChannelsScrollPane.setMinimumSize( new Dimension( 150, 150));
		ChannelsScrollPane.setVerticalScrollBarPolicy( JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);

		addButton = new JButton( "+");
		addButton.addActionListener( this);
		removeButton = new JButton( "-");
		removeButton.addActionListener( this);

		channelPanel = new ChannelPanel( configuration);

		// Layout elements.
		GroupLayout Layout = new GroupLayout( this);
		setLayout( Layout);

		Layout.setHonorsVisibility( false);
//		Layout.setAutoCreateGaps( true);
		Layout.setAutoCreateContainerGaps( true);

		Layout.setHorizontalGroup( Layout.createSequentialGroup()
			.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( ChannelsScrollPane, javax.swing.GroupLayout.PREFERRED_SIZE, 150,
							   Integer.MAX_VALUE)
				.addGroup( Layout.createSequentialGroup()
						.addComponent( addButton)
						.addComponent( removeButton)
				)
			)
			.addComponent( channelPanel)
		);

		Layout.setVerticalGroup( Layout.createParallelGroup( GroupLayout.Alignment.LEADING)
			.addGroup( Layout.createSequentialGroup()
				.addComponent( ChannelsScrollPane)
				.addGroup( Layout.createParallelGroup( GroupLayout.Alignment.LEADING)
						.addComponent( addButton)
						.addComponent( removeButton)
					)
			)
			.addComponent( channelPanel)
		);
	}

	public void set( Model model)
	{
		this.model = model;

		channels = model.getChannels();

		channelsList.setModel( new ChannelsComboBoxModel( channels));
		channelPanel.set( model, channels.getChannelFromIndex( channelsList.getSelectedIndex()));
	}
}
