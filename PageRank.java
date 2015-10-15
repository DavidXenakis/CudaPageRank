import java.util.ArrayList;
import java.util.HashMap;

public class PageRank {
	private static final double d = .6;
	private ArrayList<Node> graph  = null;
	private HashMap<Node, Double> pageRank = null;
	private int size;
	public PageRank(ArrayList<Node> graph) {
		this.graph = graph;
		pageRank = new HashMap<Node, Double>();
	}
	public void getPageRanks() {
		for(int i = 0; i < graph.size(); i++) {
			Node node = graph.get(i);
			if(pageRank.get(node) != null) {
				System.out.println("Converged");
				return;
			}
			pageRank.put(node, pageRank(node));
		}
	}
	private double pageRank(Node node) {
		double sum = 0.0;
		if(node.outlinks.size() == 0) {
			return 1 / size;
		}
		for(Node inlink : node.inlinks) {
			sum += (1 / inlink.outlinks.size()) * pageRank(inlink);
		}
		return (1 - d) * (1 / size) + d * sum;
	}
}