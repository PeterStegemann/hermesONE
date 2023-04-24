package net.stegemann.io.xml;

import net.stegemann.configuration.Configuration;
import net.stegemann.configuration.type.Bool;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.Text;
import net.stegemann.configuration.util.ConfigurationField;
import net.stegemann.io.DocumentException;
import org.w3c.dom.Document;
import org.w3c.dom.Node;

import java.lang.reflect.Field;

public class XMLWriter
{
	private static final boolean debug = false;

	private final DocumentGenerator documentGenerator;

	XMLWriter( DocumentGenerator DocumentGenerator)
 	{
		 this.documentGenerator = DocumentGenerator;
	}

	public void writeToFile( Configuration Configuration, String FileName)
		throws DocumentException
	{
		Document Document = documentGenerator.createDocument();

		exportObject( Document, Document, Names.CONFIGURATION, Configuration);

		documentGenerator.storeDocument( Document, FileName);
	}

	private void exportObject( Document Document, Node ParentNode, String Name, Object Object)
	{
		Node ChildNode = addChildNode( Document, ParentNode, Name);

		exportFields( Document, ChildNode, Object);
	}

	void exportFields( Document Document, Node Node, Object Object)
	{
		exportFields( Document, Node, Object, Object.getClass());
	}

	private void exportFields( Document Document, Node Node, Object Object, Class<?> ThisClass)
	{
		Class< ?> SuperClass = ThisClass.getSuperclass();

		if( SuperClass != null)
		{
			exportFields( Document, Node, Object, SuperClass);
		}

		Field[] Fields = ThisClass.getDeclaredFields();

		for( Field Field : Fields)
		{
			exportField( Document, Node, Field, Object);
		}
	}

	private void exportField( Document Document, Node Node, Field Field, Object Object)
 	{
		Object Value = getFieldValue( Object, Field);
		ConfigurationField Annotation = getValueAnnotation( Field, Value);

		String Name = Annotation != null ? Annotation.name() : null;

		exportValue( Document, Node, Name, Annotation, Value);
	}

	private boolean exportValue( Document Document, Node Node, String Name, ConfigurationField Annotation, Object Value)
	{
		if( Annotation != null)
		{
			if( Annotation.ignore() == true)
			{
				return false;
			}

			if( Annotation.name().isBlank() == false)
			{
				Name = Annotation.name();
			}
		}

		if( Name == null)
		{
			return false;
		}

		if( !exportSimpleValue( Document, Node, Name, Value))
		{
			exportComplexValue( Document, Node, Name, Annotation, Value);
		}

		return true;
	}

	private boolean exportSimpleValue( Document Document, Node Node, String Name, Object Value)
	{
		if( Value instanceof Bool)
		{
			if( debug) java.lang.System.out.println( Name + ": " + (( Bool) Value).getValue());

			documentGenerator.appendNode( Document, Node, Name, ( Bool) Value);
		}
		else if( Value instanceof Enum)
		{
			if( debug) java.lang.System.out.println( Name + ": " + (( Enum< ?>) Value).ordinal());

			documentGenerator.appendNode( Document, Node, Name, ( Enum< ?>) Value);
		}
		else if( Value instanceof Number)
		{
			if( debug) java.lang.System.out.println( Name + ": " + (( Number) Value).getValue());

			documentGenerator.appendNode( Document, Node, Name, ( Number) Value);
		}
		else if( Value instanceof String)
		{
			if( debug) java.lang.System.out.println( Name + ": " + Value);

			documentGenerator.appendNode( Document, Node, Name, ( String) Value);
		}
		else if( Value instanceof Text)
		{
			if( debug) java.lang.System.out.println(Name + ": " + (( Text) Value).getValue());

			documentGenerator.appendNode( Document, Node, Name, ( Text) Value);
		}
		else
		{
			return false;
		}

		return true;
	}

	private void exportComplexValue( Document Document, Node Node, String Name, ConfigurationField Annotation,
									 Object Value)
	{
		if( Value instanceof Iterable)
		{
			if( debug) System.out.println( Name + ": <iterable>");

			exportIterable( Document, Node, Name, Annotation.itemName(), ( Iterable< ?>) Value);
		}
		else
		{
			if( debug) System.out.println( Name + ": <Object>");

			exportObject( Document, Node, Name, Value);
		}
	}

	private void exportIterable( Document Document, Node ParentNode, String Name, String ItemName, Iterable< ?> Value)
	{
		if( ItemName.isEmpty())
		{
			exportIterable( Document, ParentNode, Name, Value);
		}
		else
		{
			Node ChildNode = addChildNode( Document, ParentNode, Name);

			exportIterable( Document, ChildNode, ItemName, Value);
		}
	}

	void exportIterable( Document Document, Node ParentNode, String ItemName, Iterable< ?> Value)
	{
		int Id = 0;

		for( Object Object: Value)
		{
			if( exportIterableItem( Document, ParentNode, ItemName, Id, Object))
			{
				Id++;
			}
		}
	}

	private boolean exportIterableItem( Document Document, Node ParentNode, String Name, int Id, Object Object)
	{
		ConfigurationField Annotation = Object.getClass().getAnnotation( ConfigurationField.class);

		if( exportValue( Document, ParentNode, Name, Annotation, Object))
		{
			ParentNode.insertBefore( Document.createComment(Name + " " + Id), ParentNode.getLastChild());

			return true;
		}

		return false;
	}

	private static Node addChildNode( Document Document, Node ParentNode, String Name)
	{
		Node ChildNode = Document.createElement( Name);
		ParentNode.appendChild( ChildNode);

		return ChildNode;
	}

	private static ConfigurationField getValueAnnotation( Field Field, Object Value)
	{
		ConfigurationField Annotation = Value.getClass().getAnnotation( ConfigurationField.class);

		if( Annotation == null)
		{
			Annotation = Field.getAnnotation( ConfigurationField.class);
		}

		return Annotation;
	}

	private static Object getFieldValue( Object Object, Field Field)
	{
		Field.setAccessible( true);

		try
		{
			return Field.get( Object);
		}
		catch( IllegalAccessException reason)
		{
			throw new RuntimeException( reason);
		}
	}
}
