package net.stegemann.io.serial.configuration;

import lombok.Getter;
import lombok.Setter;import net.stegemann.misc.ChangeObservable;

@Getter
public class ConfigurationProgress extends ChangeObservable< ConfigurationProgress>
{
	private int modelsCount;
	private int typesCount;
	private int sourcesCount;

	private int modelsMaximum;
	private int typesMaximum;
	private int sourcesMaximum;

	ConfigurationProgress()
	{
		reset();
	}

	public void setTypesCount( int typesCount)
	{
		this.typesCount = typesCount;
		
		notifyChange( this);
	}

	public void setModelsCount( int modelsCount)
	{
		this.modelsCount = modelsCount;
	
		notifyChange( this);
	}

	public void setSourcesCount( int sourcesCount)
	{
		this.sourcesCount = sourcesCount;

		notifyChange( this);
	}

	public void setTypesMaximum( int typesMaximum)
	{
		this.typesMaximum = typesMaximum;

		notifyChange( this);
	}

	public void setModelsMaximum( int modelsMaximum)
	{
		this.modelsMaximum = modelsMaximum;

		notifyChange( this);
	}

	public void setSourcesMaximum( int sourcesMaximum)
	{
		this.sourcesMaximum = sourcesMaximum;

		notifyChange( this);
	}

    public int getTotalCount()
    {
        return modelsCount * 5 + typesCount + sourcesCount;
    }

    public int getTotalMaximum()
    {
        return modelsMaximum * 5 + typesMaximum + sourcesMaximum;
    }

    public void reset()
	{
		typesCount = 0;
		modelsCount = 0;
		sourcesCount = 0;
        modelsMaximum = 0;
        typesMaximum = 0;
	    sourcesMaximum = 0;

		notifyChange( this);
	}
}
