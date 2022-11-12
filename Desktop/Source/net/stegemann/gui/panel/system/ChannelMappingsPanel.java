package net.stegemann.gui.panel.system;

import java.util.ArrayList;
import java.util.List;

import javax.swing.GroupLayout;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.SwingConstants;
import javax.swing.GroupLayout.Group;

import net.stegemann.configuration.ChannelMappings;
import net.stegemann.configuration.type.Number;
import net.stegemann.gui.components.ChannelIdComponent;

public class ChannelMappingsPanel extends JPanel
{
	private static final long serialVersionUID = 660474941338897581L;

	private class ChannelMappingLine
	{
		private final JLabel label;
		private final ChannelIdComponent source;

		public ChannelMappingLine()
		{
			label = new JLabel();
			label.setHorizontalAlignment( SwingConstants.LEADING);
			source = new ChannelIdComponent();
//			source = new NumberComponent( 5);
//			source.setHorizontalAlignment( SwingConstants.TRAILING);
		}

		public JLabel GetLabel()
		{
			return label;
		}

		public ChannelIdComponent GetSource()
		{
			return source;
		}
	}

	private ChannelMappings channelMappings;

	private GroupLayout layout;
	private JLabel sourceLabel;

	private List< ChannelMappingLine> channelMappingLines = new ArrayList< ChannelMappingLine>();

	public ChannelMappingsPanel( ChannelMappings UseChannelMappings)
	{
		channelMappings = UseChannelMappings;

		sourceLabel = new JLabel( "Quelle");
		sourceLabel.setHorizontalAlignment( SwingConstants.CENTER);
	}

	public void set()
	{
		channelMappingLines.clear();

		int Index = 0;

		for( Number CurrentChannelMapping: channelMappings)
		{
			ChannelMappingLine NewChannelMappingLine = new ChannelMappingLine();
			channelMappingLines.add( NewChannelMappingLine);

			NewChannelMappingLine.GetLabel().setText( "Kanal " + ( char)( Index + '1'));
			NewChannelMappingLine.GetSource().attachValue( CurrentChannelMapping);

			Index++;
		}

		JLabel CornerLabel = new JLabel();

		removeAll();

		// Layout elements.
		layout = new GroupLayout( this);
		setLayout( layout);

//		layout.setAutoCreateGaps( true);
		layout.setAutoCreateContainerGaps( true);

		Group HorizontalGroup = layout.createSequentialGroup();

		Group LabelColumnGroup =
			layout.createParallelGroup( GroupLayout.Alignment.CENTER).addComponent( CornerLabel);
		Group SourceColumnGroup =
			layout.createParallelGroup( GroupLayout.Alignment.CENTER).addComponent( sourceLabel);

		for( ChannelMappingLine CurrentChannelMappingLine: channelMappingLines)
		{
			LabelColumnGroup.addComponent( CurrentChannelMappingLine.GetLabel());
			SourceColumnGroup.addComponent( CurrentChannelMappingLine.GetSource());
		}

		HorizontalGroup.addGroup( LabelColumnGroup);
		HorizontalGroup.addGroup( SourceColumnGroup);

		layout.setHorizontalGroup( HorizontalGroup);

		Group VerticalGroup = layout.createSequentialGroup()
			.addGroup( layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( CornerLabel)
				.addComponent( sourceLabel)
			);

		for( ChannelMappingLine CurrentChannelMappingLine: channelMappingLines)
		{
			VerticalGroup.addGroup( layout.createParallelGroup( GroupLayout.Alignment.CENTER)
				.addComponent( CurrentChannelMappingLine.GetLabel())
				.addComponent( CurrentChannelMappingLine.GetSource())
			);
		}

		layout.setVerticalGroup( VerticalGroup);
	}
}
