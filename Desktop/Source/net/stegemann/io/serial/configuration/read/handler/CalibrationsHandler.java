package net.stegemann.io.serial.configuration.read.handler;

import net.stegemann.configuration.Calibration;
import net.stegemann.configuration.Calibrations;
import net.stegemann.io.serial.base.DesktopProtocol;

class CalibrationsHandler extends DesktopConnectionHandler
{
	private final Calibrations calibrations;
	private int calibrationIndex;

	public CalibrationsHandler( Calibrations calibrations)
	{
		this.calibrations = calibrations;

		calibrationIndex = 0;
	}

	@Override
	public void complexOpened( DesktopProtocol.Id id)
	{
		switch( id)
		{
			case Calibration :
			{
				Calibration NewCalibration = new Calibration();

				pushHandler( new CalibrationHandler( NewCalibration));

				calibrations.setCalibration( calibrationIndex, NewCalibration);

				calibrationIndex++;
			}
			break;

			default : super.complexOpened( id); break;
		}
	}
}
