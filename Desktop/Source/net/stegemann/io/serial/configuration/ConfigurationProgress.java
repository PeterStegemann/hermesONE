package net.stegemann.io.serial.configuration;

import lombok.Getter;
import net.stegemann.misc.ChangeObservable;

@Getter
public class ConfigurationProgress extends ChangeObservable< ConfigurationProgress>
{
	private int typeCount;
	private int modelCount;
	private int sourceCount;

	ConfigurationProgress()
	{
		reset();
	}

	public void setTypeCount( int useTypeCount)
	{
		typeCount = useTypeCount;
		
		notifyChange( this);
	}

	public void setModelCount( int useModelCount)
	{
		modelCount = useModelCount;
	
		notifyChange( this);
	}

	public void setSourceCount( int useSourceCount)
	{
		sourceCount = useSourceCount;
	
		notifyChange( this);
	}

	public void reset() {
		typeCount = 0;
		modelCount = 0;
		sourceCount = 0;
	}
}
