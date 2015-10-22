import java.io.PrintWriter;
import java.util.ArrayList;



public class Main {
	public static void main(String args[]) {
		PrintWriter writeTo = null;
		int printLimit = 50;
		String fileName = "soc-LiveJournal1";
		try{
			writeTo= new PrintWriter(fileName + "Output.txt", "UTF-8");
		}
		catch (Exception e) {
			System.out.println(e.getMessage());
			return;
		}
		Graph graph = new Graph(fileName + ".txt", true, false, writeTo);
		PageRank pageRank = new PageRank(graph);
		long time = System.currentTimeMillis();		
		pageRank.getPageRanks();
		time = System.currentTimeMillis() - time;
		writeTo.println("Page Rank took " + time + "ms.");
		pageRank.printPageRanks(writeTo, printLimit);
		writeTo.println("Num Iterations: " + pageRank.numIter);
		writeTo.close();
//		ArrayList<Node> graph = new ArrayList<Node>();
//		PageRank pageRank = null;
//		graph.add(new Node()s);
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
