import java.util.*;
import java.io.*;

public class Graph {
	public HashMap<String, Node> nodes;
	public boolean undirected;
	
	public class Node {
		HashMap<String, Node> Edges;
		String element;
		int value = 0;
		
		public Node(String element, int value) {
			this.element = element;
			this.value = value;
			Edges = new HashMap<String, Node>();
		}
		
		public Node(String element) {
			this.element = element;
			Edges = new HashMap<String, Node>();
		}
		
	   public String toString() {
		   return element;
	   }
	   
	   //Something goes wrong with this hashCode when a Node is used for the HashMap
	   public int hashCode() {
		   return element.hashCode();
	   }
	   
	   //Need this if you override hashCode
	   public boolean equals(Object obj) {
		   if(obj instanceof Node) {
			   Node obj1 = (Node) obj;
			   return obj1.element == this.element;
		   } else {
			   return false;
		   }
		   
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
	public Graph(String fileName, boolean undirected) {
		nodes = new HashMap<String, Node>();
		this.undirected = undirected;
		String fileType = fileName.substring(fileName.length() - 3, fileName.length());
		
		if(fileType.compareTo("csv") == 0){
			scanCSV(new File(fileName));
		}
		else if(fileType.compareTo("txt") == 0) {
			scanSNAP(new File(fileName));
		}
		else {
			System.out.println("File type not supported yet");
			return;
		}
	}
	
	void scanCSV(File file) {
		Scanner lineScan = null;
		Scanner stringScan = null;
		String nodeEle = null;
		int nodeVal = 0; 
		Node fromNode;
		Node toNode;
		
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
			
			stringScan.useDelimiter("[,\\s]+");
			
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
			if(undirected)
				toNode.addEdge(fromNode);
			
			if(!nodes.containsKey(fromNode.toString())) {
				nodes.put(fromNode.toString(), fromNode);
			}
			
			if(undirected && !nodes.containsKey(toNode.toString())) {
				nodes.put(toNode.toString(), toNode);
			}
			
		}

		
		lineScan.close();
		stringScan.close();
	}

	void scanSNAP(File file) {
		Scanner lineScan = null;
		Scanner stringScan = null;
		String nodeEle = null;
		int nodeVal = 0; 
		Node fromNode;
		Node toNode;
		
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
			
			stringScan.useDelimiter("[\\s]+");
			nodeEle = stringScan.next();
			if(nodeEle.charAt(0) == '#')
				continue;
			
			if(nodes.containsKey(nodeEle))
				fromNode = nodes.get(nodeEle);
			else
				fromNode = new Node(nodeEle);
			
			
			nodeEle = stringScan.next();
			
			if(nodes.containsKey(nodeEle))
				toNode = nodes.get(nodeEle);
			else
				toNode = new Node(nodeEle);
			
			//just getting rid of the 5th parm for now
			if(stringScan.hasNext()) 
				stringScan.next();
			
			fromNode.addEdge(toNode);
			if(undirected)
				toNode.addEdge(fromNode);
			
			if(!nodes.containsKey(fromNode.toString())) {
				nodes.put(fromNode.toString(), fromNode);
			}
			
			if(undirected && !nodes.containsKey(toNode.toString())) {
				nodes.put(toNode.toString(), toNode);
			}
			
		}

		
		lineScan.close();
		stringScan.close();
	}
	
	
	void printGraph() {
		ArrayList<Node> printNodes = new ArrayList<Node>(nodes.values());
	    for (Node node : printNodes) {
	    	node.printEdges();
	    }
	}
	
}
