import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.Map.Entry;
import java.util.Set;

public class PageRank {
	private static final double d = .6;
	private Graph graph  = null;
	private HashMap<Node, Double> pageRank = null;
	private int size;
	public PageRank(Graph graph) {
		this.graph = graph;
		pageRank = new HashMap<Node, Double>();
		size = graph.nodes.size();
	}
	
	public void printPageRanks() {
		for(Entry<Node, Double> entry : pageRank.entrySet()) {
			System.out.println(entry.getKey().element + " : " + entry.getKey().inlinks.size() + " " + entry.getValue());
		}
	}
	
	public void getPageRanks() {
		Collection<Node> entries = graph.nodes.values();
		for(Node node : entries) {
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
		double newPagerank = 1.0 / size;
		if(pageRank.get(node) == null) {
			pageRank.put(node, 1.0 / size);
		} else {
			System.out.println("HERE");
		}
		double sum = 0.0;
		int i = 0;
		if(node.inlinks.size() > 0) {
			do {
				sum += (1.0 / node.inlinks.get(i).outlinks.size()) * pageRank(node.inlinks.get(i));
				newPagerank = (1 - d) * (1.0 / size) + d * sum;
				i++;
			} while((newPagerank - pageRank.get(node)) >= 0.0015 && i < node.inlinks.size()-1);
		}
		
		return newPagerank;
	}
}