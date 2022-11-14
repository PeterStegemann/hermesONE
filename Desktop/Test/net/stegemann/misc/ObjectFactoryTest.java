package net.stegemann.misc;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;

class ObjectFactoryTest {
	private static class TestType
	{
	}

	@Test
	void test()
	{
		TestType firstResult = getSingleton();
		TestType secondResult = getSingleton();

		assertEquals( firstResult, secondResult);
	}

	private static TestType getSingleton()
	{
	    return ObjectFactory.singleton( TestType::new);
	}
}
