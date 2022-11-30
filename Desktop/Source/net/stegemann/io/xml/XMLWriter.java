package net.stegemann.io.xml;

import net.stegemann.configuration.Configuration;
import net.stegemann.configuration.type.Bool;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.Text;
import net.stegemann.configuration.util.ConfigurationField;
import net.stegemann.io.WriteException;
import org.w3c.dom.Document;
import org.w3c.dom.Node;

import java.lang.reflect.Field;

public class XMLWriter
{
	private static final boolean debug = true;

	private final DocumentGenerator documentGenerator;

	XMLWriter( DocumentGenerator documentGenerator)
 	{
		 this.documentGenerator = documentGenerator;
	}

	public void writeToFile( Configuration configuration, String fileName)
		throws WriteException
	{
		Document document = documentGenerator.createDocument();

		exportObject( document, document, Names.CONFIGURATION, configuration);

		documentGenerator.storeDocument( document, fileName);
	}

	private void exportObject( Document document, Node parentNode, String name, Object object)
	{
		Node childNode = addChildNode( document, parentNode, name);

		exportFields( document, childNode, object);
	}

	void exportFields( Document document, Node node, Object object)
	{
		exportFields( document, node, object, object.getClass());
	}

	private void exportFields( Document document, Node node, Object object, Class<?> thisClass)
	{
		Class< ?> superClass = thisClass.getSuperclass();

		if( superClass != null)
		{
			exportFields(document, node, object, superClass);
		}

		Field[] fields = thisClass.getDeclaredFields();

		for( Field field : fields)
		{
			exportField(document, node, field, object);
		}
	}

	private void exportField( Document document, Node node, Field field, Object object)
 	{
		Object value = getFieldValue( object, field);
		ConfigurationField annotation = getValueAnnotation( field, value);

		String name = annotation != null ? annotation.name() : null;

		exportValue( document, node, name, annotation, value);
	}

	private boolean exportValue( Document document, Node node, String name, ConfigurationField annotation, Object value)
	{
		if( annotation != null)
		{
			if( annotation.ignore() == true)
			{
				return false;
			}

			if( annotation.name().isBlank() == false)
			{
				name = annotation.name();
			}
		}

		if( name == null)
		{
			return false;
		}

		if( !exportSimpleValue( document, node, name, value))
		{
			exportComplexValue( document, node, name, annotation, value);
		}

		return true;
	}

	private boolean exportSimpleValue( Document document, Node node, String name, Object value)
	{
		if( value instanceof Bool)
		{
			if( debug) java.lang.System.out.println( name + ": " + (( Bool) value).getValue());

			documentGenerator.appendNode( document, node, name, ( Bool) value);
		}
		else if( value instanceof Enum)
		{
			if( debug) java.lang.System.out.println( name + ": " + (( Enum< ?>) value).ordinal());

			documentGenerator.appendNode( document, node, name, ( Enum< ?>) value);
		}
		else if( value instanceof Number)
		{
			if( debug) java.lang.System.out.println( name + ": " + (( Number) value).getValue());

			documentGenerator.appendNode( document, node, name, ( Number) value);
		}
		else if( value instanceof String)
		{
			if( debug) java.lang.System.out.println( name + ": " + value);

			documentGenerator.appendNode( document, node, name, ( String) value);
		}
		else if( value instanceof Text)
		{
			if( debug) java.lang.System.out.println(name + ": " + (( Text) value).getValue());

			documentGenerator.appendNode( document, node, name, ( Text) value);
		}
		else
		{
			return false;
		}

		return true;
	}

	private void exportComplexValue( Document document, Node node, String name, ConfigurationField annotation,
									 Object value)
	{
		if( value instanceof Iterable)
		{
			if( debug) System.out.println( name + ": <iterable>");

			exportIterable( document, node, name, annotation.itemName(), ( Iterable< ?>) value);
		}
		else
		{
			if( debug) System.out.println( name + ": <object>");

			exportObject( document, node, name, value);
		}
	}

	private void exportIterable( Document document, Node parentNode, String name, String itemName, Iterable< ?> value)
	{
		if( itemName.isEmpty())
		{
			exportIterable( document, parentNode, name, value);
		}
		else
		{
			Node childNode = addChildNode( document, parentNode, name);

			exportIterable( document, childNode, itemName, value);
		}
	}

	void exportIterable( Document document, Node parentNode, String itemName, Iterable< ?> value)
	{
		int id = 0;

		for( Object object: value)
		{
			if( exportIterableItem( document, parentNode, itemName, id, object))
			{
				id++;
			}
		}
	}

	private boolean exportIterableItem( Document document, Node parentNode, String name, int id, Object object)
	{
		ConfigurationField annotation = object.getClass().getAnnotation( ConfigurationField.class);

		if( exportValue( document, parentNode, name, annotation, object))
		{
			parentNode.insertBefore( document.createComment(name + " " + id), parentNode.getLastChild());

			return true;
		}

		return false;
	}

	private static Node addChildNode( Document document, Node parentNode, String name)
	{
		Node childNode = document.createElement( name);
		parentNode.appendChild( childNode);

		return childNode;
	}

	private static ConfigurationField getValueAnnotation( Field field, Object value)
	{
		ConfigurationField annotation = value.getClass().getAnnotation( ConfigurationField.class);

		if( annotation == null)
		{
			annotation = field.getAnnotation( ConfigurationField.class);
		}

		return annotation;
	}

	private static Object getFieldValue( Object object, Field field)
	{
		field.setAccessible( true);

		try
		{
			return field.get( object);
		}
		catch( IllegalAccessException reason)
		{
			throw new RuntimeException( reason);
		}
	}
}
