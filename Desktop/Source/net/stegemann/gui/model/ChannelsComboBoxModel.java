package net.stegemann.gui.model;

import javax.swing.AbstractListModel;
import javax.swing.ComboBoxModel;

import net.stegemann.configuration.Channel;
import net.stegemann.configuration.Channels;
import net.stegemann.misc.ChangeListener;

public class ChannelsComboBoxModel extends AbstractListModel< Channel>
										  implements ComboBoxModel< Channel>, ChangeListener< Channels>
{
	private static final long serialVersionUID = 4147331814088951705L;

	private final Channels channels;
	private Object selectedItem = null;

	public ChannelsComboBoxModel( Channels UseChannels)
	{
		this.channels = UseChannels;

		UseChannels.addChangeListener( this);
	}

	@Override
	public Object getSelectedItem()
	{
		return selectedItem;
	}

	@Override
	public void setSelectedItem( Object SelectedItem)
	{
		this.selectedItem = SelectedItem;
	}

	@Override
	public Channel getElementAt( int Index)
	{
		return channels.getChannelFromIndex( Index);
	}

	@Override
	public int getSize()
	{
		return channels.getChannelCount();
	}

	@Override
	public void hasChanged( Channels UseChannels)
	{
		fireContentsChanged( this, 0, getSize() - 1);
	}
}
