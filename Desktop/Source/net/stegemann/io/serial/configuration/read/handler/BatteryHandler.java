package net.stegemann.io.serial.configuration.read.handler;

import net.stegemann.configuration.Battery;
import net.stegemann.io.serial.base.DesktopProtocol;

class BatteryHandler extends DesktopConnectionHandler
{
	private final Battery battery;

	public BatteryHandler( Battery battery)
	{
		this.battery = battery;
	}

	@Override
	public void valueRead( DesktopProtocol.Id id, String textContent)
	{
        switch( id)
        {
            case BatteryWarnLowVoltage ->
            {
                readValue( battery.getWarnLowVoltage(), textContent);
            }

            case BatteryWarnCriticalVoltage ->
            {
                readValue( battery.getWarnCriticalVoltage(), textContent);
            }

            case BatteryMinimumVoltage ->
            {
                readValue( battery.getMinimumVoltage(), textContent);
            }

            case BatteryMaximumVoltage ->
            {
                readValue( battery.getMaximumVoltage(), textContent);
            }

            case BatteryCalibrationValue ->
            {
                readValue( battery.getCalibrationVoltage(), textContent);
            }

            default -> super.valueRead( id, textContent);
        }
	}
}
