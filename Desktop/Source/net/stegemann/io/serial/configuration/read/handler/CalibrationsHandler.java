package net.stegemann.io.serial.configuration.read.handler;

import net.stegemann.configuration.Calibration;
import net.stegemann.configuration.Calibrations;
import net.stegemann.io.serial.base.DesktopProtocol;

class CalibrationsHandler extends DesktopConnectionHandler
{
    private final Calibrations calibrations;
    private int calibrationIndex = 0;

    public CalibrationsHandler( Calibrations calibrations)
    {
        this.calibrations = calibrations;
    }

    @Override
    public void complexOpened( DesktopProtocol.Id id)
    {
        switch( id)
        {
            case Calibration ->
            {
                Calibration calibration = new Calibration();

                pushHandler( new CalibrationHandler( calibration));

                calibrations.setCalibration( calibrationIndex, calibration);

                calibrationIndex++;
            }

            default -> super.complexOpened( id);
        }
    }
}
