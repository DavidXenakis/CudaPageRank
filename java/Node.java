import java.util.ArrayList;
import java.util.List;


public class Node {
	List<Node> inlinks;
	List<Node> outlinks;
	//HashMap<String, Node> Edges;
	String element;
	double pageRank;
	int value = 0;
	
	public Node(String element, int value) {
		this.element = element;
		this.value = value;
		this.pageRank = 0.0;
		this.inlinks = new ArrayList<Node>();
		this.outlinks = new ArrayList<Node>();
	}
	
	public Node(String element) {
		this.element = element;
		this.pageRank = 0.0;
		this.inlinks = new ArrayList<Node>();
		this.outlinks = new ArrayList<Node>();
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
	   if(obj instanceof String) {
		   return ((String) obj).equals(this.element);
	   } else if(obj instanceof Node) {
		   Node obj1 = (Node) obj;
		   return obj1.element.equals(this.element);
	   } else {
		   return false;
	   }
	   
   }
   
   public void addInlink(Node newNode) {
	   inlinks.add(newNode);
   }
   
   public void addOutlink(Node newNode) {
	   outlinks.add(newNode);
   }
   
   /*public void printEdges() {
	   ArrayList<Node> printNodes = new ArrayList<Node>(Edges.values());
	   System.out.print(element + "'s " + "nodes: ");
	   for (Node node : printNodes) {
		   System.out.print(node + ", ");
	   }
	   System.out.println();
   }*/
}
