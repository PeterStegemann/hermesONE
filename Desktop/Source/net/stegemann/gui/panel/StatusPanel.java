package net.stegemann.gui.panel;

import javax.swing.GroupLayout;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSeparator;

import net.stegemann.configuration.Configuration;
import net.stegemann.configuration.Model;
import net.stegemann.configuration.source.Timer;
import net.stegemann.configuration.source.Sources;
import net.stegemann.configuration.view.SourcesView;
import net.stegemann.configuration.view.SourcesView.HasEmpty;
import net.stegemann.configuration.view.SourcesView.HasFixed;
import net.stegemann.configuration.view.SourcesView.HasProxies;
import net.stegemann.configuration.view.SourcesView.PickGlobals;
import net.stegemann.gui.components.source.SourceComponent;import java.io.Serial;

public class StatusPanel extends JPanel
{
    @Serial
    private static final long serialVersionUID = 683481639057295222L;

    private final Configuration configuration;

    private final SourceComponent topTimeId, bottomTimeId;
    private final SourceComponent leftSideId, leftBottomId, rightSideId, rightBottomId;

    public StatusPanel( Configuration configuration)
    {
        this.configuration = configuration;

        JLabel topTimeLabel = new JLabel( "Uhr Oben:");
        topTimeId = new SourceComponent();

        JLabel bottomTimeLabel = new JLabel( "Uhr Unten:");
        bottomTimeId = new SourceComponent();

        JSeparator separator = new JSeparator();

        JLabel leftSideLabel = new JLabel( "Linke Seite:");
        leftSideId = new SourceComponent();

        JLabel leftBottomLabel = new JLabel( "Links Unten:");
        leftBottomId = new SourceComponent();

        JLabel rightSideLabel = new JLabel( "Rechte Seite:");
        rightSideId = new SourceComponent();

        JLabel rightBottomLabel = new JLabel( "Rechts Unten:");
        rightBottomId = new SourceComponent();

        // Layout elements.
        GroupLayout layout = new GroupLayout( this);
        setLayout( layout);

//		Layout.setAutoCreateGaps( true);
        layout.setAutoCreateContainerGaps( true);

        layout.setHorizontalGroup
        (
            layout.createParallelGroup( GroupLayout.Alignment.CENTER)
            .addComponent( separator)
            .addGroup
            (
                layout.createSequentialGroup()
                .addGroup
                (
                    layout.createParallelGroup( GroupLayout.Alignment.TRAILING)
                    .addComponent( topTimeLabel)
                    .addComponent( bottomTimeLabel)
                    .addComponent( leftSideLabel)
                    .addComponent( leftBottomLabel)
                    .addComponent( rightSideLabel)
                    .addComponent( rightBottomLabel)
                )
                .addGroup
                (
                    layout.createParallelGroup( GroupLayout.Alignment.CENTER)
                    .addComponent( topTimeId)
                    .addComponent( bottomTimeId)
                    .addComponent( leftSideId)
                    .addComponent( leftBottomId)
                    .addComponent( rightSideId)
                    .addComponent( rightBottomId)
                )
            )
        );

        layout.setVerticalGroup
        (
            layout.createSequentialGroup()
            .addGroup
            (
                layout.createParallelGroup( GroupLayout.Alignment.CENTER)
                .addComponent( topTimeLabel)
                .addComponent( topTimeId)
            )
            .addGroup
            (
                layout.createParallelGroup( GroupLayout.Alignment.CENTER)
                .addComponent( bottomTimeLabel)
                .addComponent( bottomTimeId)
            )
            .addComponent( separator)
            .addGroup
            (
                layout.createParallelGroup( GroupLayout.Alignment.CENTER)
                .addComponent( leftSideLabel)
                .addComponent( leftSideId)
            )
            .addGroup
            (
                layout.createParallelGroup( GroupLayout.Alignment.CENTER)
                .addComponent( leftBottomLabel)
                .addComponent( leftBottomId)
            )
            .addGroup
            (
                layout.createParallelGroup( GroupLayout.Alignment.CENTER)
                .addComponent( rightSideLabel)
                .addComponent( rightSideId)
            )
            .addGroup
            (
                layout.createParallelGroup( GroupLayout.Alignment.CENTER)
                .addComponent( rightBottomLabel)
                .addComponent( rightBottomId)
            )
        );
    }

    public void Set( Model model)
    {
        Sources sources = configuration.getSources();

        SourcesView sourcesView = new SourcesView
        (
            sources, PickGlobals.Yes, model.getTypeId(), model.getId(), Timer.class,
            HasEmpty.Yes, HasFixed.No, HasProxies.No
        );

        topTimeId.setSourcesView( sourcesView);
        topTimeId.attachValue( model.getStatusTimeId( Model.StatusTime.TOP));

        bottomTimeId.setSourcesView( sourcesView);
        bottomTimeId.attachValue( model.getStatusTimeId( Model.StatusTime.BOTTOM));

        sourcesView = new SourcesView
        (
            sources, PickGlobals.Yes, model.getTypeId(), model.getId(),
            HasEmpty.Yes, HasFixed.Yes, HasProxies.Yes
        );

        leftSideId.setSourcesView( sourcesView);
        leftSideId.attachValue( model.getStatusSourceId( Model.StatusSource.LEFT_SIDE));

        leftBottomId.setSourcesView( sourcesView);
        leftBottomId.attachValue( model.getStatusSourceId( Model.StatusSource.LEFT_BOTTOM));

        rightSideId.setSourcesView( sourcesView);
        rightSideId.attachValue( model.getStatusSourceId( Model.StatusSource.RIGHT_SIDE));

        rightBottomId.setSourcesView( sourcesView);
        rightBottomId.attachValue( model.getStatusSourceId( Model.StatusSource.RIGHT_BOTTOM));
    }
}
