import java.io.PrintWriter;
import java.util.ArrayList;



public class Main {
	public static void main(String args[]) {
		PrintWriter writeTo = null;
		int printLimit = 50;
		if(args.length == 0) {
			System.out.println("Must include file name as argument");
			return;
		}
		String fileName = args[0];
		try{
			writeTo= new PrintWriter(fileName.substring(0, fileName.length() - 4) + "Output.txt", "UTF-8");
		}
		catch (Exception e) {
			System.out.println(e.getMessage());
			return;
		}
		Graph graph = new Graph(fileName, true, false, writeTo);
		PageRank pageRank = new PageRank(graph);
		long time = System.currentTimeMillis();		
		pageRank.getPageRanks();
		time = System.currentTimeMillis() - time;
		writeTo.println("Page Rank took " + time + "ms.");
		pageRank.printPageRanks(writeTo, printLimit);
		writeTo.println("Num Iterations: " + pageRank.numIter);
		writeTo.close();
	}
}
