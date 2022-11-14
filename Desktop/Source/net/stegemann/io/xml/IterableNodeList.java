package net.stegemann.io.xml;

import java.util.Iterator;
import java.util.NoSuchElementException;

import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

public class IterableNodeList implements Iterable< Node>
{
	private final NodeList nodeList;

	public IterableNodeList( NodeList useNodeList)
	{
		nodeList = useNodeList;
	}

	@Override
	public Iterator< Node> iterator()
	{
		return new Iterator<>()
		{
			private int nodeIndex = 0;

			@Override
			public boolean hasNext()
			{
				return getNode() != null;
			}

			@Override
			public Node next()
			{
				Node node = getNode();

				if( node == null)
				{
					throw new NoSuchElementException();
				}

				nodeIndex++;

				return node;
			}

			/**
			 * Return current node if it is an element or search for next one.
			 *
			 * @return Node
			 */
			private Node getNode()
			{
				Node node;

				while(( node = nodeList.item( nodeIndex )) != null)
				{
					if( node.getNodeType() == Node.ELEMENT_NODE)
					{
						return node;
					}
					else
					{
						nodeIndex++;
					}
				}

				return null;
			}
		};
	}
}
