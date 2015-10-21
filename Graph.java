import java.util.*;
import java.io.*;

public class Graph {
	public HashMap<String, Node> nodes;
	public boolean undirected;
	public boolean invert;
	
	
	//Currently just call the constructor with the file, only works with csv 
	@SuppressWarnings("resource")
	public Graph(String fileName, boolean undirected, boolean invert) {
		nodes = new HashMap<String, Node>();
		this.undirected = undirected;
		this.invert = invert;
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
		Node fromNode = null;
		Node toNode = null;
		
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
			
			stringScan.useDelimiter(", +");
			
			nodeEle = stringScan.next();
			if(nodeEle.charAt(0) == '"')
				nodeEle = nodeEle.substring(1, nodeEle.length() - 1);
			nodeVal = stringScan.nextInt();
			
			if(nodes.containsKey(nodeEle)) {
				if(invert) {
					toNode = nodes.get(nodeEle);
				} else {
					fromNode = nodes.get(nodeEle);
				}
			} else {
				if(invert) {
					toNode = new Node(nodeEle, nodeVal);
				} else {
					fromNode = new Node(nodeEle, nodeVal);
				}
			}
			
			
			nodeEle = stringScan.next();
			if(nodeEle.charAt(0) == '"')
				nodeEle = nodeEle.substring(1, nodeEle.length() - 1);
			nodeVal = stringScan.nextInt();
			
			if(nodes.containsKey(nodeEle)) {
				if(invert) {
					fromNode = nodes.get(nodeEle);
				} else {
					toNode = nodes.get(nodeEle);
				}
			}else {
				if(invert) {
					fromNode =  new Node(nodeEle, nodeVal);
				} else {
					toNode =  new Node(nodeEle, nodeVal);
				}
			}
			
			//just getting rid of the 5th parm for now
			if(stringScan.hasNext()) 
				stringScan.next();
			
			fromNode.addOutlink(toNode);
			toNode.addInlink(fromNode);
			if(undirected) {
				toNode.addOutlink(fromNode);
				fromNode.addInlink(toNode);
			}
			
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
			
			fromNode.addOutlink(toNode);
			toNode.addInlink(fromNode);
			if(undirected) {
				toNode.addOutlink(fromNode);
				fromNode.addInlink(toNode);
			}
			
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
	    	//node.printEdges();
	    }
	}
	
	public int size() {
		return nodes.size();
	}
	
	public Node get(String str) {
		return nodes.get(str);
	}

	
}
