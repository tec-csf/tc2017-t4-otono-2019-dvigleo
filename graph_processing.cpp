#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <chrono>

using namespace std;
typedef PNGraph DGraph;

void GraphML(DGraph graph) {
	ofstream file ("wiki_vote.graphml");
	if (file.is_open()) {
		file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
		file << "<graphml xmlns=\"http://graphml.graphdrawing.org/xmlns\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://graphml.graphdrawing.org/xmlns http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd\">\n";
		file << "<graph id=\"G\" edgedefault=\"directed\">\n";

		for (DGraph::TObj::TNodeI NI = graph->BegNI(); NI < graph->EndNI(); NI++)
			file << "<node id=\"" << NI.GetId() << "\"/>\n";

		int i = 1;
		for (DGraph::TObj::TEdgeI EI = graph->BegEI(); EI < graph->EndEI(); EI++, ++i)
			file << "<edge id=\"e" << i << "\" source=\"" << EI.GetSrcNId() << "\" target=\"" << EI.GetDstNId() << "\"/>\n";

		file << "</graph>\n";
		file << "</graphml>\n";
		file.close();
	}
}

void GEXF(DGraph graph) {
	ofstream file ("wiki_vote.gexf");
	if (file.is_open()) {
		file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
		file << "<gexf xmlns=\"http://www.gexf.net/1.2draft\" version=\"1.2\">\n";
		file << "<graph mode=\"static\" defaultedgetype=\"directed\">\n";

		file << "<nodes>\n";
		for (DGraph::TObj::TNodeI NI = graph->BegNI(); NI < graph->EndNI(); NI++)
			file << "<node id=\"" << NI.GetId() << "\" />\n";
		file << "</nodes>\n";

		file << "<edges>\n";
		int i = 1;
		for (DGraph::TObj::TEdgeI EI = graph->BegEI(); EI < graph->EndEI(); EI++, ++i)
			file << "<edge id=\"" << i << "\" source=\"" << EI.GetSrcNId() << "\" target=\"" << EI.GetDstNId() << "\" />\n";
		file << "</edges>\n";

		file << "</graph>\n";
		file << "</gexf>\n";
		file.close();
	}
}

void GDF(DGraph graph) {
	ofstream file ("wiki_vote.gdf");
	if (file.is_open()) {
		file << "nodedef>id VARCHAR\n";
		for (DGraph::TObj::TNodeI NI = graph->BegNI(); NI < graph->EndNI(); NI++)
			file << NI.GetId() << "\n";

		file << "edgedef>source VARCHAR, destination VARCHAR\n";
		for (DGraph::TObj::TEdgeI EI = graph->BegEI(); EI < graph->EndEI(); EI++)
			file << EI.GetSrcNId() << ", " << EI.GetDstNId() << "\n";

		file.close();
	}
}

void JSON(DGraph graph) {
  ofstream file ("wiki_vote.json");
	if (file.is_open()) {
		file << "{ \"graph\": {\n";
		file << "\"nodes\": [\n";
		for (DGraph::TObj::TNodeI NI = graph->BegNI(); NI < graph->EndNI();) {
			file << "{ \"id\": \"" << NI.GetId() << "\" }";
			if (NI++ == graph->EndNI())
				file << " ],\n";
			else
				file << ",\n";
		}

		file << "\"edges\": [\n";
		for (DGraph::TObj::TEdgeI EI = graph->BegEI(); EI < graph->EndEI();) {
			file << "{ \"source\": \"" << EI.GetSrcNId() << "\", \"target\": \"" << EI.GetDstNId() << "\" }";
			if (EI++ == graph->EndEI())
				file << " ]\n";
			else
				file << ",\n";
		}
		file << "} }";
		file.close();
	}
}

int main() {
	DGraph graph = TSnap::LoadEdgeList<DGraph>("wiki-Vote.txt",0,1);

	cout << "\nThe Wiki-Vote dataset has been imported succesfully\nExporting...\n" << endl;
	cout << "Format\tExecution Time (ms)" << endl;
	for(int i = 0; i < 4; ++i) {
		switch(i){
			case 0:
			{
				auto start = chrono::steady_clock::now();
				GraphML(graph);
				auto end = chrono::steady_clock::now();
				cout << "GraphML\t     " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << endl;
			}
			break;
			case 1:
			{
				auto start = chrono::steady_clock::now();
				GEXF(graph);
				auto end = chrono::steady_clock::now();
				cout << "GEFX\t     " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << endl;
			}
			break;
			case 2:
			{
				auto start = chrono::steady_clock::now();
				GDF(graph);
				auto end = chrono::steady_clock::now();
				cout << "GDF\t     " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << endl;
			}
			break;
			case 3:
			{
				auto start = chrono::steady_clock::now();
				JSON(graph);
				auto end = chrono::steady_clock::now();
				cout << "JSON\t     " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << endl;
			}
			break;
		}
	}
	return 0;
}
