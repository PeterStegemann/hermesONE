package net.stegemann.configuration;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

public class Calibrations implements Iterable< Calibration>
{
	private final List< Calibration> calibrations = new ArrayList< Calibration>();

	@Override
	public String toString()
	{
		StringBuffer Buffer = new StringBuffer();

		Buffer.append( "Calibrations = {\n");

		for( Calibration Calibration: calibrations)
		{
			Buffer.append( Calibration);
		}

		Buffer.append( "}\n");

		return Buffer.toString();
	}

	public void clear( int analogInputs)
	{
		calibrations.clear();

		fill( analogInputs);
	}

	public void fill( int analogInputs)
	{
		if( analogInputs <= calibrations.size())
		{
			return;
		}

		// Fill up missing calibrations.
		int Fill = analogInputs - calibrations.size();

		while( Fill > 0)
		{
			calibrations.add( new Calibration());

			Fill--;
		}
	}

	public void addCalibration( Calibration calibration)
	{
		calibrations.add( calibration);
	}

	public void setCalibration( int index, Calibration calibration)
	{
		fill( index + 1);

		calibrations.set( index, calibration);
	}

	public Calibration getCalibration( int index)
	{
		if( index == -1)
		{
			return null;
		}

		return calibrations.get( index);
	}

	public int getCalibrationCount()
	{
		return calibrations.size();
	}

	private class CalibrationsIterator implements Iterator< Calibration>
	{
		private Iterator< Calibration> iterator;

		public CalibrationsIterator( Calibrations calibrations)
		{
			iterator = calibrations.calibrations.iterator();
		}

		@Override
		public boolean hasNext()
		{
			return iterator.hasNext();
		}

		@Override
		public Calibration next()
		{
			return iterator.next();
		}

		@Override
		public void remove()
		{
			iterator.remove();
		}
	}

	@Override
	public Iterator< Calibration> iterator()
	{
		return new CalibrationsIterator( this);
	}
}
