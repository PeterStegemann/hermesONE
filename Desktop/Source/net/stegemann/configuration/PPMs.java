package net.stegemann.configuration;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

public class PPMs implements Iterable< PPM>
{
	private final List< PPM> ppms = new ArrayList<>();

	@Override
	public String toString()
	{
		StringBuffer Buffer = new StringBuffer();

		Buffer.append( "PPMs = {\n");

		for( PPM PPM: ppms)
		{
			Buffer.append( PPM);
		}

		Buffer.append( "}\n");

		return Buffer.toString();
	}

	@Override
	public Iterator< PPM> iterator()
	{
		return ppms.iterator();
	}

	public void clear( int outputs, int outputChannels)
	{
		ppms.clear();

		fill( outputs, outputChannels);
	}

	public void fill( int outputs, int outputChannels)
	{
		if( outputs <= ppms.size())
		{
			return;
		}

		// Fill up missing ppms.
		int fill = outputs - ppms.size();

		while( fill > 0)
		{
			ppms.add( new PPM( "Modul " + ( ppms.size() + 1), outputChannels));

			fill--;
		}
	}

	public PPM getPPM( int index)
	{
		if( index == -1)
		{
			return null;
		}

		return ppms.get( index);
	}

	public int getPPMCount()
	{
		return ppms.size();
	}
}
