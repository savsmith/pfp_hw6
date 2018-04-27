#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <string>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <limits>
#include "ittnotify.h" // header for VTune calls

#include "graph.h"

#define INT_MAX std::numeric_limits<int>::max()

enum SourceNode {
	other = 4,
	rmat = 0,
	roadFLA = 316606,
	roadNY = 140960
};

class serialBellmanFord {
	graph &g;
	SourceNode src;
	int *distances;

private:
	void initialize() {
		for(int n = g.begin(); n < g.end(); n++) {
			distances[n] = INT_MAX;
		}
		distances[src] = 0;
	}

	bool relaxEdge(graph::node_t u, graph::node_t v, graph::edge_t e) {
		int newDist = distances[u] + g.get_edge_data(e);
		if( distances[u] != INT_MAX && newDist < distances[v]) {
			distances[v] = newDist;
			return true;
		}
		//cout << "** Edge not relaxed, changed bool == false **\n";
		return false;
	}

	void printarray(int* array, int size){
		for(int i = 0; i < size; i++){
			std::cout << array[i] << std::endl;
		}
	}

public:
	serialBellmanFord(graph &g): g(g) {
		distances = new int[g.size_nodes()];

		string type = g.getGraphType();
		if (type.compare("rmat") == 0) {
			src = rmat;
			//cout << "type assigned to rmat" << endl;
		} else if (type.compare("roadFLA") == 0) {
			src = roadFLA;
			//cout << "type assigned to roadFLA" << endl;
		} else if (type.compare("roadNY") == 0) {
			src = roadNY;
			//cout << "type assigned to roadNY" << endl;
		} else if (type.compare("other") == 0) {
			cout << "type assigned to other" << endl;
			src = other;
		}
	}

	void bellmanFord() {
		initialize();
		bool changed = true;

		__itt_resume(); // Start measuring runtime here

		for(auto i = 0; i < g.size_nodes() - 1; i++) {
			cout << "ITERATION " << i+1 << endl;
			if(!changed) {
				cout << "Now we're breaking FREEEEE\n";
				break;
			}
			changed = false;
			for(auto u = g.begin(); u < g.end(); u++) {
				//cout << "Node: " << u << endl;
				for(auto e = g.edge_begin(u); e < g.edge_end(u); e++) {
					//cout << "Edge: " << e << endl;
					graph::node_t v = g.get_edge_dst(e);
					graph::edge_data_t weight = g.get_edge_data(e);
					if(relaxEdge(u,v,e)) {
						changed = true;
						cout << "A change happened\n";
					}
				}
			}
			cout << endl;
		}

		__itt_pause(); //Stop measuring runtime here

		//printGraphDistances();
	}

	void printGraphDistances() {
		std::cout << "Distances" << std::endl;
		for (auto n = g.begin(); n < g.end(); n++) {
			if (distances[n] == INT_MAX) {
				std::cout << n+1 << " INF" << std::endl;
			} else {
				std::cout << n+1 << " " << distances[n] << std::endl;
			}
		}
		cout << endl;
	}

};

int main (int argc, char *argv[]) {
	// Ensure right number of arguments being used
	if (argc < 2) {
	    std::cerr << "Usage: " << argv[0] << " <input.dimacs>\n";
	    return 0;
  	}

	// std::cout << "Starting Program" << std::endl;

	graph g; // Construct graph g from input
	if(!g.construct_from_dimacs(argv[1])) {
		return 0;
	}
	std::cout << "Graph Name: " << g.getName() << std::endl;

	serialBellmanFord bf(g);
	std::cout << "Graph size: " << g.size_nodes() << std::endl;
	bf.bellmanFord();

	return 0;
}
