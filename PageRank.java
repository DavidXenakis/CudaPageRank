import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.Map.Entry;
import java.util.Set;

public class PageRank {
	private static final double d = 0.85;
	private Graph graph  = null;
	private HashMap<Node, Double> pageRank = null;
	private HashMap<Node, Integer> r = new HashMap<Node, Integer>();
	private int size;
	public PageRank(Graph graph) {
		this.graph = graph;
		pageRank = new HashMap<Node, Double>();
		size = graph.nodes.size();
	}
	
	private int visitedNode(Node node) {
		int returnVal = 0;
		if(r.get(node) != null) {
			int num = r.get(node);
			returnVal = num +1;
			r.put(node, returnVal);
		} else {
			r.put(node,  1);
			returnVal = 1;
		}
		
		return returnVal;
		
	}
	
	public void printPageRanks() {
		for(Entry<Node, Double> entry : pageRank.entrySet()) {
			System.out.println(entry.getKey().element + " : " + entry.getKey().inlinks.size() + " " + entry.getValue());
		}
	}
	
	public void getPageRanks() {
		Collection<Node> entries = graph.nodes.values();
		for(Node node : entries) {
			r = new HashMap<Node, Integer>();
			if(pageRank.get(node) != null) {
				System.out.println("Converged");
				return;
			}
			double num = pageRank(node);
			node.pageRank = num;
			pageRank.put(node, num);
		}
		
	}
	private double pageRank(Node node) {
		double newPagerank = 0.0;
		double sum = 0.0;
		/*int i = 0;*/
		if(visitedNode(node) >= 2000) {
			return 1.0 / size;
		}
		for(int i = 0; i < node.inlinks.size(); i++) {
			sum += (1.0 / node.inlinks.get(i).outlinks.size()) * pageRank(node.inlinks.get(i));
		}
		//newPagerank = (1 - d) * (1.0 / size) + d * sum;
		newPagerank = (1 - d) + d * sum;
		
		return newPagerank;
	}	
}