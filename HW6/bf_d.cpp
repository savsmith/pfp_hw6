#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <string>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <atomic>
#include <omp.h>

//#include "ittnotify.h" // header for VTune calls

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
	std::atomic<int> *distances;

private:
	void set(std::atomic<int> &container, int value){
		auto current = container.load();
		while(!container.compare_exchange_weak(current, value));
	}
	void initialize() {
		for(int n = g.begin(); n < g.end(); n++) {
			set(distances[n], INT_MAX);
		}
		set(distances[src], 0);
	}
	bool relaxEdge(graph::node_t u, graph::node_t v, graph::edge_t e) {
		int newDist = distances[u].load() + g.get_edge_data(e);
		if(distances[u].load() != INT_MAX && newDist < distances[v].load()) {
			set(distances[v], newDist);
			//cout << "Setting node " << u << " to dist " << newDist << " correctly? ("<< (distances[u].load() == newDist) << ") " << endl;
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
		distances = new std::atomic<int>[g.size_nodes()];

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
			//cout << "type assigned to other" << endl;
			src = other;
		}
	}

	uint64_t serialBF(bool print) {
		struct timespec tick, tock;         // for measuring runtime
		uint64_t execTime;                  // time in nanoseconds
		initialize();
		bool changed = true;

		clock_gettime(CLOCK_MONOTONIC_RAW, &tick);
		for(auto i = 0; i < g.size_nodes() - 1; i++) {
			changed = false;
			/* Execute Bellman Ford */
			for(auto e = g.edge_begin(); e < g.edge_end(); e++) {
				//cout << "Edge: " << e << endl;
				graph::node_t u = g.get_edge_src(e);
				graph::node_t v = g.get_edge_dst(e);
				graph::edge_data_t weight = g.get_edge_data(e);
				if(relaxEdge(u,v,e)) {
					changed = true;
				}
			}
			if(!changed)
				break;
		}
		clock_gettime(CLOCK_MONOTONIC_RAW, &tock);
		execTime = 1000000000 * (tock.tv_sec - tick.tv_sec) + tock.tv_nsec - tick.tv_nsec;
  		
		if (print)
			printGraphDistances();

  		return execTime;
	}

	void printGraphDistances() {
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
	    std::cerr << "Usage: " << argv[0] << " <input.dimacs> <print? 0/1>\n";
	    return 0;
  	}

	// std::cout << "Starting Program" << std::endl;
	
	graph g; // Construct graph g from input
	if(!g.construct_from_dimacs(argv[1])) {
		return 0;
	}
	//std::cout << "Graph Name: " << g.getName() << std::endl;
	
	serialBellmanFord bf(g);
	//std::cout << "Graph size: " << g.size_nodes() << std::endl;
	uint64_t execTime = bf.serialBF((bool)atoi(argv[2]));

	if(!((bool)atoi(argv[2])))
		std::cout << (long long unsigned int)execTime;

	return 0;
}
