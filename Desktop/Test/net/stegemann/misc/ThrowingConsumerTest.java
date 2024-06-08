package net.stegemann.misc;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

class ThrowingConsumerTest
{
	static class TestException extends Exception
	{
	}

	@Test
	void apply()
	{
		ThrowingConsumer< String, TestException> throwingConsumer = ( testValue) ->
		{
			assertEquals( "test", testValue);

			throw new TestException();
		};

		assertThrows( TestException.class, () -> throwingConsumer.accept( "test"));
	}
}
