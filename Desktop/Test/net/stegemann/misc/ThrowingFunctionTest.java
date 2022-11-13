package net.stegemann.misc;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertThrows;

class ThrowingFunctionTest
{
	static class TestException extends Exception
	{
	}

	@Test
	void apply()
	{
		ThrowingFunction< TestException> throwingFunction = () ->
		{
			throw new TestException();
		};

		assertThrows( TestException.class, throwingFunction::apply);
	}
}
