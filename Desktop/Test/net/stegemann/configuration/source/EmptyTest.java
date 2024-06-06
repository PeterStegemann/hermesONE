package net.stegemann.configuration.source;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertNotNull;

class EmptyTest
{
    @Test
    void testToString()
    {
        String result = new Empty().toString();

        System.out.println( result);

        assertNotNull( result);
    }
}
