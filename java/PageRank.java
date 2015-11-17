import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.Map.Entry;

public class PageRank {
	private static final double d = 0.85;
	private Graph graph  = null;
	private HashMap<Node, Double> pageRank = new HashMap<Node, Double>();
	private int size;
	private long startTime = 0;
	private long endTime = 0;
	public int numIter = 0;
	public PageRank(Graph graph) {
		this.graph = graph;
		size = graph.nodes.size();
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
	
	public void printPageRanks(PrintWriter writeTo, int printLimit) {
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
			writeTo.println(i++ + " obj: " + node.element + " with pagerank: " + node.pageRank);
			if(i == printLimit + 1) {
				writeTo.println("(Shortened for length)");
				return;
			}
		}
	}
	
	public long processingTime() {
		return endTime - startTime;
	}
	
	public void getPageRanks() {
		Collection<Node> entries = graph.nodes.values();
		startTime = System.currentTimeMillis();
		System.out.println("SIZE " + size);
		boolean done = false;
		double totalSum = 0.0;
		boolean firstIter = true;
		for(Node node : entries) {
			pageRank.put(node, 1.0 / size);
		}
		while(!done) {
			numIter++;
			totalSum = 0.0;
			for(Node node : entries) {
				double num = pageRank(node);
				totalSum += Math.abs(num - pageRank.get(node));
				node.pageRank = num;
			}
			for(Node node : entries) {
				pageRank.put(node, node.pageRank);
			}
			System.out.println(totalSum);
			if(totalSum < 0.001 && !firstIter) {
				done = true;
				endTime = System.currentTimeMillis();
			}
			firstIter = false;
		}
		
	}
	
	private double pageRank(Node node) {
		double newPagerank = 0.0;
		double sum = 0.0;
		for(int i = 0; i < node.inlinks.size(); i++) {
			sum += (1.0 / node.inlinks.get(i).outlinks.size()) * pageRank.get(node.inlinks.get(i));
		}
		newPagerank = (1 - d) * (1.0 / size) + d * sum;
				
		return newPagerank;
	}
}