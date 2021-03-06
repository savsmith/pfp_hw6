#ifndef SP2018_CS377P_GRAPH_H
#define SP2018_CS377P_GRAPH_H

#include <string>
#include <cassert>

using namespace std;

#ifndef SP2018_CS377P_STRUCT_OF_ARRAYS
struct Node {
  double pr[2];
  int out_degree;
};
#endif

class graph {
public:
  using node_t = int;
#ifndef SP2018_CS377P_STRUCT_OF_ARRAYS
  using node_data_t = Node;
#endif
  using edge_t = int;
  using edge_data_t = int;
  using in_edge_t = edge_t;
  using in_edge_data_t = edge_data_t;

private:
  node_t num_nodes;
  edge_t num_edges;
  string name;

#ifndef SP2018_CS377P_STRUCT_OF_ARRAYS
  node_data_t* node;
#else
  double* pr[2];
  int* out_degree;
#endif

  // the csr graph
  edge_t* edge_range;
  node_t* edge_dst;
  node_t* edge_src; /* Part D Code */
  edge_data_t* edge_data;

  // the transposed csr graph
  in_edge_t* in_edge_range;
  node_t* in_edge_dst;
  node_t* in_edge_src;
  in_edge_data_t* in_edge_data;

  bool is_allocated;

public:
  node_t size_nodes() { return num_nodes; }
  edge_t size_edges() { return num_edges; }

  node_t begin() { return 0; }
  node_t end() { return num_nodes; }

  edge_t edge_begin() { return 0; }
  edge_t edge_end() { return num_edges; }

  string getName() { return name; }
  string getGraphType();

#ifndef SP2018_CS377P_STRUCT_OF_ARRAYS
  node_data_t& get_data(node_t n) { assert(n >= 0 && n < num_nodes); return node[n]; }
  double& get_pr(node_t n, bool which) { assert( n >= 0 && n < num_nodes); return node[n].pr[which]; }
  int& get_out_degree(node_t n) { assert( n >= 0 && n < num_nodes); return node[n].out_degree; }
#else
  double& get_pr(node_t n, bool which) { assert( n >= 0 && n < num_nodes); return pr[which][n]; }
  int& get_out_degree(node_t n) { assert( n >= 0 && n < num_nodes); return out_degree[n]; }
#endif

  edge_t edge_begin(node_t n) { assert(n >=0 && n < num_nodes); return edge_range[n]; }
  edge_t edge_end(node_t n) { assert(n >= 0 && n < num_nodes); return edge_range[n+1]; }

  node_t get_edge_dst(edge_t e) { assert(e >= 0 && e < num_edges); return edge_dst[e]; }
  node_t get_edge_src(edge_t e) { assert(e >= 0 && e < num_edges); return edge_src[e]; }
  edge_data_t& get_edge_data(edge_t e) { assert(e >= 0 && e < num_edges); return edge_data[e]; }

  in_edge_t in_edge_begin(node_t n) { assert(n >= 0 && n < num_nodes); return in_edge_range[n]; }
  in_edge_t in_edge_end(node_t n) { assert(n >= 0 && n < num_nodes); return in_edge_range[n+1]; }

  node_t get_in_edge_dst(in_edge_t ie) { assert(ie >= 0 && ie < num_edges); return in_edge_dst[ie]; }
  node_t get_in_edge_src(in_edge_t ie) { assert(ie >= 0 && ie < num_edges); return in_edge_src[ie]; }
  in_edge_data_t& get_in_edge_data(in_edge_t ie) { assert(ie >= 0 && ie < num_edges); return in_edge_data[ie]; }

  graph();
  ~graph();
  graph(const graph&) = delete;
  graph& operator=(const graph&) = delete;

  bool construct_from_dimacs(const std::string dimacs_name);
  void print_csr();
  void print_csr_transposed();
  void print_edges();
  void print_in_edges();
};

#endif
