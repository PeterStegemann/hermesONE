package net.stegemann.io.serial.configuration.read.handler;

import net.stegemann.configuration.Calibration;
import net.stegemann.io.serial.base.DesktopProtocol;

class CalibrationHandler extends DesktopConnectionHandler
{
	private final Calibration calibration;

	public CalibrationHandler( Calibration calibration)
	{
		this.calibration = calibration;
	}

	@Override
	public void valueRead( DesktopProtocol.Id id, String textContent)
	{
        switch( id)
        {
            case CalibrationHigh -> readValue( calibration.getHigh(), textContent);
            case CalibrationCenter -> readValue( calibration.getCenter(), textContent);
            case CalibrationLow -> readValue( calibration.getLow(), textContent);

            default -> super.valueRead( id, textContent);
        }
	}
}
