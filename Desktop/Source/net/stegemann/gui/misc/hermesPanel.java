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

    protected JButton createButton()
    {
        return button("+");
    }

    protected JButton removeButton()
    {
        return button("-");
    }

    protected JButton duplicateButton()
        {
            return button("Kopieren");
        }

    protected JButton upButton()
    {
        return button("↑");
    }

    protected JButton downButton()
    {
        return button("↓");
    }
}
