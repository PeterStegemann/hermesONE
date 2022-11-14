package net.stegemann.io.xml;

import net.stegemann.configuration.type.Bool;
import net.stegemann.configuration.type.Number;
import net.stegemann.configuration.type.Text;
import org.w3c.dom.Document;
import org.w3c.dom.Node;

public class DocumentGenerator
{
	public void appendNode
	(
		Document configurationDocument,
	 	Node configurationNode,
	 	String nodeTag,
		Bool value
	)
	{
		appendNode( configurationDocument, configurationNode, nodeTag, value.getConfigurationValue());
	}

	public void appendNode
	(
		Document configurationDocument,
	 	Node configurationNode,
	 	String nodeTag,
		Text value
	)
	{
		appendNode( configurationDocument, configurationNode, nodeTag, value.getConfigurationValue());
	}

	public void appendNode
	(
		Document configurationDocument,
		Node configurationNode,
		String nodeTag,
		Number value
	)
	{
		appendNode( configurationDocument, configurationNode, nodeTag, value.getConfigurationValue());
	}

	public void appendNode
	(
		Document configurationDocument,
		Node configurationNode,
		String nodeTag,
		int value
	)
	{
		appendNode
		(
			configurationDocument,
			configurationNode,
			nodeTag,
			java.lang.Integer.toString( value)
		);
	}

	public void appendNode
	(
		Document configurationDocument,
		Node configurationNode,
		String nodeTag,
		String value
	)
	{
		Node newNode = configurationDocument.createElement( nodeTag);
		configurationNode.appendChild( newNode);
		newNode.setTextContent( value);
	}
}
