package net.stegemann.gui.components;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Insets;
import java.awt.RenderingHints;
import java.awt.Stroke;
import java.awt.geom.Line2D;
import java.awt.geom.Path2D;

import javax.swing.JComponent;
import javax.swing.border.BevelBorder;
import javax.swing.border.CompoundBorder;

import net.stegemann.configuration.Channel;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.Volume;
import net.stegemann.misc.ChangeListener;

public class GraphComponent extends JComponent implements ChangeListener< Number>
{
	private static final long serialVersionUID = -4858108908914042294L;

	private Volume points[];
	private Number mode;

	public GraphComponent()
	{
		setPreferredSize( new Dimension( 300, 300));

		setBorder( new CompoundBorder( new BevelBorder( BevelBorder.LOWERED),
									   new BevelBorder( BevelBorder.RAISED)));
	}

	public void setPoints( Volume points[])
	{
		// Remove listeners from old points
		if( this.points != null)
		{
			for( Volume Point: this.points)
			{
				Point.removeChangeListener( this);
			}
		}

		this.points = points;

		// Add listeners to new points
		for( Volume Point: points)
		{
			Point.addChangeListener( this);
		}

		repaint();
	}

	public void setMode( Number mode)
	{
		if( mode != null)
		{
			mode.removeChangeListener( this);
		}

		this.mode = mode;

		if( mode != null)
		{
			mode.addChangeListener( this);
		}

		repaint();
	}

	@Override
	public void hasChanged( Number object)
	{
		repaint();
	}

	private static double border = 1.0;

	@Override
	public void paintComponent( Graphics graphics)
	{
		super.paintComponent( graphics); 

		// Dynamically calculate size information
		Dimension Size = getBounds().getSize();
		Insets Insets = getInsets();
		Size.width -= Insets.left + Insets.right;
		Size.height -= Insets.top + Insets.bottom;

		Graphics2D UseGraphics2D =
			( Graphics2D) graphics.create( Insets.left, Insets.top, Size.width, Size.height);

		UseGraphics2D.setRenderingHint( RenderingHints.KEY_ANTIALIASING,
										RenderingHints.VALUE_ANTIALIAS_ON);

//		int RectSize = ( Size.width > Size.height) ? Size.height : Size.width;

		double Width = Size.width - 2 * border - 1;
		double Height = Size.height - 2 * border - 1;

		// Save stroke to re set it later.
		Stroke SavedStroke = UseGraphics2D.getStroke();

		// Center line.
		UseGraphics2D.setColor( Color.GRAY);
		UseGraphics2D.draw( new Line2D.Double( border, Height / 2.0, Width + border, Height / 2.0));

		// Stroke for helping lines.
		float[] HelpDashPattern = { 5.0f, 10.0f};
		UseGraphics2D.setStroke( new BasicStroke( 1, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND,
												  10.0f, HelpDashPattern, 0.0f));

		// 25% and 75% helping lines.
		UseGraphics2D.draw( new Line2D.Double( 0, Height / 4.0, Width + border, Height / 4.0));
		UseGraphics2D.draw( new Line2D.Double( 0, Height / 4.0 * 3.0, Width + border,
											   Height / 4.0 * 3.0));

		// Stroke for point lines.
		float[] PointDashPattern = { 2.0f, 4.0f};
		UseGraphics2D.setStroke( new BasicStroke( 1, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND,
												  10.0f, PointDashPattern, 0.0f));

		// Path for the graph.
		Path2D.Double Path = new Path2D.Double();

		if(( mode == null) || ( mode.getValue() == Channel.MODE_WARP))
		{
			int Points = points.length;

			double SegmentWidth = Width / ( Points - 1);

			for( int CurrentPoint = 0; CurrentPoint < Points; CurrentPoint++)
			{
				double XPoint = CurrentPoint * SegmentWidth + border;
				double YPoint = Height -
								((( points[ CurrentPoint].getValue() + 100) / 200.0) * Height) +
								border;

				// Vertical line indicating point position.
				if(( CurrentPoint != 0) && ( CurrentPoint != ( Points - 1)))
				{
					UseGraphics2D.draw( new Line2D.Double( XPoint, border, XPoint,
														   Height + border));
				}

				// Graph line.
				if( CurrentPoint == 0)
				{
					Path.moveTo( XPoint, YPoint);
				}
				else
				{
					Path.lineTo( XPoint, YPoint);
				}
			}
		}
		else if( mode.getValue() == Channel.MODE_CLIP)
		{
			// Center line.
			UseGraphics2D.draw( new Line2D.Double( Width / 2.0 + border, border,
												   Width / 2.0 + border, Height + border));

			int Points = points.length;

			// This only works well with 3 points, which is what this was made for.
			int Low = points[ 0].getValue();
			int Center = points[( Points - 1) / 2].getValue();
			int High = points[ Points - 1].getValue();

			// Low _must_ be below high.
			if( Low > High)
			{
				int Temp = Low;
				Low = High;
				High = Temp;
			}

			int LowX = Low - Center;
			int HighX = High - Center;

			// Draw.
			drawLine( -100, Low, LowX, Low, Width, Path);
			drawLine( LowX, Low, HighX, High, Width, Path);
			drawLine( HighX, High, 100, High, Width, Path);
		}

		UseGraphics2D.setStroke( new BasicStroke( 3, BasicStroke.CAP_ROUND,
													 BasicStroke.JOIN_ROUND));
		UseGraphics2D.setColor( Color.BLUE);
		UseGraphics2D.draw( Path);

		UseGraphics2D.setStroke( SavedStroke);
	}

	private void drawLine( double LowX, double LowY, double HighX, double HighY, double Size,
						   Path2D.Double Path)
	{
		if(( HighX <= -100) || ( LowX >= 100))
		{
			// Not visible at all.
			return;
		}
/*
		if( LowX <= -100)
		{
			if( LowY != HighY)
			{
				LowY -= LowX;
			}
			LowX = -100;
		}

		if( HighX >= 100)
		{
			double Clip = HighX - 100;
			if( LowY != HighY)
			{
				HighY -= Clip;
			}
			HighX = 100;
		}
*/
		// Map to frame.
		LowX = ((( LowX + 100) / 200.0) * Size) + border;
		LowY = Size - ((( LowY + 100) / 200.0) * Size) + border;

		HighX = ((( HighX + 100) / 200.0) * Size) + border;
		HighY = Size - ((( HighY + 100) / 200.0) * Size) + border;

		// Draw.
		Path.moveTo( LowX, LowY);
		Path.lineTo( HighX, HighY);
	}

	@SuppressWarnings( "deprecation")
	@Override
	/**
	 * We use this to make the component square.
	 */
	public void reshape( int x, int y, int width, int height)
	{
		int currentWidth = getWidth();
		int currentHeight = getHeight();

		// Only reshape if the values have changed.
		if(( currentWidth != width) || ( currentHeight != height))
		{
			int size = Math.min( width, height);

			super.reshape( x, y, size, size);
		}
	}
}
