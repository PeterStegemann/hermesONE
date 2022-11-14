package net.stegemann.io.serial.configuration;

import lombok.Getter;
import net.stegemann.misc.ChangeObservable;

@Getter
public class ConfigurationProgress extends ChangeObservable< ConfigurationProgress>
{
	private int typeCount = 0;
	private int modelCount = 0;
	private int sourceCount = 0;

	ConfigurationProgress()	{}

	public void setTypeCount( int typeCount)
	{
		this.typeCount = typeCount;
		
		notifyChange( this);
	}

	public void setModelCount( int modelCount)
	{
		this.modelCount = modelCount;
	
		notifyChange( this);
	}

	public void setSourceCount( int sourceCount)
	{
		this.sourceCount = sourceCount;
	
		notifyChange( this);
	}
}
