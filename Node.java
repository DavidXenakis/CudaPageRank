import java.util.ArrayList;
import java.util.List;


public class Node {
	List<Node> inlinks;
	List<Node> outlinks;
	double pageRank;
	
	public Node() {
		inlinks = new ArrayList<Node>();
		outlinks = new ArrayList<Node>();
		pageRank = 0.0;
	}
}
