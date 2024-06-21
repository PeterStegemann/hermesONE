package net.stegemann.gui.panel;

import net.stegemann.configuration.Channel;
import net.stegemann.configuration.Channels;
import net.stegemann.configuration.Configuration;
import net.stegemann.configuration.Model;
import net.stegemann.gui.model.ChannelListCellRenderer;
import net.stegemann.gui.model.ChannelsComboBoxModel;

import javax.swing.*;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.Serial;

public class ChannelsPanel extends JPanel implements ActionListener, ListSelectionListener
{
	@Serial
	private static final long serialVersionUID = 6397934418258601251L;

	private Channels channels;
	private Model model;

	private final JList< Channel> channelsList;

	private final ChannelPanel channelPanel;

	@Override
	public void valueChanged( ListSelectionEvent event)
	{
		if( event.getValueIsAdjusting() == false)
		{
			channelPanel.set( model, channels.getChannelFromIndex( channelsList.getSelectedIndex()));
		}
	}

	@Override
	public void actionPerformed( ActionEvent event)
	{
	}

	public ChannelsPanel( Configuration configuration)
	{
		channelsList = new JList<>();
		channelsList.setSelectionMode( ListSelectionModel.SINGLE_SELECTION);
		channelsList.setLayoutOrientation( JList.VERTICAL);
		channelsList.addListSelectionListener( this);
		channelsList.setCellRenderer( new ChannelListCellRenderer());

		JScrollPane channelsScrollPane = new JScrollPane( channelsList);
		channelsScrollPane.setMinimumSize( new Dimension( 150, 150));
		channelsScrollPane.setVerticalScrollBarPolicy( JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);

		channelPanel = new ChannelPanel( configuration);

		// Layout elements.
		GroupLayout layout = new GroupLayout( this);
		setLayout( layout);

		layout.setHonorsVisibility( false);
//		Layout.setAutoCreateGaps( true);
		layout.setAutoCreateContainerGaps( true);

		layout.setHorizontalGroup
		(
			layout.createSequentialGroup().addGroup
			(
				layout.createParallelGroup( GroupLayout.Alignment.CENTER)
                .addComponent
                (
                    channelsScrollPane, javax.swing.GroupLayout.PREFERRED_SIZE, 150, Integer.MAX_VALUE
                )
            )
			.addComponent( channelPanel)
		);

		layout.setVerticalGroup
		(
			layout.createParallelGroup( GroupLayout.Alignment.LEADING)
            .addGroup
            (
                layout.createSequentialGroup().addComponent( channelsScrollPane)
            )
            .addComponent( channelPanel)
		);
	}

	public void set( Model model)
	{
		this.model = model;
		channels = this.model.getChannels();

		channelsList.setModel( new ChannelsComboBoxModel( channels));
		channelPanel.set( this.model, channels.getChannelFromIndex( channelsList.getSelectedIndex()));
	}
}
