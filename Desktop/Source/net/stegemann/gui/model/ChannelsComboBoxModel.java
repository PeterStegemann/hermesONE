package net.stegemann.gui.model;

import net.stegemann.configuration.Channel;
import net.stegemann.configuration.Channels;
import net.stegemann.misc.ChangeListener;

import javax.swing.*;
import java.io.Serial;

public class ChannelsComboBoxModel extends AbstractListModel< Channel>
								implements ComboBoxModel< Channel>, ChangeListener< Channels>
{
	@Serial
	private static final long serialVersionUID = 4147331814088951705L;

	private final Channels channels;
	private Object selectedItem = null;

	public ChannelsComboBoxModel( Channels useChannels)
	{
		channels = useChannels;

		useChannels.addChangeListener( this);
	}

	@Override
	public Object getSelectedItem()
	{
		return selectedItem;
	}

	@Override
	public void setSelectedItem( Object useSelectedItem)
	{
		selectedItem = useSelectedItem;
	}

	@Override
	public Channel getElementAt( int index)
	{
		return channels.getChannelFromIndex( index);
	}

	@Override
	public int getSize()
	{
		return channels.getChannelCount();
	}

	@Override
	public void hasChanged( Channels useChannels)
	{
		fireContentsChanged( this, 0, getSize() - 1);
	}
}
