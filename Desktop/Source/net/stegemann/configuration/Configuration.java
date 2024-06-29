package net.stegemann.configuration;

import lombok.Builder;
import lombok.Getter;
import net.stegemann.configuration.source.Sources;
import net.stegemann.configuration.util.ConfigurationField;
import net.stegemann.io.xml.Names;

import static net.stegemann.misc.Utility.doAll;
import static net.stegemann.misc.Utility.indent;

@Getter
@Builder
public class Configuration
{
    @ConfigurationField( name = Names.SYSTEM)
    private final System system = new System();
    @ConfigurationField( name = Names.TYPES, itemName = Names.TYPE)
    private final Types types = new Types();
    @ConfigurationField( name = Names.MODELS, itemName = Names.MODEL)
    private final Models models = new Models();
    @ConfigurationField( name = Names.SOURCES, itemName = Names.SOURCE)
    private final Sources sources = new Sources();

    public Configuration()
    {
        clear();
    }

    public boolean validate()
    {
        return doAll
        (
            () -> types.validate( this),
            () -> models.validate( this),
            () -> sources.validate( this)
        );
    }

    public void clear()
    {
        clearSystem();
        clearModels();
    }

    public void clearModels()
    {
        types.clear();
        models.clear();
        sources.clear();
    }

    public void clearSystem()
    {
        system.clear();
    }

    public void fill()
    {
        system.fill();
        models.fillChannels( system.getOutputChannels().getValue());
    }

    @Override
    public String toString()
    {
        return String.format
        (
            """
            Configuration
            {
                system: %s
                types: %s
                models: %s
                sources: %s
            }
            """,
            indent( system), indent( types), indent( models), indent( sources)
        );
    }
}
