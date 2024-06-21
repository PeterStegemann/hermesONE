package net.stegemann.gui.misc;

import java.awt.event.ActionListener;
import javax.swing.*;

public abstract class hermesPanel extends JPanel implements ActionListener
{
    protected JButton button( String text)
    {
        JButton button = new JButton( text);
        button.addActionListener( this);

        return button;
    }
}
