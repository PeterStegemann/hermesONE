package net.stegemann.io.xml;

import lombok.Getter;
import net.stegemann.configuration.type.Bool;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.Text;
import net.stegemann.configuration.util.ConfigurationField;
import org.junit.jupiter.api.Test;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;

import java.util.Iterator;
import java.util.List;

import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.*;

class XMLWriterTest
{
	private final DocumentGenerator documentGenerator = mock( DocumentGenerator.class);
	private final XMLWriter xmlWriter = new XMLWriter( documentGenerator);

	@Test
	void exportIgnore()
	{
		@Getter
		class TestClass
		{
			@ConfigurationField( name = "testName", ignore = true)
			private final String testField = "testField";
		}

		Document document = mock( Document.class);
		Node node = mock( Node.class);

		TestClass testObject = new TestClass();

		xmlWriter.exportFields( document, node, testObject);

		verify( documentGenerator, times( 0))
				.appendNode( document, node, "testName", testObject.getTestField());
	}

	@Test
	void exportBool()
	{
		@Getter
		class TestClass
		{
			@ConfigurationField( name = "testName")
			private final Bool testField = new Bool();
		}

		Document document = mock( Document.class);
		Node node = mock( Node.class);

		TestClass testObject = new TestClass();

		xmlWriter.exportFields( document, node, testObject);

		verify( documentGenerator).appendNode( document, node, "testName", testObject.getTestField());
	}

	@Test
	void exportNumber()
	{
		@Getter
		class TestClass
		{
			@ConfigurationField( name = "testName")
			private final Number testField = new Number( 10);
		}

		Document document = mock( Document.class);
		Node node = mock( Node.class);

		TestClass testObject = new TestClass();

		xmlWriter.exportFields( document, node, testObject);

		verify( documentGenerator).appendNode( document, node, "testName", testObject.getTestField());
	}

	@Test
	void exportString()
	{
		@Getter
		class TestClass
		{
			@ConfigurationField( name = "testName")
			private final String testField = "testField";
		}

		Document document = mock( Document.class);
		Node node = mock( Node.class);

		TestClass testObject = new TestClass();

		xmlWriter.exportFields( document, node, testObject);

		verify( documentGenerator).appendNode( document, node, "testName", testObject.getTestField());
	}

	@Test
	void exportText()
	{
		@Getter
		class TestClass
		{
			@ConfigurationField( name = "testName")
			private final Text testField = new Text( "testField");
		}

		Document document = mock( Document.class);
		Node node = mock( Node.class);

		TestClass testObject = new TestClass();

		xmlWriter.exportFields( document, node, testObject);

		verify( documentGenerator).appendNode( document, node, "testName", testObject.getTestField());
	}

	@Test
	void exportList()
	{
		class TestIterable implements Iterable< String>
		{
			private final List< String> children = List.of( "testField");

			@Override
			public Iterator< String> iterator()
			{
				return children.iterator();
			}
		}

		@Getter
		class TestClass
		{
			@ConfigurationField( name = "testIterable", itemName = "testItem")
			private final TestIterable testIterable = new TestIterable();
		}

		TestClass testObject = new TestClass();

		Document document = mock( Document.class);
		Node node = mock( Element.class);
		Element iterableNode = mock( Element.class);
		when( document.createElement( eq( "testIterable"))).thenReturn( iterableNode);
		Element itemNode = mock( Element.class);
		when( document.createElement( eq( "testItem"))).thenReturn( itemNode);

		xmlWriter.exportFields( document, node, testObject);

		verify( document).createElement( eq( "testIterable"));
		verify( documentGenerator)
			.appendNode( eq( document), eq( iterableNode), eq("testItem"), eq( "testField"));
	}

	@Test
	void exportListIgnoreItem()
	{
		@Getter
		@ConfigurationField( ignore = true)
		class TestItem
		{
			@ConfigurationField( name = "testName")
			private final String testField = "testField";
		}

		class TestIterable implements Iterable< TestItem>
		{
			private final List< TestItem> children = List.of( new TestItem());

			@Override
			public Iterator< TestItem> iterator()
			{
				return children.iterator();
			}
		}

		TestIterable testObject = new TestIterable();

		Document document = mock( Document.class);
		Node node = mock( Element.class);

		xmlWriter.exportIterable( document, node, "testItem", testObject);

		verify( document, times( 0)).createElement( eq( "testItem"));
	}

	@Test
	void exportListClassWithName()
	{
		@Getter
		@ConfigurationField( name = "className")
		class TestItem
		{
			@ConfigurationField( name = "testName")
			private final String testField = "testField";
		}

		class TestIterable implements Iterable< TestItem>
		{
			private final List< TestItem> children = List.of( new TestItem());

			@Override
			public Iterator< TestItem> iterator()
			{
				return children.iterator();
			}
		}

		TestIterable testObject = new TestIterable();

		Document document = mock( Document.class);
		Node node = mock( Element.class);
		Element iterableNode = mock( Element.class);
		when( document.createElement( eq( "className"))).thenReturn( iterableNode);

		xmlWriter.exportIterable( document, node, "testItem", testObject);

		verify( document).createElement( eq( "className"));
		verify( documentGenerator)
				.appendNode( eq( document), eq( iterableNode), eq("testName"), eq( "testField"));
	}

	@Test
	void exportClassWithName()
	{
		@Getter
		@ConfigurationField( name = "testClass")
		class TestClass
		{
			@ConfigurationField( name = "testName")
			private final String testField = "testField";
		}

		@Getter
		class TestContainer
		{
			@ConfigurationField( name = "testField")
			private final TestClass testField = new TestClass();
		}

		Document document = mock( Document.class);
		Node node = mock( Node.class);

		TestContainer testContainer = new TestContainer();

		Element classNode = mock( Element.class);
		when( document.createElement( eq( "testClass"))).thenReturn( classNode);

		xmlWriter.exportFields( document, node, testContainer);

		verify( document).createElement( eq( "testClass"));
		verify( documentGenerator).appendNode( document, classNode, "testName",
				testContainer.getTestField().getTestField());
	}

	@Test
	void exportInherited()
	{
		@Getter
		class TestBaseClass
		{
			@ConfigurationField( name = "testBaseName")
			private final String testField = "testField";
		}

		@Getter
		class TestClass extends TestBaseClass
		{
			@ConfigurationField( name = "testName")
			private final String testField = "testField";
		}

		Document document = mock( Document.class);
		Node node = mock( Node.class);

		TestClass testObject = new TestClass();

		xmlWriter.exportFields( document, node, testObject);

		verify( documentGenerator).appendNode( document, node, "testBaseName", testObject.getTestField());
		verify( documentGenerator).appendNode( document, node, "testName", testObject.getTestField());
	}
}
