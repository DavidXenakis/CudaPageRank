import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.Map.Entry;
import java.util.Set;

public class PageRank {
	private static final double d = 0.85;
	private Graph graph  = null;
	private HashMap<Node, Double> pageRank2 = null;
	private HashMap<Node, Double> pageRank = new HashMap<Node, Double>();
	private HashMap<Node, Integer> r = new HashMap<Node, Integer>();
	private int size;
	public int numIter = 0;
	public PageRank(Graph graph) {
		this.graph = graph;
		pageRank2 = new HashMap<Node, Double>();
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
		ArrayList<Node> nodes = new ArrayList<Node>();
		for(Entry<Node, Double> entry : pageRank.entrySet()) {
			nodes.add(entry.getKey());
		}
		Collections.sort(nodes, new Comparator<Node>() {

			@Override
			public int compare(Node o1, Node o2) {
				if(o1.pageRank < o2.pageRank) {
					return 1;
				} else {
					return -1;
				}
			}
			
		});
		int i = 1;
		for(Node node : nodes) {
			System.out.println(i++ + " obj: " + node.element + " with pagerank: " + node.pageRank);
		}
	}
	
	public void getPageRanks2() {
		Collection<Node> entries = graph.nodes.values();
		for(Node node : entries) {
			r = new HashMap<Node, Integer>();
			double num = pageRank2(node);
			node.pageRank = num;
			pageRank2.put(node, num);
		}
		
	}
	
	public void getPageRanks() {
		Collection<Node> entries = graph.nodes.values();
		boolean done = false;
		double oldSum = 0.0;
		double newSum = 0.0;
		boolean firstIter = true;
		while(!done) {
			numIter++;
			oldSum = 0.0;
			newSum = 0.0;
			for(Node node : entries) {
				double num = pageRank(node);
				newSum += num;
				oldSum += pageRank.get(node);
				node.pageRank = num;
				pageRank.put(node, num);
			}
			if(Math.abs(newSum - oldSum) < 0.0001 && !firstIter) {
				done = true;
			}
			firstIter = false;
		}
		
	}
	
	private double pageRank(Node node) {
		double newPagerank = 0.0;
		double sum = 0.0;
		double tempSum = 0.0;
		if(pageRank.get(node) == null) {
			pageRank.put(node, 1.0 / size);
			return 1.0 / size;
		}
		
		for(int i = 0; i < node.inlinks.size(); i++) {
			sum += (1.0 / node.inlinks.get(i).outlinks.size()) * pageRank.get(node.inlinks.get(i));
		}
		newPagerank = (1 - d) * (1.0 / size) + d * sum;
				
		return newPagerank;
	}
	
	private double pageRank2(Node node) {
		double newPagerank = 0.0;
		double sum = 0.0;
		double tempSum = 0.0;
		/*int i = 0;*/
		if(visitedNode(node) >= 10) {
			return 1.0 / size;
		}
		
		for(int i = 0; i < node.inlinks.size(); i++) {
			sum += (1.0 / node.inlinks.get(i).outlinks.size()) * pageRank2(node.inlinks.get(i));
		}
		newPagerank = (1 - d) * (1.0 / size) + d * sum;
		
		return newPagerank;
	}	
}