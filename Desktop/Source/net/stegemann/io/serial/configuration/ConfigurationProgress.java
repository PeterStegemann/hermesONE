package net.stegemann.io.serial.configuration;

import net.stegemann.misc.ChangeObservable;

public class ConfigurationProgress 
	extends ChangeObservable< ConfigurationProgress>
{
	private int typeCount = 0;
	private int modelCount = 0;
	private int sourceCount = 0;
	
	public int getTypeCount()
	{
		return typeCount;
	}
	
	public void setTypeCount( int typeCount)
	{
		this.typeCount = typeCount;
		
		notifyChange( this);
	}

	public int getModelCount() 
	{
		return modelCount;
	}

	public void setModelCount( int modelCount)
	{
		this.modelCount = modelCount;
	
		notifyChange( this);
	}

	public int getSourceCount() {
		return sourceCount;
	}

	public void setSourceCount( int sourceCount)
	{
		this.sourceCount = sourceCount;
	
		notifyChange( this);
	}
}
