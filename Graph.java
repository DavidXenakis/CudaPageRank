import java.util.*;
import java.io.*;

public class Graph {
	public HashMap<String, Node> nodes;
	
	public class Node {
		HashMap<String, Node> Edges;
		String element;
		int value;
		
		public Node(String element, int value) {
			this.element = element;
			this.value = value;
			Edges = new HashMap<String, Node>();
		}
		
	   public String toString() {
		   return element;
	   }
	   
	   //Something goes wrong with this hashCode when a Node is used for the HashMap
	   public int hashCode() {
		   return element.hashCode();
	   }
	   
	   public void addEdge(Node newNode) {
		   Edges.put(newNode.toString(), newNode);
	   }
	   
	   public void printEdges() {
		   ArrayList<Node> printNodes = new ArrayList<Node>(Edges.values());
		   System.out.print(element + "'s " + "nodes: ");
		   for (Node node : printNodes) {
			   System.out.print(node + ", ");
		   }
		   System.out.println();
	   }
	}
	
	
	//Currently just call the constructor with the file, only works with csv 
	@SuppressWarnings("resource")
	public Graph(String fileName) {
		nodes = new HashMap<String, Node>();
		
		File file = null;
		String fileType = fileName.substring(fileName.length() - 3, fileName.length());
		Scanner lineScan = null;
		Scanner stringScan = null;
		String nodeEle = null;
		int nodeVal = 0; 
		Node fromNode;
		Node toNode;
		
		
		if(fileType.compareTo("csv") == 0){
			file = new File(fileName);
		}
		else {
			System.out.println("File type not supported yet");
			return;
		}
			
		try {
			lineScan = new Scanner(file); 
	    }
	    catch(Exception e) {
	    	System.err.println(e.getMessage());
	    	return;
	    }
		
		lineScan.useDelimiter("\n");
		while(lineScan.hasNext()) {
			try {
				stringScan = new Scanner(lineScan.next()); 
		    }
		    catch(Exception e) {
		    	System.err.println(e.getMessage());
		    	return;
		    }
			
			stringScan.useDelimiter(",\\s");
			
			nodeEle = stringScan.next();
			if(nodeEle.charAt(0) == '"')
				nodeEle = nodeEle.substring(1, nodeEle.length() - 1);
			nodeVal = stringScan.nextInt();
			
			if(nodes.containsKey(nodeEle))
				fromNode = nodes.get(nodeEle);
			else
				fromNode = new Node(nodeEle, nodeVal);
			
			
			nodeEle = stringScan.next();
			if(nodeEle.charAt(0) == '"')
				nodeEle = nodeEle.substring(1, nodeEle.length() - 1);
			nodeVal = stringScan.nextInt();
			
			if(nodes.containsKey(nodeEle))
				toNode = nodes.get(nodeEle);
			else
				toNode = new Node(nodeEle, nodeVal);
			
			//just getting rid of the 5th parm for now
			if(stringScan.hasNext()) 
				stringScan.next();
			
			fromNode.addEdge(toNode);
			if(!nodes.containsKey(fromNode.toString())) {
				nodes.put(fromNode.toString(), fromNode);
			}
			
		}
		
//		ArrayList<Node> printNodes = new ArrayList<Node>(nodes.values());
//	    for (Node node : printNodes) {
//	    	node.printEdges();
//	    }

		
		lineScan.close();
		stringScan.close();
	}
	
	
	
}
