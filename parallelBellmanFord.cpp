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
#include <pthread.h> 
#include <vector>
#include <time.h>
#include "ittnotify.h" // header for VTune calls

#include "graph.h"

#define INT_MAX std::numeric_limits<int>::max()
#define MAX_THREADS 512 

enum SourceNode {
	other = 4,
	rmat = 0,
	roadFLA = 316606,
	roadNY = 140960
};

class parallelBellmanFord {
	graph &g;
	int numThreads;
	SourceNode src;
	std::atomic<int>* distances;

	/* THREAD VARS */
	pthread_t handles[MAX_THREADS];
	int shortNames[MAX_THREADS];
	std::vector<graph::node_t> nodes[MAX_THREADS];

private:
		void printarray(int* array, int size){
		for(int i = 0; i < size; i++){
			std::cout << array[i] << std::endl;
		}
	}

	void initialize() {
		for(int n = g.begin(); n < g.end(); n++) {
			distances[n].store(INT_MAX);
		}
		distances[src].store(0);

		for(int i = 0; i < numThreads; i++){
			nodes[i] = std::vector<int>();
		}

		for(auto i = g.begin(); i < g.size_nodes(); i+=1){
			(nodes[i%numThreads]).push_back(i);
		}
	}

	bool relaxEdge(graph::node_t u, graph::node_t v, graph::edge_t e) {
		int newDist = distances[u].load() + g.get_edge_data(e);
		if( distances[u].load() != INT_MAX && newDist < distances[v].load()) {
			distances[v].store(newDist);
			return true;
		}
		//cout << "** Edge not relaxed, changed bool == false **\n";
		return false;
	}

public:
	parallelBellmanFord(graph &g, int t): g(g), numThreads(t) { 
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
			cout << "type assigned to other" << endl;
			src = other;
		}
	}

	void compute(int myId) {
		
		__itt_resume();
		for(int i = 0 ; i < g.size_nodes() - 1; i++) {
			//cout << "Iteration " << i << endl;
			for(auto& u: nodes[myId%numThreads]) {

				for(auto e = g.edge_begin(u); e < g.edge_end(u); e++) {
					graph::node_t v = g.get_edge_dst(e);
					graph::edge_data_t weight = g.get_edge_data(e);
					if(relaxEdge(u,v,e)) {
						//cout << "Something changed\n";
						//changed = true;
					}
				}
			}
		}
		__itt_pause();
		pthread_exit(NULL);
		
	}

	static void* relax_wrapper(void* object)
	{
		static std::atomic<int> myId {0};
		reinterpret_cast<parallelBellmanFord*>(object)->compute(myId++);
	}

	void bellmanFord(){
		struct timespec tick, tock;         // for measuring runtime
		uint64_t execTime;                  // time in nanoseconds
		initialize();

		int step = g.size_nodes()/numThreads;
		int prev[g.size_nodes()];

		clock_gettime(CLOCK_MONOTONIC_RAW, &tick);
		__itt_resume();
		for(int i = 0; i < numThreads; ++i) {
			int rc = pthread_create(&handles[i], NULL, relax_wrapper, this);
		    if (rc) {
		      std::cout << "Error:unable to create thread," << rc << std::endl;
		      exit(-1);
		    }
		}

		for(int i=0; i < numThreads; i++){
    		pthread_join(handles[i], NULL);
  		}
  		__itt_pause();
  		clock_gettime(CLOCK_MONOTONIC_RAW, &tock);
  		execTime = 1000000000 * (tock.tv_sec - tick.tv_sec) + tock.tv_nsec - tick.tv_nsec;
  		std::cout << "elapsed process CPU time = " << (long long unsigned int)execTime << " nanoseconds\n";

	}

	void printGraphDistances() {
		//std::cout << "Distances" << std::endl;
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
	__itt_pause();
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
	
	parallelBellmanFord bf(g, atoi(argv[2]));

	bf.bellmanFord();
	return 0;
}
