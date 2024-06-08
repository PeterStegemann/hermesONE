package net.stegemann.configuration;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertNotNull;

class ConfigurationTest
{
    @Test
    void testToString()
    {
        String result = new Configuration().toString();

        java.lang.System.out.println( result);

        assertNotNull( result);
    }
}
