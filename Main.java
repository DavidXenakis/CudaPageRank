import java.util.ArrayList;



public class Main {
	public static void main(String args[]) {
		Graph graph = new Graph("football.csv", false, true);
		Node node = new Node("Buffalo");
		for(Node node1 : graph.nodes.get("Northeastern").outlinks) {
			System.out.println(node1.element);
		}
		PageRank pageRank = new PageRank(graph);
		pageRank.getPageRanks();
		pageRank.printPageRanks();
//		ArrayList<Node> graph = new ArrayList<Node>();
//		PageRank pageRank = null;
//		graph.add(new Node());
//		for(int i = 0; i < 10; i++) {
//			Node node1 = graph.get(0);
//			Node node2 = new Node();
//			node1.outlinks.add(node2);
//			node2.inlinks.add(node1);
//			graph.add(node2);
//		}
//		graph.get(1).outlinks.add(graph.get(4));
//		pageRank = new PageRank(graph);
//		pageRank.getPageRanks();
//		pageRank.printPageRanks();
	}
}
