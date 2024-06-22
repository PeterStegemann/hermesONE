package net.stegemann.io.xml;

import net.stegemann.configuration.type.ConfigurationValue;
import net.stegemann.io.DocumentException;
import net.stegemann.io.ReadException;
import net.stegemann.io.WriteException;
import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.xml.sax.SAXException;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.*;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import java.io.FileOutputStream;
import java.io.IOException;

public class DocumentGenerator
{
	public Document createDocument()
		throws DocumentException
	{
		return documentBuilder().newDocument();
	}

	public Document readDocument( String fileName)
		throws DocumentException
	{
		DocumentBuilder builder = documentBuilder();
		Document document;

		try
		{
			document = builder.parse( fileName);
		}
		catch( SAXException reason)
		{
			throw new ReadException( "Failed to parse xml document '" + fileName + "'.", reason);
		}
		catch( IOException reason)
		{
			throw new ReadException( "Failed to read xml document '" + fileName + "'.", reason);
		}

		return document;
	}

	public void storeDocument( Document document, String fileName)
		throws WriteException
	{
 	   	DOMSource domSource = new DOMSource( document);
		StreamResult result = streamResult( fileName);
		Transformer transformer = transformer();

		try
		{
			transformer.transform( domSource, result);
		}
		catch( TransformerException reason)
		{
			throw new WriteException( "Failed to transform xml.", reason);
		}
	}

	private DocumentBuilder documentBuilder()
		throws DocumentException
	{
		DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
		DocumentBuilder builder;

		try
		{
			builder = factory.newDocumentBuilder();
		}
		catch( ParserConfigurationException reason)
		{
			throw new DocumentException( "Failed to create xml document builder.", reason);
		}

		return builder;
	}

	private Transformer transformer()
		throws WriteException
	{
		TransformerFactory transformerFactory = TransformerFactory.newInstance();
		Transformer transformer;

		try
		{
			transformer = transformerFactory.newTransformer();
		}
		catch( TransformerConfigurationException reason)
		{
			throw new WriteException( "Failed to create xml tranformer.", reason);
		}

		transformer.setOutputProperty( OutputKeys.OMIT_XML_DECLARATION, "yes");
		transformer.setOutputProperty("{http://xml.apache.org/xslt}indent-amount", "2");
		transformer.setOutputProperty( OutputKeys.INDENT, "yes");

		return transformer;
	}

	private StreamResult streamResult( String fileName)
		throws WriteException
	{
		try
		{
			FileOutputStream resultFile = new FileOutputStream( fileName);

			return new StreamResult( resultFile);
		}
		catch( IOException reason)
		{
			throw new WriteException( "Failed to create file '" + fileName + "'.", reason);
		}
	}

	public void appendNode( Document document, Node node, String nodeTag, ConfigurationValue value)
	{
		appendNode( document, node, nodeTag, value.getStringValue());
	}

	public void appendNode( Document document,	Node node,	String nodeTag, int value)
	{
		appendNode( document, node, nodeTag, java.lang.Integer.toString( value));
	}

	public void appendNode( Document document,	Node node,	String nodeTag, Enum< ?> value)
	{
		appendNode( document, node, nodeTag, value.ordinal());
	}

	public void appendNode( Document document, Node node, String nodeTag, String value)
	{
		Node newNode = document.createElement( nodeTag);
		node.appendChild( newNode);
		newNode.setTextContent( value);
	}
}
